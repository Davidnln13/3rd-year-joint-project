#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include <Thor\Math.hpp>
#include <iostream>

//forward references, does not work with static variables, these variables are located in Game.cpp
extern b2World world; //defining the box2d world object, saves us from passing it over to every object that needs it
extern float PPM;

class Weapon
{
public:

	Weapon();
	~Weapon();

	void update();
	void render(sf::RenderWindow& window);
	void attack();

	//getters
	b2Body* getBody();

private:
	b2Body* m_body;
	sf::RectangleShape m_rect;
};