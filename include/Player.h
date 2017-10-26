#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include <iostream>

//forwards references, does nto work with static variables, these variable sare located in Game.cpp
extern b2World world; //defining the box2d world object, saves us from passing it over to every objetc that needs it
extern float PPM;

class Player
{
public:
	Player();

	void update();
	void render(sf::RenderWindow& window);


private:
	b2Body* m_body;
	sf::RectangleShape m_playerRect;
};