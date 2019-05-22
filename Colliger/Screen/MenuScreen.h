#pragma once

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Screen.h"
#include "Helper/Viewport/TryFitViewport.h"
#include "Helper/Button/Button.h"
#include "PursuitScene_1.h"
#include "ColligerLogoScene.h"

class MenuScreen :
	public Screen
{
private:
	using MenuButton = Button<sf::Text>;

	enum State
	{
		LOGO_ANIMATION,
		BUTTON_ALPHA_ANIMATION,
		PURSUIT_ANIMATION
	};
public:
	MenuScreen(Game& game);
	~MenuScreen() = default;

	virtual void Update(float deltaTime) override;

	virtual void Draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void Resize(const sf::Vector2u& size) override;
private:
	MenuButton& AppendButton(const std::string& str, const sf::Vector2f& pos); // Append button by pos

	MenuButton& AppendButton(const std::string& str); // Append button to menuButtons.end()

	void InitPursuitScene(bool initialInit);
private:
	// Logo scene consts
	static const int LOGO_CHARACTER_SIZE;
	static const float LOGO_LETTER_SPACING;
	static const float LOGO_Y_SCALE;
	static const float LOGO_COLLIGER_VELOCITY;
	static const float LOGO_SCENE_DELAY;
	static const sf::Color LOGO_TEXT_FILL_COLOR;
	static const sf::Color LOGO_TEXT_OUTLINE_COLOR;
	static const float LOGO_TEXT_OUTLINE_THICKNESS;
	// Pursuit consts
	static const float PURSUIT_ACTOR_VELOCITY;
	static const float PURSUIT_ACTOR_SPACING;
	static const float PURSUIT_SCENE_DELAY;
	static const float PURSUIT_Y_SCALE;
	static const int PURSUIT_MIN_GHOST_COUNT;
	static const int PURSUIT_MAX_GHOST_COUNT;
	// Button consts
	static const int BUTTON_CHARACTER_SIZE;
	static const int BUTTON_Y_OFFSET;
	static const float BUTTON_OUTLINE_EFFECT_THICKNESS;
	static const float BUTTON_X_SCALE;
	static const float BUTTON_FIRST_Y_SCALE;
	static const float BUTTON_EFFECT_SCALE;
	static const sf::Uint8 BUTTON_ALPHA_INC;
	static const BaseButton::CoordProjector BUTTON_COORD_PROJECTOR;
	// Other consts
	static const sf::Color BACKGROUND_COLOR;

	std::list<MenuButton> menuButtons;
	// sf::Color uses integer values which makes it hard to 
	// make a fade animation (as increments are floats which 
	// might get rounded to 0) so a custom alpha "counter"
	// is used
	State state;
	sf::RectangleShape background;
	float buttonAlpha;
	PursuitScene_1 pursuitScene;
	Random<int> ghostCountRandom;
	ColligerLogoScene logoScene;
	std::unique_ptr<Viewport> viewport;
};
