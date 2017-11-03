#pragma once
#include "Weapon.h"

class JoystickController;

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
	void handleJoystick(JoystickController& controller);
	void createPlayerJoint(bool facingLeft);

private:
	bool m_isFacingLeft;
	bool m_canAttack;
	float m_speed;

	b2Body* m_playerBody;
	b2Body* m_shoulderBody;
	b2Body* m_forearmBody;
	sf::RectangleShape m_playerRect;
	sf::RectangleShape m_forearmRect;
	Weapon m_sword;
	b2PrismaticJoint* m_playerToArmJoint; //the joint between our the players body and the players arm
	b2RevoluteJoint* m_handWeaponJoint;
};

#include "JoystickController.h"