#include "Boundary.h"

Boundary::Boundary(sf::Vector2f size, sf::Vector2f position):
	m_rect(size)
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(position.x / PPM, position.y / PPM); //spawn the box at this position
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((size.x / 2.f) / PPM, (size.y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.density = 1; 
	boxFixDef.friction = .7f;
	boxFixDef.restitution = 0.5f;
	boxFixDef.shape = &boxShape;
	m_body->CreateFixture(&boxFixDef);

	m_rect.setOrigin(size.x / 2.f, size.y / 2.f); //setting the origin to the center of the floor
	m_rect.setFillColor(sf::Color::Black);
}

void Boundary::update()
{
}

void Boundary::render(sf::RenderWindow & window)
{
	m_rect.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_rect.setRotation(m_body->GetAngle());
	window.draw(m_rect);
}
