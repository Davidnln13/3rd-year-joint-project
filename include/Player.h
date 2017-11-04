#pragma once
#include "Weapon.h"

class JoystickController; //forward reference for our joystick class

class Player
{
public:
	Player(sf::Vector2f position, sf::Vector2f size);
	~Player(); //Any classes that have a box2d body should have that body deleted in here

	void update();
	void render(sf::RenderWindow& window);
	void moveRight();
	void moveLeft();
	void attack();
	void handleJoystick(JoystickController& controller);
	void invertPlayerJoint(bool facingLeft); //inverts the joints for our player, so we can swap where the weapon and arm is jointed to

private:
	bool m_isFacingLeft;
	bool m_canAttack;
	float m_speed;

	sf::Vector2f m_position;

	//Our rectangles to draw our player and forearm
	sf::RectangleShape m_playerRect;
	sf::RectangleShape m_forearmRect;

	//Our weapon variable
	Weapon m_sword;

	//Box2D Variables
	b2Body* m_playerBody;
	b2Body* m_shoulderBody;
	b2Body* m_forearmBody;
	b2PrismaticJoint* m_playerToArmJoint; //the joint between our the players body and the players arm
	b2RevoluteJoint* m_armToSwordJoint; //the joitnt bewteen our players arm and the sword

	//We hold the definitions as variables so we can easily change joint definition variables on the fly without recreating them each time
	b2PrismaticJointDef m_playerToArmJointDef;
	b2RevoluteJointDef m_armToSwordJointDef;
};

#include "JoystickController.h"