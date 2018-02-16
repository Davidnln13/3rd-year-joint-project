#pragma once
#include "SFML\Graphics.hpp"

class Slider
{
public:
	Slider(std::string name, sf::Vector2f pos);

	void update();
	void draw(sf::RenderWindow& window);
	//setter
	void setSliderLevel(int level);
	//getter
	int getSliderLevel();

private:
	sf::Vector2f m_position; //the position of our slider
	int m_currentVal;
	const float MAX_VALUE = 100;
	const float MIN_VALUE = 0;
	
};