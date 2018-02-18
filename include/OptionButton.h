#pragma once
#include "Label.h"
#include "Thor\Animations.hpp"

extern ResourceManager resourceManager;

class OptionButton
{
public:
	OptionButton(sf::Vector2f position, std::string name, sf::Font& font, std::string iconName, std::string textureName);
	void update();
	void render(sf::RenderWindow& window);
	void select();
	void deSelect();
	void pressed(bool isPressed);

	void setUpAnimation(); //sets up our animations
	void applyAlpha(float value);

	//Getters
	std::string& getName();
	bool getSelected();

	//Setters
	void setText(std::string text);
	void setText(std::string text, sf::Font& font);

private:
	bool m_selected; //bool to hold whether a button is selected or not
	bool m_pressed; //bool to hold wheter the button was pressed or not
	sf::Vector2f m_position; //position that we want to draw at
	sf::Sprite m_sprite; //our button sprite
	sf::Sprite m_icon; //our icon sprite
	sf::Sprite m_pressedSprite;
	std::string m_name; //the name of our button
	Label m_label;

	//Animation variables
	sf::Clock m_animationClock, m_iconAnimationClock, m_pressedAnimationClock, m_loopClock; //our animation clock, we will use this to update our animations
	sf::IntRect m_selectedRect, m_iconSelectedRect; //the frame of our spritesheet we show when our button is selected
	sf::IntRect m_unselectedRect, m_iconUnselectedRect;//the frame of our spritesheet we show when our button is not selected
	thor::FrameAnimation m_selectAnimation, m_deselectAnimation, m_iconSelectAnimation, m_iconDeselectAnimation, m_pressedAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	thor::Animator<sf::Sprite, std::string> m_buttonAnimator;
	thor::Animator<sf::Sprite, std::string> m_iconAnimator;
	thor::Animator<sf::Sprite, std::string> m_pressedAnimator;
};