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

	if (fixA->GetBody() == m_player1->getSwordBody() && fixB->GetBody() == m_player2->getSwordBody()
		|| fixB->GetBody() == m_player1->getSwordBody() && fixA->GetBody() == m_player2->getSwordBody())
	{
		m_player1->swordClashed();
		m_player2->swordClashed();
	}
}

void ContactListener::EndContact(b2Contact * contact)
{
	auto fixA = contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	if (fixA->GetBody() == m_player1->getJumpBody() || fixB->GetBody() == m_player1->getJumpBody())
	{
		m_player1->setCanJump(false);
	}
	if (fixA->GetBody() == m_player2->getJumpBody() || fixB->GetBody() == m_player2->getJumpBody())
	{
		m_player2->setCanJump(false);
	}
}

bool ContactListener::canPlayerJump(Player* player1, Player* player2, b2Fixture& fixA, b2Fixture& fixB)
{
	//if the first player passed to this method collides with a body that is not the other players arm/sword then we can jump
	if (fixA.GetBody() == player1->getJumpBody() && fixB.GetBody() != player2->getArmBody() && fixB.GetBody() != player2->getSwordBody()
		|| fixB.GetBody() == player1->getJumpBody() && fixA.GetBody() != player2->getArmBody() && fixA.GetBody() != player2->getSwordBody())
	{
		return true;
	}

	return false;
}

void ContactListener::setPlayers(Player & player1, Player & player2)
{
	m_player1 = &player1;
	m_player2 = &player2;
}
