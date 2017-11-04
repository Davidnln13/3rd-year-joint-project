#pragma once
#include "Box2D\Box2D.h"
#include <iostream>

//forward references for our player to avoid circular reference
class Player;
class Weapon;

/// <summary>
/// Author: Daryl keogh
/// Date: 04/11/2017
/// Description: The contact listener inherits from the Box2d class b2ContactListener, we can then override the BeginContact
/// and EndContact methods and check for collision between sensors
/// </summary>

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void setPlayers(Player& player1, Player& player2);
private:
	//we hold a pointer to our players so we can invoke methods if their sensor have collided/not collided
	Player* m_player1;
	Player* m_player2;
};


#include "Player.h"
#include "Weapon.h"