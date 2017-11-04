#include "Weapon.h"

Weapon::Weapon() :
	m_rect(sf::Vector2f(35, 5))
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((1280 / 2.f) / PPM, (720 / 2.f) / PPM);
	bodyDef.fixedRotation = true;
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((m_rect.getSize().x / 2.f) / PPM, (m_rect.getSize().y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.shape = &boxShape;
	boxFixDef.density = .2; //giving the sword a mass of .2
	boxFixDef.isSensor = true;
	m_body->CreateFixture(&boxFixDef);

	m_rect.setOrigin(m_rect.getSize().x / 2, m_rect.getSize().y / 2); //setting the origin to the center of the box
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Blue);
	m_rect.setOutlineThickness(1);
}

Weapon::~Weapon()
{
	world.DestroyBody(m_body);
}

void Weapon::update()
{
}

void Weapon::render(sf::RenderWindow & window)
{
	m_rect.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_rect.setRotation(m_body->GetAngle() * (180.f / thor::Pi)); //have to convert from radians to degrees here
	window.draw(m_rect);
}

void Weapon::attack(b2Vec2 force)
{

}

b2Body* Weapon::getBody()
{
	return m_body;
}
