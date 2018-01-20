#pragma once
#include "Label.h"
#include "Thor\Animations.hpp"

extern ResourceManager resourceManager;

class Button
{
public:
	Button(sf::Vector2f position, std::string name, std::string iconName);
	void update();
	void render(sf::RenderWindow& window);
	void fade(int desiredAlpha);
	void select();
	void deSelect();

	void setUpAnimation(); //sets up our animations
	void applyAlpha(float value);

	//Getters
	std::string& getName();
	float getAlpha();
	bool getSelected();

	//Setters
	void setAlpha(float a);
	void setFadeIn(bool fade);
	void setFadeOut(bool fade);

private:
	float m_alpha; //the transparency of our button
	bool m_fadeIn; //bool to hold wheter to fade in the button
	bool m_fadeOut; //bool to hold wheter to fade out the button
	bool m_selected; //bool to hold wheter a button is selected or not
	sf::Vector2f m_position; //position that we want to draw at
	sf::Sprite m_sprite; //our button sprite
	sf::Sprite m_icon; //our icon sprite
	std::string m_name; //the name of our button
	Label m_label;

	//Animation variables
	sf::Clock m_animationClock, m_iconAnimationClock; //our animation clock, we will use this to update our animations
	sf::IntRect m_selectedRect, m_iconSelectedRect; //the frame of our spritesheet we show when our button is selected
	sf::IntRect m_unselectedRect, m_iconUnselectedRect;//the frame of our spritesheet we show when our button is not selected
	thor::FrameAnimation m_selectAnimation, m_deselectAnimation, m_iconSelectAnimation, m_iconDeselectAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	thor::Animator<sf::Sprite, std::string> m_buttonAnimator;
	thor::Animator<sf::Sprite, std::string> m_iconAnimator;
};