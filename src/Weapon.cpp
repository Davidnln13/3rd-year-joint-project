#include "Weapon.h"

Weapon::Weapon(sf::Vector2f position) :
	m_rect(sf::Vector2f(35, 5)),
	m_swordThrown(false),
	m_setSensor(false)
{
	//Load our shader
	m_recolourShader.loadFromFile(resourceManager.getShaderHolder()["recolourShader"], sf::Shader::Fragment);

	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x / PPM, position.y / PPM);
	bodyDef.fixedRotation = false;
	bodyDef.bullet = true; //we set the weapon as a bullet so collision detection for the weapon updates more frequently so we get smoother collisions
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((m_rect.getSize().x / 2.f) / PPM, (m_rect.getSize().y / 2.f) / PPM);

	m_bodyFixt.shape = &boxShape;
	m_bodyFixt.density = .2f; //giving the sword a mass of .2
	m_bodyFixt.isSensor = true;
	m_bodyFixt.filter.categoryBits = 0x0002;
	m_body->CreateFixture(&m_bodyFixt);
	m_body->SetUserData(this);

	m_rect.setOrigin(m_rect.getSize().x / 2, m_rect.getSize().y / 2); //setting the origin to the center of the box
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::White);
	m_rect.setOutlineThickness(1);

	m_sprite.setTexture(resourceManager.getTextureHolder()["sword"]);
	m_sprite.setOrigin(m_sprite.getLocalBounds().left + (m_sprite.getLocalBounds().width / 2.0f) + 7.5f, m_sprite.getLocalBounds().top + m_sprite.getLocalBounds().height / 2.0f);

	m_lightSprite.setTexture(resourceManager.getTextureHolder()["swordLight"]);
	m_lightSprite.setOrigin(m_lightSprite.getLocalBounds().left + m_lightSprite.getLocalBounds().width / 2, m_lightSprite.getLocalBounds().top + m_lightSprite.getLocalBounds().height / 2);
}

Weapon::~Weapon()
{
	world.DestroyBody(m_body);
}

void Weapon::update()
{
	//If our bool is true, set our wepaon sesnor to false
	if (m_setSensor)
	{
		m_body->GetFixtureList()->SetSensor(false);
		m_setSensor = false;
	}

	//If we have thrown our sword then move our sword in the direction it was thrown
	if (m_swordThrown)
	{
		if (m_throwDirection == "Left")
			m_body->SetLinearVelocity(b2Vec2(-24, 0));
		else
			m_body->SetLinearVelocity(b2Vec2(24, 0));
	}
}

void Weapon::render(sf::RenderWindow & window)
{
	//Drawing our light for our sword
	m_lightSprite.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_lightSprite.setRotation(m_body->GetAngle() * (180.f / thor::Pi));

	m_rect.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_rect.setRotation(m_body->GetAngle() * (180.f / thor::Pi)); //have to convert from radians to degrees here

	m_sprite.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_sprite.setRotation(m_body->GetAngle() * (180.f / thor::Pi));

	window.draw(m_sprite, &m_recolourShader);
	//window.draw(m_rect);
}

void Weapon::respawn()
{
	//if our pivot joint does not equal nullptr
	if (m_pivotJoint != nullptr)
	{
		//then destroy the joint and set our pointer to nullptr
		world.DestroyJoint(m_pivotJoint);
		m_pivotJoint = nullptr;
	}

	//reset the body/weapon
	m_body->SetTransform(m_body->GetPosition(), 0);
	m_body->GetFixtureList()->SetSensor(true);
	m_body->SetLinearVelocity(b2Vec2(0, 0));
	m_body->SetAngularVelocity(0);
	m_body->ApplyTorque(0, true);

	m_swordThrown = false;
	m_setSensor = false;
}

void Weapon::throwWeapon(std::string direction)
{
	m_throwDirection = direction;
	m_swordThrown = true;

	m_body->SetGravityScale(0); //set gravity scale to 0 on our sword
	m_body->SetLinearVelocity(b2Vec2(0,0)); //reset the velocity of our sword
	m_body->SetAngularVelocity(0); //reset the angular velocity

	//Apply a torque to our body so it spins
	if (direction == "Left")
		m_body->ApplyTorque(-5, true);
	else
		m_body->ApplyTorque(5, true);
}

void Weapon::applyForce(b2Vec2 force)
{
	m_body->ApplyForceToCenter(force, true);
}

void Weapon::pickUp()
{
	m_body->SetTransform(m_body->GetPosition(), 0);
	m_body->GetFixtureList()->SetSensor(true);
	m_body->SetGravityScale(1);
	m_body->ApplyTorque(0, true);
}

void Weapon::parried()
{
	m_body->SetAngularVelocity(0);
	m_body->SetLinearVelocity(b2Vec2(0,0));
	m_body->GetFixtureList()->SetSensor(false);
	m_body->SetGravityScale(1);
	m_body->ApplyLinearImpulseToCenter(b2Vec2(2, 0), true);
	m_body->ApplyTorque(-5, true);
}

b2Body* Weapon::getBody()
{
	return m_body;
}

sf::Sprite & Weapon::getLight()
{
	return m_lightSprite;
}

sf::Vector2f Weapon::getPosition()
{
	return sf::Vector2f(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
}

void Weapon::setPosition(b2Vec2 pos, float angle)
{
	m_body->SetTransform(pos, angle * (180.f / thor::Pi));
}

void Weapon::setSwordThrown()
{
	m_swordThrown = false;

	m_body->SetLinearVelocity(b2Vec2(0, m_body->GetLinearVelocity().y));
	m_body->SetAngularVelocity(0);

	if (m_throwDirection == "Left")
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(.225f, 0), true);
		m_body->ApplyTorque(1.5, true);
	}
	else
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(-.225f, 0), true);
		m_body->ApplyTorque(-1.5, true);
	}
	m_body->SetGravityScale(1);
	m_setSensor = true;
}

void Weapon::setSwordDirection(std::string direction)
{
	if (direction == "Right")
		m_sprite.setScale(1,1);
	else if(direction == "Left")
		m_sprite.setScale(-1, 1);
}

void Weapon::negateSword()
{
		m_sprite.setScale(m_sprite.getScale().x * -1,1);
}

void Weapon::setColor(sf::Color color)
{
	//Calculate our new colours, needs to be a number between 0 and 1 so we divide each paractmeter by 255
	float newR = color.r / 255.0f;
	float newG = color.g / 255.0f;
	float newB = color.b / 255.0f;
	float newA = color.a / 255.0f;

	//Set our shade runiform variables to our new calculated colour
	m_recolourShader.setUniform("newR", newR);
	m_recolourShader.setUniform("newG", newG);
	m_recolourShader.setUniform("newB", newB);
	m_recolourShader.setUniform("newA", newA);
}
