#include "Weapon.h"

Weapon::Weapon(sf::Vector2f position) :
	m_rect(sf::Vector2f(35, 5)),
	m_swordThrown(false),
	m_destroyPivot(false)
{
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
	m_bodyFixt.density = .2; //giving the sword a mass of .2
	m_bodyFixt.isSensor = true;
	m_body->CreateFixture(&m_bodyFixt);
	m_body->SetUserData(this);

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
	if (m_destroyPivot && m_pivotJoint != nullptr)
	{
		world.DestroyJoint(m_pivotJoint);
		m_pivotJoint = nullptr;
		m_destroyPivot = false;
	}

	if (m_swordThrown)
	{
		if (m_throwDirection == "Left")
			m_pivotBody->SetLinearVelocity(b2Vec2(-24, 0));
		else
			m_pivotBody->SetLinearVelocity(b2Vec2(24, 0));
	}
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

void Weapon::throwWeapon(std::string direction)
{
	m_throwDirection = direction;
	m_swordThrown = true;

	m_body->SetGravityScale(0); //set gravity scale to 0 on our sword

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.gravityScale = 0;
	bodyDef.position.Set(m_body->GetPosition().x, m_body->GetPosition().y);
	m_pivotBody = world.CreateBody(&bodyDef);

	b2CircleShape cs;
	cs.m_radius = 5 / 2 / PPM;

	b2FixtureDef pivotFixDef;
	pivotFixDef.shape = &cs;
	pivotFixDef.density = 1;
	pivotFixDef.isSensor = true;
	m_pivotBody->CreateFixture(&pivotFixDef);
	m_pivotBody->SetUserData(this);

	//Creating the revolte joint between our sword and our pivot
	b2RevoluteJointDef pivot;
	pivot.bodyA = m_body;
	pivot.bodyB = m_pivotBody;
	pivot.collideConnected = false; //so our sword and pivot dont collide
	pivot.localAnchorA.Set(0, 0);
	pivot.localAnchorB.Set(0, 0);
	m_pivotJoint = (b2RevoluteJoint*)world.CreateJoint(&pivot);

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
	m_body->SetGravityScale(0);
}

b2Body* Weapon::getBody()
{
	return m_body;
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
	m_body->SetFixedRotation(false);
	m_body->GetFixtureList()->SetSensor(false);
	if (m_throwDirection == "Left")
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(.55, 0), true);
		m_body->ApplyTorque(1.5, true);
	}
	else
	{
		m_body->ApplyLinearImpulseToCenter(b2Vec2(-.55, 0), true);
		m_body->ApplyTorque(-1.5, true);
	}
	m_body->SetGravityScale(1);
	m_destroyPivot = true;
}
