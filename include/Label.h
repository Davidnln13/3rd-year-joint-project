#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"

extern ResourceManager resourceManager;

class Label
{
public:
	Label(std::string textString, sf::Vector2f& position);

	void draw(sf::RenderWindow& window);

private:
	sf::Text m_text;
	sf::Vector2f m_position;
};