#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size, std::string userData)
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x / PPM, position.y / PPM); //spawn the box at this position
	m_body = world.CreateBody(&bodyDef); //add the body to the world
	if(userData == "")
		m_body->SetUserData("Boundary");
	else
		m_body->SetUserData(&userData);

	b2PolygonShape boxShape;
	boxShape.SetAsBox((size.x / 2.f) / PPM, (size.y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.density = 100; 
	boxFixDef.friction = .25f;
	boxFixDef.restitution = 0.0f;
	boxFixDef.shape = &boxShape;
	m_body->CreateFixture(&boxFixDef);

}

b2Body* Obstacle::body()
{
	return m_body;
}
