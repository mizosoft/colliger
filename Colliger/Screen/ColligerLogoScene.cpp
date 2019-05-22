#include "ColligerLogoScene.h"

#include "Assets.h"

#include "Entity/ColligerAnimationData.h"

float logoVelocity;

ColligerLogoScene::ColligerLogoScene(const sf::Vector2u& worldBounds, ResourceManager& resourceManager) :
	worldBounds(worldBounds),
	colliger(resourceManager),
	text("olliger", *Assets::GAME_FONT.Get(resourceManager)),
	textVelocity(0.f), // Determined on Init(...)
	colligerTargetX(0.f),
	textTargetX(0.f)
{
}

void ColligerLogoScene::Init(
	unsigned int characterSize, 
	float letterSpacing,
	float yPosScale, 
	float colligerVelocity
)
{
	float yPos = worldBounds.y * yPosScale;

	// Init colliger

	// Set unscaled bounds and velcity
	colliger.SetDirection(Direction::RIGHT);
	colliger.SetVelocity(colligerVelocity);

	// Set scale
	float colligerScale = characterSize / colliger.GetBounds().width;
	colliger.SetScale({ colligerScale, colligerScale });

	// Set center pos
	sf::FloatRect colligerScaledBounds = colliger.GetBounds();
	colliger.SetCenterPosition({ -1 * (colligerScaledBounds.width / 2.f + letterSpacing), yPos }); // Before screen by letterSpacing

	// Init text

	// Set characterSize
	text.setCharacterSize(characterSize);
	text.setLetterSpacing(letterSpacing);
	sf::FloatRect textBounds = text.getGlobalBounds();
	text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);
	text.setPosition({ worldBounds.x + textBounds.width / 2.f + letterSpacing, yPos }); // After screen by letterSpacing

	// Calculate textVelocity from colligerVelocity
	float logoWidth = colligerScaledBounds.width + letterSpacing + textBounds.width;
	float logoLeft = (worldBounds.x - logoWidth) / 2.f; // Centered logo left coord
	colligerTargetX = logoLeft + colligerScaledBounds.width / 2.f; // Where colliger should stop
	textTargetX = logoLeft + colligerScaledBounds.width + letterSpacing + textBounds.width / 2.f; // Where text should stop
	float colligerDistance = colligerTargetX - colliger.GetCenterPosition().x; // The distance colliger should cover
	float textDistance = text.getPosition().x - textTargetX; // The distance text should cover
	// t1 = t2, d1 / v1 = d2 / v2, we have (d1, d2, v1), v2 = v1 * d2 / d1;
	textVelocity = colligerVelocity * textDistance / colligerDistance;
}

sf::Text& ColligerLogoScene::GetText()
{
	return text;
}

void ColligerLogoScene::Update(float deltaTime)
{
	UpdateTicker(deltaTime);

	if (state == PlayState::PLAYING)
	{
		colliger.Update(deltaTime);
		text.move({ -1 * deltaTime * textVelocity, 0.f });

		sf::Vector2f colligerPos = colliger.GetCenterPosition(), textPos = text.getPosition();
		if (colligerPos.x >= colligerTargetX || textPos.x <= textTargetX)
		{
			// The animation is finished
			state = PlayState::FINISHED;

			// Align to targetPos
			colligerPos.x = colligerTargetX;
			colliger.SetCenterPosition(colligerPos);
			textPos.x = textTargetX;
			text.setPosition(textPos);

			auto data = ColligerAnimationData::INSTANCE;
			colliger.FreezeAnimationOn(data.GetFrame(colliger.GetOrientation(), colliger.GetStyle(), ColligerMouthLevel::OPEN_3));
		}
	}
}

void ColligerLogoScene::Draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (state >= PlayState::PLAYING) // Draw when PLAYING or FINISHED
	{
		target.draw(colliger, states);
		target.draw(text, states);
	}
}
