#include "MenuScreen.h"

#include "Game.h"
#include "Assets.h"
#include "Helper/Button/ScaleButtonEffect.h"
#include "Helper/Button/OutlineButtonEffect.h"
#include "Helper/Utils/Math.h"
#include "GamePlayScreen.h"
#include "TransitionScreen.h"

// Logo scene consts
const int MenuScreen::LOGO_CHARACTER_SIZE = 80;
const float MenuScreen::LOGO_LETTER_SPACING = 3;
const float MenuScreen::LOGO_Y_SCALE = 0.2f;
const float MenuScreen::LOGO_COLLIGER_VELOCITY = 140.f;
const float MenuScreen::LOGO_SCENE_DELAY = 0.5f;
const sf::Color MenuScreen::LOGO_TEXT_FILL_COLOR = sf::Color(0x2b94DFFF);
const sf::Color MenuScreen::LOGO_TEXT_OUTLINE_COLOR = sf::Color(0x404040FF);
const float MenuScreen::LOGO_TEXT_OUTLINE_THICKNESS = 3;
// Pursuit consts
const float MenuScreen::PURSUIT_Y_SCALE = 0.85f;
const float MenuScreen::PURSUIT_ACTOR_VELOCITY = 250.f;
const float MenuScreen::PURSUIT_ACTOR_SPACING = 4.f;
const float MenuScreen::PURSUIT_SCENE_DELAY = 1.5f;
const int MenuScreen::PURSUIT_MIN_GHOST_COUNT = 3;
const int MenuScreen::PURSUIT_MAX_GHOST_COUNT = 6;
// Button consts
const int MenuScreen::BUTTON_CHARACTER_SIZE = 46;
const int MenuScreen::BUTTON_Y_OFFSET = 22;
const float MenuScreen::BUTTON_OUTLINE_EFFECT_THICKNESS = 3.f;
const float MenuScreen::BUTTON_X_SCALE = 0.5f;
const float MenuScreen::BUTTON_FIRST_Y_SCALE = 0.35f;
const float MenuScreen::BUTTON_EFFECT_SCALE = 0.85f;
const sf::Uint8 MenuScreen::BUTTON_ALPHA_INC = 100;
const BaseButton::CoordProjector MenuScreen::BUTTON_COORD_PROJECTOR = [](const Game & game, const sf::Vector2i & coord)
{
	return game.GetWindow().mapPixelToCoords(coord);
};
// Other consts
const sf::Color MenuScreen::BACKGROUND_COLOR = sf::Color(0x00173DFF);

MenuScreen::MenuScreen(Game& game) :
	Screen(game),
	state(State::LOGO_ANIMATION),
	background(),
	menuButtons(),
	buttonAlpha(0.f),
	logoScene(game.GetWindow().getSize(), game.GetResourceManager()),
	pursuitScene(game.GetWindow().getSize(), game.GetResourceManager()),
	ghostCountRandom(std::uniform_int_distribution<int>(PURSUIT_MIN_GHOST_COUNT, PURSUIT_MAX_GHOST_COUNT)),
	viewport(new TryFitViewport())
{
	// Set viewport to screen coords
	sf::Vector2u windowSize = game.GetWindow().getSize();
	float wx = static_cast<float>(windowSize.x), wy = static_cast<float>(windowSize.y);
	viewport->GetView().reset({0.f, 0.f, wx, wy });

	// Init background
	background.setSize({ wx, wy });
	background.setFillColor(BACKGROUND_COLOR);
	
	// Init buttons
	MenuButton& spBtn = AppendButton("SinglePlayer", { BUTTON_X_SCALE * windowSize.x, BUTTON_FIRST_Y_SCALE * windowSize.y });
	spBtn.AddListener([this]() 
		{ 
			// Deactivate buttons
			for (MenuButton& button : this->menuButtons)
			{
				button.SetActive(false);
			}

			Game& game = this->game;
			std::shared_ptr<Screen> me = game.GetScreen();
			std::shared_ptr<Screen> transition = std::make_shared<TransitionScreen>(
				game,
				[&game]() { return std::static_pointer_cast<Screen>(std::make_shared<GamePlayScreen>(game)); }
			);
			game.SetScreen(transition);
		});
	AppendButton("Multiplayer");
	AppendButton("ScoreBoard");
	MenuButton& exitBtn = AppendButton("Exit");
	exitBtn.AddListener([&game]() { game.Exit(); });

	// Init logo scene
	sf::Text& text = logoScene.GetText(); 
	text.setFillColor(LOGO_TEXT_FILL_COLOR);
	text.setOutlineColor(LOGO_TEXT_OUTLINE_COLOR);
	text.setOutlineThickness(LOGO_TEXT_OUTLINE_THICKNESS);
	logoScene.Init(
		LOGO_CHARACTER_SIZE,
		LOGO_LETTER_SPACING,
		LOGO_Y_SCALE,
		LOGO_COLLIGER_VELOCITY
	);
	logoScene.SetDelay(LOGO_SCENE_DELAY);
}

void MenuScreen::Update(float deltaTime)
{
	switch (state)
	{
	case LOGO_ANIMATION:
	{
		if (logoScene.GetState() == DelayedScene::PlayState::FINISHED)
		{
			state = State::BUTTON_ALPHA_ANIMATION;
		}
		else
		{
			logoScene.Update(deltaTime);
		}

		break;
	}
	case BUTTON_ALPHA_ANIMATION:
	{
		if (menuButtons.begin()->GetSprite().getFillColor().a >= 255) // Button alpha anim finished
		{
			state = PURSUIT_ANIMATION;
			InitPursuitScene(true);

			for (MenuButton& button : menuButtons)
			{
				button.SetActive(true);
			}
		}
		else
		{
			for (MenuButton& button : menuButtons)
			{
				sf::Uint8 prevAlpha = buttonAlpha;
				buttonAlpha += deltaTime * BUTTON_ALPHA_INC;
				sf::Uint8 alpha = Min(255, std::abs(static_cast<sf::Uint8>(buttonAlpha)));
				if (alpha < prevAlpha) // handle overflows
				{
					alpha = 255;
				}
				sf::Text& text = button.GetSprite();
				sf::Color fc = text.getFillColor();
				sf::Color oc = text.getOutlineColor();
				fc.a = alpha;
				oc.a = alpha;
				text.setFillColor(fc);
				text.setOutlineColor(oc);
			}
		}

		break;
	}
	default: // PURSUIT_ANIMATION
	{
		if (pursuitScene.GetState() == DelayedScene::PlayState::FINISHED)
		{
			InitPursuitScene(false);
		}
		else
		{
			pursuitScene.Update(deltaTime);
		}
	}
	}
}

void MenuScreen::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.setView(viewport->GetView());
	target.draw(background, states);
	target.draw(logoScene, states);
	target.draw(pursuitScene, states);
	for (const MenuButton& button : menuButtons)
	{
		target.draw(button, states);
	}
	target.setView(target.getDefaultView());
}

void MenuScreen::Resize(const sf::Vector2u& size)
{
	viewport->Update(size);
}

MenuScreen::MenuButton& MenuScreen::AppendButton(const std::string& str, const sf::Vector2f& pos)
{
	// Append new button

	menuButtons.emplace_back(BUTTON_COORD_PROJECTOR);
	MenuButton& button = *menuButtons.rbegin();

	// Init text

	sf::Font& gameFont = *Assets::GAME_FONT.Get(game.GetResourceManager());
	sf::Text text(str, gameFont, BUTTON_CHARACTER_SIZE);
	text.setPosition(pos);
	// Set initial colors to invisible
	sf::Color fillColor = text.getFillColor();
	sf::Color outlineColor = text.getOutlineColor();
	fillColor.a = 0;
	outlineColor.a = 0;
	text.setFillColor(fillColor);
	text.setOutlineColor(outlineColor);
	// Set origin to center
	sf::FloatRect bounds = text.getGlobalBounds();
	text.setOrigin({ bounds.width / 2.f, bounds.height / 2.f });

	// Set sprite and effects
	button.SetSprite(text);
	button.SetButtonEffect(
		ButtonEffect::EffectType::CLICK, 
		std::make_shared<ScaleButtonEffect<sf::Text>>(sf::Vector2f{BUTTON_EFFECT_SCALE, BUTTON_EFFECT_SCALE})
	);
	// Make the OutlineButtonEffect respects the alpha values
	button.SetButtonEffect(
		ButtonEffect::EffectType::HOVER, 
		std::make_shared<OutlineButtonEffect<sf::Text>>(BUTTON_OUTLINE_EFFECT_THICKNESS, sf::Color::Red, true)
	);
	button.Register(game.GetEventDispatcher());

	return button;
}

MenuScreen::MenuButton& MenuScreen::AppendButton(const std::string& str)
{
	MenuButton& upperButton = *menuButtons.rbegin();
	sf::FloatRect bounds = upperButton.GetButtonBounds();
	return AppendButton(str, { bounds.left + bounds.width / 2.f, bounds.top + 1.5f * bounds.height + BUTTON_Y_OFFSET }); // Buttons assumed to be of the same height
}

void MenuScreen::InitPursuitScene(bool initialInit)
{
	if (initialInit)
	{
		pursuitScene.SetActorVelocity(PURSUIT_ACTOR_VELOCITY);
		pursuitScene.SetActorSpacing(PURSUIT_ACTOR_SPACING);
		Random<bool, std::bernoulli_distribution> stateRandom;
		pursuitScene.Init(
			static_cast<PursuitScene_1::Type>(stateRandom.Next() + 1),
			stateRandom.Next() ? Direction::LEFT : Direction::RIGHT,
			PURSUIT_Y_SCALE,
			ghostCountRandom.Next()
		);
		pursuitScene.SetDelay(PURSUIT_SCENE_DELAY);
	}
	else
	{
		PursuitScene_1::Type type = pursuitScene.GetType();
		if (type == PursuitScene_1::Type::COLLIGER_CHASING)
		{
			type = PursuitScene_1::Type::COLLIGER_CHASED;
		}
		else
		{
			type = PursuitScene_1::Type::COLLIGER_CHASING;
		}

		Direction direction = pursuitScene.GetEntranceDirection();
		if (direction == Direction::LEFT)
		{
			direction = Direction::RIGHT;
		}
		else
		{
			direction = Direction::LEFT;
		}

		pursuitScene.Init(type, direction, PURSUIT_Y_SCALE, ghostCountRandom.Next());
	}
}
