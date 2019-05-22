#include "TransitionScreen.h"

#include "Game.h"

TransitionScreen::FactoryTask::FactoryTask(const ScreenFactory& screenFactory) :
	started(false),
	screenFactory(screenFactory),
	promisePtr(std::make_shared<std::promise<ScreenPtr>>())
{
}

TransitionScreen::FactoryTask::FactoryTask(const FactoryTask& other) :
	started(false),
	screenFactory(),
	promisePtr()
{
	CopyFrom(other);
}

TransitionScreen::FactoryTask& TransitionScreen::FactoryTask::operator=(const FactoryTask& other)
{
	CopyFrom(other);

	return *this;
}

TransitionScreen::FactoryTask::FactoryTask(FactoryTask&& other) noexcept :
	started(false),
	screenFactory(),
	promisePtr()
{
	MoveFrom(other);
}

TransitionScreen::FactoryTask& TransitionScreen::FactoryTask::operator=(FactoryTask&& other) noexcept
{
	MoveFrom(other);

	return *this;
}

void TransitionScreen::FactoryTask::operator()()
{
	bool b = false;
	if (started.compare_exchange_strong(b, true))
	{
		try
		{
			promisePtr->set_value(screenFactory());
		}
		catch (...)
		{
			promisePtr->set_exception(std::current_exception());
		}
	}
}

TransitionScreen::ScreenFuture TransitionScreen::FactoryTask::GetFuture() const
{
	return promisePtr->get_future();
}

void TransitionScreen::FactoryTask::CopyFrom(const FactoryTask& other)
{
	screenFactory = other.screenFactory;
	promisePtr = other.promisePtr;
}

void TransitionScreen::FactoryTask::MoveFrom(FactoryTask& other)
{
	screenFactory = std::move(other.screenFactory);
	promisePtr = std::move(other.promisePtr);
}

///////////////////////////////////////////////////////////////////////////////////////

const float TransitionScreen::ALPHA_INCREASE_RATE = 700.f;

TransitionScreen::TransitionScreen(Game& game, const ScreenFactory& nextScreenFactory) :
	Screen(game),
	previousScreen(game.GetScreen()),
	nextScreenFuture(),
	fadeOut(FadeAlpha::Type::FADE_OUT),
	fadeIn(FadeAlpha::Type::FADE_IN),
	fadeColor(sf::Color::Black),
	renderState(RenderState::PREV_SCREEN),
	rect(),
	factoryThread(1, std::chrono::seconds(0)) // Only used once so no need to set wait value
{
	fadeColor.a = 0;

	// Set rects bounds
	const sf::View& prevScreenView = previousScreen->GetView();
	sf::Vector2f size = prevScreenView.getSize(), center = prevScreenView.getCenter();;
	rect.setSize(size);
	rect.setPosition({ center.x - size.x / 2.f, center.y - size.y / 2.f });

	// Set inc scale
	fadeOut.SetAlphaIncreaseRate(ALPHA_INCREASE_RATE);
	fadeIn.SetAlphaIncreaseRate(ALPHA_INCREASE_RATE);

	// Spawn future
	FactoryTask task(nextScreenFactory);
	nextScreenFuture = task.GetFuture();
	factoryThread.Submit(task);
	factoryThread.Start();
}

void TransitionScreen::Resize(const sf::Vector2u& newSize)
{	
	if (renderState == RenderState::PREV_SCREEN)
	{
		previousScreen->Resize(newSize);
	}
	else
	{
		if (ScreenFutureReady()) // Only resize next screen if it's ready
		{
			nextScreenFuture.get()->Resize(newSize);
		}
	}
}

void TransitionScreen::Update(float deltaTime)
{
	// Make sure this scope outlives this screen
	// if Game::SetScreen is called to avoid
	// nuclear explosions
	ScreenPtr me;
	if (renderState == RenderState::PREV_SCREEN)
	{
		if (fadeOut.IsRunning()) // Still fading out
		{
			fadeColor.a = static_cast<sf::Uint8>(fadeOut.GetAlpha());
			fadeOut.Update(deltaTime);
			previousScreen->Update(deltaTime);
		}
		else // Fade out finished
		{
			renderState = RenderState::NEXT_SCREEN;
		}
	}
	else
	{
		bool ready = ScreenFutureReady();
		if (ready)
		{
			ScreenPtr screen = nextScreenFuture.get();
			if (fadeIn.IsRunning()) // Still fading in
			{
				fadeColor.a = static_cast<sf::Uint8>(fadeIn.GetAlpha());
				fadeIn.Update(deltaTime);
				screen->Update(deltaTime);
			}
			else // Finished fading in
			{
				me = game.GetScreen();
				game.SetScreen(screen);
			}
		}
		else
		{
			fadeColor.a = static_cast<sf::Uint8>(fadeOut.GetAlpha()); // Would have been max by now
		}
	}

	rect.setFillColor(fadeColor);
}

void TransitionScreen::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	ScreenPtr screenToDraw;
	const sf::View* view = nullptr;
	if (renderState == RenderState::PREV_SCREEN)
	{
		screenToDraw = previousScreen;
		view = &previousScreen->GetView();
	}
	else
	{
		if (ScreenFutureReady())
		{
			screenToDraw = nextScreenFuture.get();
			view = &screenToDraw->GetView();
		}
		else
		{
			// No screen to draw
			view = &previousScreen->GetView();
		}
	}

	target.setView(*view);
	if (screenToDraw)
	{
		target.draw(*screenToDraw, states);
	}
	target.draw(rect);
	target.setView(target.getDefaultView());
}

bool TransitionScreen::ScreenFutureReady() const
{
	return nextScreenFuture.wait_for(std::chrono::seconds(0L)) == std::future_status::ready;
}
