#include "Player.h"

Player::Player(sf::Vector2f size = sf::Vector2f(15,15)) :
	m_rect(size),
	m_sword()
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((1280 / 2.f) / PPM, (720 / 2.f) / PPM); //spawn the player in the center of the screen
	bodyDef.fixedRotation = true;
	m_body = world.CreateBody(&bodyDef); //add the body to the world

	b2PolygonShape boxShape;
	boxShape.SetAsBox((m_rect.getSize().x / 2.f) / PPM, (m_rect.getSize().y / 2.f) / PPM);

	b2FixtureDef boxFixDef;
	boxFixDef.shape = &boxShape;
	boxFixDef.density = 1; //giving the player a mass of 1
	m_body->CreateFixture(&boxFixDef);

	m_rect.setOrigin(m_rect.getSize().x / 2, m_rect.getSize().y / 2); //setting the origin to the center of the box
	m_rect.setFillColor(sf::Color::Transparent);
	m_rect.setOutlineColor(sf::Color::Red);
	m_rect.setOutlineThickness(1);


	//Creating the joint between our sword and player
	b2RevoluteJointDef rjd;
	rjd.bodyA = m_body;
	rjd.bodyB = m_sword.getBody();
	rjd.collideConnected = false; //so our sword and player dont collide
	rjd.localAnchorA.Set(0,0); //the center of our player
	rjd.localAnchorB.Set(-m_rect.getSize().x / PPM, 0);
	m_joint = (b2RevoluteJoint*)world.CreateJoint(&rjd);
}

Player::~Player()
{
	world.DestroyBody(m_body);
}

void Player::update()
{
	std::cout << m_body->GetLinearVelocity().y << std::endl;

}

void Player::render(sf::RenderWindow & window)
{
	m_rect.setPosition(m_body->GetPosition().x * PPM, m_body->GetPosition().y * PPM);
	m_rect.setRotation(m_body->GetAngle() * (180.f / thor::Pi)); //have to convert from radians to degrees here
	window.draw(m_rect);
	m_sword.render(window);
}

void Player::moveRight()
{
	m_body->ApplyForceToCenter(b2Vec2(12, 0), true);
}

void Player::moveLeft()
{
	m_body->ApplyForceToCenter(b2Vec2(-12, 0), true);
}

void Player::attack()
{
	m_sword.attack();
}
