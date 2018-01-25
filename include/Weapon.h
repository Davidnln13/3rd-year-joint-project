#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include <Thor\Math.hpp>
#include <iostream>
#include "ResourceManager.h"
#include "Thor\Animations.hpp"

//forward references, does not work with static variables, these variables are located in Game.cpp
extern b2World world; //defining the box2d world object, saves us from passing it over to every object that needs it
extern float PPM;
extern ResourceManager resourceManager;

class Weapon
{
public:

	Weapon(sf::Vector2f position);
	~Weapon();

	void update();
	void render(sf::RenderWindow& window);
	void respawn();
	void throwWeapon(std::string direction);
	void applyForce(b2Vec2 force);
	void pickUp(); //picks up the weapon
	void parried();
	void negateSword();

	//getters
	b2Body* getBody();
	sf::Sprite& getLight();
	sf::Vector2f getPosition();

	//Setters
	void setPosition(b2Vec2 pos, float angle);
	void setSwordThrown();
	void setSwordDirection(std::string direction);
private:
	bool m_swordThrown;
	bool m_setSensor;
	std::string m_throwDirection; //the direction our sword was thrown in
	b2Body* m_body;
	b2Body* m_pivotBody;
	b2FixtureDef m_bodyFixt;
	sf::Sprite m_sprite;
	sf::RectangleShape m_rect;
	b2RevoluteJoint* m_pivotJoint;
	sf::Sprite m_lightSprite;
};