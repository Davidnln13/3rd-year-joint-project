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
	b2Body* m_playerBody;
	b2Body* m_shoulderBody;
	b2Body* m_forearmBody;
	sf::RectangleShape m_playerRect;
	sf::RectangleShape m_shoulderRect;
	sf::RectangleShape m_forearmRect;
	Weapon m_sword;
	b2RevoluteJoint* m_playerBodyToShoulderJoint; //the joint between our the players body and the players shoulder
	b2RevoluteJoint* m_shoulderToForearmJoint; //the elbow joint
	b2RevoluteJoint* m_handWeaponJoint;
};