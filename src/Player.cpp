#include "Player.h"

Player::Player() :
	m_playerRect(sf::Vector2f(15, 15))
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((1280 / 2.f) / PPM, (720 / 2.f) / PPM); //spawn the player in the center of the screen
	m_playerRect.setPosition(1280 / 2.f, 720 / 2.f); //testing drawing a rectangle for the player
	bodyDef.angle = 0;
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((15 / 2.f) / PPM, (15 / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.shape = &boxShape;
	boxFixDef.density = 1; //giving the player a mass of 1
	m_body->CreateFixture(&boxFixDef);

	m_playerRect.setOrigin(15.f / 2, 15.f / 2); //setting the origin to the center of the box
}

void Player::update()
{
}

void Player::render(sf::RenderWindow & window)
{
	m_playerRect.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_playerRect.setRotation(m_body->GetAngle());
	window.draw(m_playerRect);
}
