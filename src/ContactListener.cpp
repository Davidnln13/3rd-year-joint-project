#include "ContactListener.h"

ContactListener::ContactListener():
	m_spawnParticle(false)
{

}

void ContactListener::update()
{
	if (m_spawnParticle)
	{

		auto pos = m_p1Sword->getPosition();

		if(m_player1->facingLeft())
			pos.x -= 35 / 2.0f;
		else
			pos.x += 35 / 2.0f;
		

		//Add a new particle system to our vector
		m_particleSystems.push_back(std::unique_ptr<ParticleSystem>(new ParticleSystem(0, 35, pos)));

		m_spawnParticle = false;
	}

	//Update all of our particle systems
	for (auto& pSystem : m_particleSystems)
		pSystem->update();
}

void ContactListener::draw(sf::RenderWindow & window)
{

	//Draw all of our particle systems
	for (auto& pSystem : m_particleSystems)
		pSystem->draw(window);

}
void ContactListener::BeginContact(b2Contact * contact)
{
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	checkIfSwordHitWall(*m_player1, *fixA, *fixB);
	checkIfSwordHitWall(*m_player2, *fixA, *fixB);

	//invoking our canPlayerJump method to check if each player is not making contact with an invalid surface that they can jump on such as a sword
	if(canPlayerJump(m_player1, m_player2, *fixA, *fixB))
		m_player1->setCanJump(true);

	else if (canPlayerJump(m_player2, m_player1, *fixA, *fixB))
		m_player2->setCanJump(true);

	//checking if a sword has collided with a player, if so, reset the player
	else if (hasSwordHitPlayer(m_player1, m_player2, *fixA, *fixB)) //if a sword has hit the first player then respawn them
	{
		m_player1->setDead(true);
		m_player2->increaseKills();
	}

	else if (hasSwordHitPlayer(m_player2, m_player1, *fixA, *fixB)) //if a sword has hit the second player then respawn them
	{
		m_player2->setDead(true);
		m_player1->increaseKills();
	}


	//Checking if either player has picked up a weapon
	else if (checkForSwordPickUp(m_player1, *fixA, *fixB))
		m_player1->setPickupWeapon();
	else if (checkForSwordPickUp(m_player2, *fixA, *fixB))
		m_player2->setPickupWeapon();

	else
	{
		//Checking if two swords have collided
		haveTwoSwordsCollided(m_player1, m_player2, *fixA, *fixB);
		haveTwoSwordsCollided(m_player2, m_player1, *fixA, *fixB);

		//Checks if a player is moving into a wall, if so set the players can move boolean to false
		checkCanPlayerMove(m_player1, *fixA, *fixB, false);
		checkCanPlayerMove(m_player2, *fixA, *fixB, false);
	}

	if (m_spawnParticle)
	{
		b2WorldManifold worldManifold;
		contact->GetWorldManifold(&worldManifold);

		m_partcleSpawnPos = sf::Vector2f(worldManifold.points[0].x, worldManifold.points[0].y);
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	//Kill our players if they leave the kill box
	if (payerLeftKillBox(m_player1, *fixA, *fixB))
		m_player1->setDead(true);
	if (payerLeftKillBox(m_player2, *fixA, *fixB))
		m_player2->setDead(true);


	//invoking our canPlayerJump method to check if each player is not making contact with an invalid surface that they can jump on such as a sword
	if (canPlayerJump(m_player1, m_player2, *fixA, *fixB))
		m_player1->setCanJump(false);
	else if (canPlayerJump(m_player2, m_player1, *fixA, *fixB))
		m_player2->setCanJump(false);

	//checking if two swords have stopped colliding
	else if (fixA->GetBody() == m_player1->getSwordBody() && fixB->GetBody() == m_player2->getSwordBody()
		|| fixB->GetBody() == m_player1->getSwordBody() && fixA->GetBody() == m_player2->getSwordBody()
		|| fixA->GetBody() == m_player2->getSwordBody() && fixB->GetBody() == m_player1->getSwordBody()
		|| fixB->GetBody() == m_player2->getSwordBody() && fixA->GetBody() == m_player1->getSwordBody())
	{
		m_player1->setClashed(false);
		m_player2->setClashed(false);
	}
	else
	{
		//Checks if a player is moving into a wall, if so set the players can move boolean to true
		checkCanPlayerMove(m_player1, *fixA, *fixB, true);
		checkCanPlayerMove(m_player2, *fixA, *fixB, true);
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
		player1->playAudio();
		if (player1->holdingSword() == false) //so if we are not holding a sword
			player1->setSwordThrown();
		if(player2->holdingSword() == false) //if the the other player is also not holding a sword
			player2->setSwordThrown();

		else if (player1->switchedWeaponPos())
		{
			player2->setParried(true);
		}
		else
		{
			player1->setClashed(true);
		}
		m_spawnParticle = true;
	}
}

void ContactListener::checkCanPlayerMove(Player * player, b2Fixture & fixA, b2Fixture & fixB, bool canMove)
{
	if (fixA.GetBody() == player->getLeftSensorBody() && fixB.GetBody()->GetUserData() == "Boundary"
		|| fixB.GetBody() == player->getLeftSensorBody() && fixA.GetBody()->GetUserData() == "Boundary")
	{
		player->setCanMoveLeft(canMove);
	}
	else if (fixA.GetBody() == player->getRightSensorBody() && fixB.GetBody()->GetUserData() == "Boundary"
		|| fixB.GetBody() == player->getRightSensorBody() && fixA.GetBody()->GetUserData() == "Boundary")
	{
		player->setCanMoveRight(canMove);
	}
}

bool ContactListener::payerLeftKillBox(Player * player, b2Fixture & fixA, b2Fixture & fixB)
{
	if (fixA.GetBody()->GetUserData() == "Kill Box" && fixB.GetBody() == player->getPlayerBody())
	{
		return true;
	}
	else if (fixB.GetBody()->GetUserData() == "Kill Box" && fixA.GetBody() == player->getPlayerBody())
	{
		return true;
	}

	return false;
}

void ContactListener::checkIfSwordHitWall(Player& player, b2Fixture & fixA, b2Fixture & fixB)
{
	//If the sword has hit a wall
	if (fixA.GetBody() == player.getSwordBody() && fixB.GetBody()->GetUserData() == "Wall"
		|| fixB.GetBody() == player.getSwordBody() && fixA.GetBody()->GetUserData() == "Wall")
	{
		//if the player is not holding the sword
		if(player.holdingSword() == false)
			player.setSwordThrown(); //Invoke set thrown so gravity and physics apply to the sword again
	}
}

void ContactListener::setPlayers(Player & player1, Player & player2)
{
	m_player1 = &player1;
	m_player2 = &player2;
	m_p1Sword = player1.getWeapon();
	m_p2Sword = player2.getWeapon();
}
