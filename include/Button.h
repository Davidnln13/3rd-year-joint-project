#pragma once
#include "Label.h"
#include "Thor\Animations.hpp"

extern ResourceManager resourceManager;

class Button
{
public:
	Button(sf::Vector2f position, std::string name);
	void update();
	void render(sf::RenderWindow& window);
	void select();
	void deSelect();

	void setUpAnimation(); //sets up our animations

	//Getters
	std::string& getName();

private:
	sf::Vector2f m_position; //a reference to our position that we want to draw at
	sf::Sprite m_sprite; //our button sprite
	std::string m_name; //the name of our button
	Label m_label;

	//Animation variables
	sf::Clock m_animationClock; //our animation clock, we will use this to update our animations
	sf::IntRect m_selectedRect; //the frame of our spritesheet we show when our button is selected
	sf::IntRect m_unselectedRect;//the frame of our spritesheet we show when our button is not selected
	sf::IntRect m_currentRect; //the current rectangle of the spritesheet we want to show
	thor::FrameAnimation m_selectAnimation, m_deselectAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	thor::Animator<sf::Sprite, std::string> m_animator;
};