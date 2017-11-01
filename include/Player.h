#pragma once
#include "Weapon.h"

class Player
{
public:
	Player(sf::Vector2f size);
	~Player(); //Any classes that have a box2d body should have that body deleted in here

	void update();
	void render(sf::RenderWindow& window);
	void moveRight();
	void moveLeft();
	void attack();

private:
	b2Body* m_body;
	sf::RectangleShape m_rect;
	Weapon m_sword;
	b2RevoluteJoint* m_joint; //the joint between our sword and the player
};