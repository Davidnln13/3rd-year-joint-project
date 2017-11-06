#pragma once
#include "Label.h"

extern ResourceManager resourceManager;

class Button
{
public:
	Button(sf::Vector2f position, std::string name);

	void render(sf::RenderWindow& window);

	//Getters
	std::string& getName();

private:
	sf::Vector2f m_position; //a reference to our position that we want to draw at
	sf::Sprite m_sprite; //our button sprite
	std::string m_name; //the name of our button
};