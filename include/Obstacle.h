#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"

//forward references so we can reference the box2d world and Pixel Per Metre variable
extern b2World world; //defining the box2d world object, saves us from passing it over to every objetc that needs it
extern float PPM;
extern ResourceManager resourceManager;

class Obstacle
{
public:
	Obstacle(sf::Vector2f position, sf::Vector2f size);

private:
	b2Body* m_body;
};