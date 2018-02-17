#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"

extern b2World world; //defining the box2d world object, saves us from passing it over to every object that needs it
extern float PPM;
extern ResourceManager resourceManager;

class Flag
{

public:
	Flag(sf::Texture& texture);

	void draw(sf::RenderWindow& window);

	//Getters
	sf::RectangleShape& hitBox();

	//Setters
	void setPosition(float x, float y, float xScale);
	void setPickedUp(bool pickedUp);

private:
	b2Body* m_body;

	sf::RectangleShape m_hitBox;
	sf::Sprite m_sprite;
};