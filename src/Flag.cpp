#include "Flag.h"

Flag::Flag(sf::Texture& texture) :
	m_hitBox(sf::Vector2f(25, 60))
{
	m_startPos = sf::Vector2f(-590, 650);


	//Set the properties of our hitbox
	m_hitBox.setOutlineColor(sf::Color(244, 241, 66, 255));
	m_hitBox.setOutlineThickness(2);
	m_hitBox.setFillColor(sf::Color::Transparent);
	m_hitBox.setOrigin(5, 30);
	m_hitBox.setPosition(100, 100);

	//Set the properties of our sprite
	m_sprite.setTexture(texture);
	m_sprite.setOrigin(2.5, 30);
	m_sprite.setPosition(100, 100);

	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(100 / PPM, 100 / PPM); //spawn the box at this position
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((m_hitBox.getSize().x / 2.f) / PPM, (m_hitBox.getSize().y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.density = 0;
	boxFixDef.friction = 0.0f;
	boxFixDef.restitution = 0.0f;
	//We set our mask bits to 0x0001 because this bit category is not the category the player is in (0x0002),
	//Therefore we avoid collisions with the player and only collided with things in the 0x0001 category
	boxFixDef.filter.maskBits = 0x0001;
	boxFixDef.shape = &boxShape;
	m_body->CreateFixture(&boxFixDef);
}

void Flag::draw(sf::RenderWindow & window)
{
	m_hitBox.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_sprite.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	//window.draw(m_hitBox); //Draw our hitbox
	window.draw(m_sprite); //Draw our sprite
}

void Flag::reset()
{
	m_hitBox.setScale(1, 1);
	m_hitBox.setPosition(m_startPos.x, m_startPos.y);
	m_sprite.setScale(1, 1);
	m_sprite.setPosition(m_startPos.x, m_startPos.y);
	m_body->SetTransform(b2Vec2(m_startPos.x / PPM, m_startPos.y / PPM), 0);
	m_body->SetAngularVelocity(0);
	m_body->SetLinearVelocity(b2Vec2(0, 0));
	m_body->GetFixtureList()->SetSensor(false); //Set our body as a sensor so no weird collision appears within 
}

sf::RectangleShape & Flag::hitBox()
{
	return m_hitBox;
}

void Flag::setPosition(float x, float y, float xScale)
{
	if (xScale == -1)
	{
		m_hitBox.setScale(-1, 1);
		m_hitBox.setPosition(x, y);
		m_sprite.setScale(-1, 1);
		m_sprite.setPosition(x, y);
		m_body->SetTransform(b2Vec2(x / PPM, y / PPM), 0);
	}
	else
	{
		m_hitBox.setScale(1, 1);
		m_hitBox.setPosition(x , y);
		m_sprite.setScale(1, 1);
		m_sprite.setPosition(x, y);
		m_body->SetTransform(b2Vec2(x / PPM, y / PPM), 0);
	}

}

void Flag::setPickedUp(bool pickedUp)
{
	if (pickedUp)
	{
		m_body->GetFixtureList()->SetSensor(true); //Set our body as a sensor so no weird collision appears within 
	}
	else
	{
		m_body->SetAngularVelocity(0);
		m_body->SetLinearVelocity(b2Vec2(0, 0));
		m_body->GetFixtureList()->SetSensor(false); //Set our body as a sensor so no weird collision appears within 
	}
}

void Flag::startPos(float x, float y)
{
	m_startPos = sf::Vector2f(x, y); //Set our start pos
	m_body->SetTransform(b2Vec2(x / PPM, y / PPM), 0); //Set our body's position
	m_body->GetFixtureList()->SetSensor(false); //Set our body as a sensor so no weird collision appears within 
}
