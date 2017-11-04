#pragma once
#include "Box2D\Box2D.h"
#include <iostream>

class Player;
class Weapon;

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
	void setPlayers(Player& player1, Player& player2);
private:
	Player* m_player1;
	Player* m_player2;
};


#include "Player.h"
#include "Weapon.h"