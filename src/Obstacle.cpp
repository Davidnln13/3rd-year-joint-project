#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size)
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x / PPM, position.y / PPM); //spawn the box at this position
	m_body = world.CreateBody(&bodyDef); //add the body to the world
	m_body->SetUserData("Boundary");

	b2PolygonShape boxShape;
	boxShape.SetAsBox((size.x / 2.f) / PPM, (size.y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.density = 1; 
	boxFixDef.friction = .7f;
	boxFixDef.restitution = 0.39f;
	boxFixDef.shape = &boxShape;
	m_body->CreateFixture(&boxFixDef);

}