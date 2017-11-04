#include "ContactListener.h"

void ContactListener::BeginContact(b2Contact * contact)
{
	auto fixA= contact->GetFixtureA();
	auto fixB = contact->GetFixtureB();

	//if the first or second body belongs to the player1's jump sensor then set its can jump boolean
	if (fixA->GetBody()->GetUserData() == m_player1 && fixA->GetBody() == m_player1->getJumpSensor()
		|| fixB->GetBody()->GetUserData() == m_player1 && fixB->GetBody() == m_player1->getJumpSensor())
	{
		m_player1->setCanJump(true); //allow our user to jump
	}
	//else if it belongs to the second player then set their boolean for canJump
	if (fixA->GetBody()->GetUserData() == m_player2 && fixA->GetBody() == m_player2->getJumpSensor()
		|| fixB->GetBody()->GetUserData() == m_player2 && fixB->GetBody() == m_player2->getJumpSensor())
	{
		m_player2->setCanJump(true);
	}
}

void ContactListener::EndContact(b2Contact * contact)
{

}

void ContactListener::setPlayers(Player & player1, Player & player2)
{
	m_player1 = &player1;
	m_player2 = &player2;
}
