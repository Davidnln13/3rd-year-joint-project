#pragma once
#include "SFML\Graphics.hpp"
#include "Label.h"

class Slider
{
public:
	Slider(std::string name, sf::Vector2f pos, sf::IntRect rect);

	void update();
	void draw(sf::RenderWindow& window);
	//setter
	void setSliderLevel(int level);
	void setTexture(sf::Texture texture);
	void setRect(sf::IntRect rect);
	void moveDown();
	void moveUp();
	//getter
	int getSliderLevel();
	std::string& getName();

private:
	sf::Vector2f m_position; //the position of our slider
	sf::Sprite m_sprite;
	sf::IntRect m_rect;
	std::string m_name;
	int m_currentVal;
	const float MAX_VALUE = 100;
	const float MIN_VALUE = 0;
	
};