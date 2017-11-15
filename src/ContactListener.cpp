#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact * contact)
{
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	//invoking our canPlayerJump method to check if each player is not making contact with an invalid surface that they can jump on such as a sword
	if(canPlayerJump(m_player1, m_player2, *fixA, *fixB))
		m_player1->setCanJump(true);

	if (canPlayerJump(m_player2, m_player1, *fixA, *fixB))
		m_player2->setCanJump(true);

	//Checking if two swords have collided
	haveTwoSwordsCollided(m_player1, m_player2, *fixA, *fixB);
	haveTwoSwordsCollided(m_player2, m_player1, *fixA, *fixB);

	//checking if a sword has collided with a player, if so, reset the player
	if (hasSwordHitPlayer(m_player1, m_player2, *fixA, *fixB)) //if a sword has hit the first player then respawn them
		m_player1->setRespawn(true);
	if (hasSwordHitPlayer(m_player2, m_player1, *fixA, *fixB)) //if a sword has hit the second player then respawn them
		m_player2->setRespawn(true);

	//Checking if either player has picked up a weapon
	if (checkForSwordPickUp(m_player1, *fixA, *fixB))
		m_player1->setPickupWeapon();
	if (checkForSwordPickUp(m_player2, *fixA, *fixB))
		m_player2->setPickupWeapon();
}

void ContactListener::EndContact(b2Contact * contact)
{
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	//invoking our canPlayerJump method to check if each player is not making contact with an invalid surface that they can jump on such as a sword
	if (canPlayerJump(m_player1, m_player2, *fixA, *fixB))
		m_player1->setCanJump(false);
	if (canPlayerJump(m_player2, m_player1, *fixA, *fixB))
		m_player2->setCanJump(false);

	//checking if two swords have stopped colliding
	if (fixA->GetBody() == m_player1->getSwordBody() && fixB->GetBody() == m_player2->getSwordBody()
		|| fixB->GetBody() == m_player1->getSwordBody() && fixA->GetBody() == m_player2->getSwordBody()
		|| fixA->GetBody() == m_player2->getSwordBody() && fixB->GetBody() == m_player1->getSwordBody()
		|| fixB->GetBody() == m_player2->getSwordBody() && fixA->GetBody() == m_player1->getSwordBody())
	{
		m_player1->setClashed(false);
		m_player2->setClashed(false);
	}
}

bool ContactListener::canPlayerJump(Player* player1, Player* player2, b2Fixture& fixA, b2Fixture& fixB)
{
	//if the first player passed to this method collides with a body that is not the other players arm/sword then we can jump
	if (fixA.GetBody() == player1->getJumpBody() && fixB.GetBody()->GetUserData() == "Boundary"
		|| fixB.GetBody() == player1->getJumpBody() && fixA.GetBody()->GetUserData() == "Boundary")
	{
		return true;
	}

	return false;
}

bool ContactListener::hasSwordHitPlayer(Player * player1, Player * player2, b2Fixture & fixA, b2Fixture & fixB)
{
	if (fixA.GetBody() == player1->getPlayerBody() && fixB.GetBody() == player2->getSwordBody() && player2->getSwordBody()->GetFixtureList()->IsSensor()
		|| fixB.GetBody() == player1->getPlayerBody() && fixA.GetBody() == player2->getSwordBody() && player2->getSwordBody()->GetFixtureList()->IsSensor())
	{
		if (player2->holdingSword() == false) //so if the second player threw the sword then stop the other player's sword
			player2->setSwordThrown();

		return true;
	}

	return false;
}

bool ContactListener::checkForSwordPickUp(Player * player, b2Fixture & fixA, b2Fixture & fixB)
{
	if (fixA.GetBody() == player->getSwordBody() && fixB.GetBody() == player->getPlayerBody() && player->holdingSword() == false
		|| fixB.GetBody() == player->getSwordBody() && fixA.GetBody() == player->getPlayerBody() && player->holdingSword() == false)
	{
		return true;
	}

	return false;
}

void ContactListener::haveTwoSwordsCollided(Player * player1, Player* player2, b2Fixture & fixA, b2Fixture & fixB)
{
	//If the Fixture is a sword and it has collided with the other players sword
	if (fixA.GetBody() == player1->getSwordBody() && fixB.GetBody() == m_player2->getSwordBody()
		|| fixB.GetBody() == player1->getSwordBody() && fixA.GetBody() == m_player2->getSwordBody())
	{ 
		if (player1->holdingSword() == false) //so if we are not holding a sword
			player1->setSwordThrown();
		else if (player1->switchedWeaponPos())
			player2->setParried(true);
		else
			player1->setClashed(true);
	}
}

void ContactListener::setPlayers(Player & player1, Player & player2)
{
	m_player1 = &player1;
	m_player2 = &player2;
}
