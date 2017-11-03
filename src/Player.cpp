#include "Player.h"

Player::Player(sf::Vector2f size = sf::Vector2f(15,15)) :
	m_playerRect(size),
	m_forearmRect(sf::Vector2f(15  ,5  )),
	m_sword(),
	m_canAttack(true),
	m_speed(0.0f)
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set((1280 / 2.f) / PPM, (720 / 2.f) / PPM); //spawn the player in the center of the screen
	bodyDef.fixedRotation = true;
	m_playerBody = world.CreateBody(&bodyDef); //add the body to the world

	b2BodyDef forearmDef;
	forearmDef.type = b2_dynamicBody;
	forearmDef.position.Set((1280 / 2.f) / PPM, (720 / 2.f) / PPM); //spawn the player in the center of the screen
	forearmDef.fixedRotation = true;
	m_forearmBody = world.CreateBody(&forearmDef); //add the body to the world

	b2PolygonShape playerShape;
	playerShape.SetAsBox((m_playerRect.getSize().x / 2.f) / PPM, (m_playerRect.getSize().y / 2.f) / PPM);

	b2PolygonShape forearmShape;
	forearmShape.SetAsBox((m_forearmRect.getSize().x / 2.f) / PPM, (m_forearmRect.getSize().y / 2.f) / PPM);

	b2FixtureDef playerDef;
	playerDef.shape = &playerShape;
	playerDef.density = 1.5; //giving the player a mass of 1
	m_playerBody->CreateFixture(&playerDef);

	b2FixtureDef forearmFixDef;
	forearmFixDef.shape = &forearmShape;
	forearmFixDef.density = .1f; //giving the player a mass of 1
	forearmFixDef.isSensor = true;
	m_forearmBody->CreateFixture(&forearmFixDef);

	m_playerRect.setOrigin(m_playerRect.getSize().x / 2, m_playerRect.getSize().y / 2); //setting the origin to the center of the box
	m_playerRect.setFillColor(sf::Color::Transparent);
	m_playerRect.setOutlineColor(sf::Color::Red);
	m_playerRect.setOutlineThickness(1);


	m_forearmRect.setOrigin(m_forearmRect.getSize().x / 2, m_forearmRect.getSize().y / 2); //setting the origin to the center of the box
	m_forearmRect.setFillColor(sf::Color::Transparent);
	m_forearmRect.setOutlineColor(sf::Color::Red);
	m_forearmRect.setOutlineThickness(1);

	//Creating the joint between our arm and player
	b2PrismaticJointDef sjf;
	sjf.bodyA = m_playerBody;
	sjf.bodyB = m_forearmBody;
	sjf.collideConnected = false; //so our sword and player dont collide
	sjf.enableLimit = true;
	sjf.lowerTranslation = 0;
	sjf.upperTranslation = .66;
	sjf.localAnchorA.Set(0, (2.5) / PPM); //the center of our player
	sjf.localAnchorB.Set((-5) / PPM,(-5) /PPM);
	m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&sjf);


	//Creating the joint between our sword and player
	b2RevoluteJointDef rjd;
	rjd.bodyA = m_forearmBody;
	rjd.bodyB = m_sword.getBody();
	rjd.collideConnected = false; //so our sword and player dont collide
	rjd.localAnchorA.Set(0, 0); //the center of our player
	rjd.localAnchorB.Set(-28.5 / PPM, 0);
	m_handWeaponJoint = (b2RevoluteJoint*)world.CreateJoint(&rjd);


}

Player::~Player()
{
	world.DestroyBody(m_playerBody);
}

void Player::update()
{
	if (m_playerToArmJoint->GetJointTranslation() >= -0.15 && m_playerToArmJoint->GetJointTranslation() <= 0.15)
	{
		m_forearmBody->SetLinearVelocity(b2Vec2(0, 0));
		m_canAttack = true;
	}
	else
	{
		if(m_isFacingLeft)
			m_forearmBody->ApplyForceToCenter(b2Vec2(1, 0), true);
		else
			m_forearmBody->ApplyForceToCenter(b2Vec2(-1, 0), true);
		m_canAttack = false;
	}
}

void Player::render(sf::RenderWindow & window)
{
	m_playerRect.setPosition(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM);
	m_playerRect.setRotation(m_playerBody->GetAngle() * (180.f / thor::Pi)); //have to convert from radians to degrees here

	m_forearmRect.setPosition(m_forearmBody->GetPosition().x * PPM, m_forearmBody->GetPosition().y * PPM);
	m_forearmRect.setRotation(m_forearmBody->GetAngle() * (180.f / thor::Pi)); //have to convert from radians to degrees here

	window.draw(m_playerRect);
	window.draw(m_forearmRect);
	m_sword.render(window);
}

void Player::moveRight()
{
	createPlayerJoint(false);
	m_playerBody->SetLinearVelocity(b2Vec2(m_speed, m_playerBody->GetLinearVelocity().y));
}

void Player::moveLeft()
{
	createPlayerJoint(true);
	m_playerBody->SetLinearVelocity(b2Vec2(m_speed, m_playerBody->GetLinearVelocity().y));
}

void Player::attack()
{
	if (m_canAttack)
	{
		if(m_isFacingLeft)
			m_forearmBody->ApplyForceToCenter(b2Vec2(-50, 0), true);
		else
			m_forearmBody->ApplyForceToCenter(b2Vec2(50,0), true);
	}

}

void Player::handleJoystick(JoystickController & controller)
{
	bool moved = false;
	if (controller.isButtonHeld("LeftThumbStickLeft"))
	{
		moved = true;
		m_speed = -7;
		moveLeft();
	}
	if (controller.isButtonHeld("LeftThumbStickRight"))
	{
		moved = true;
		m_speed = 7;
		moveRight();
	}
	if (controller.isButtonPressed("X"))
		attack();

	if (moved == false)
	{
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}
void Player::createPlayerJoint(bool facingLeft)
{
	if (m_isFacingLeft == false && facingLeft || m_isFacingLeft && facingLeft == false)
	{
		//Creating the joint between our sword and player
		b2PrismaticJointDef pjd;
		pjd.bodyA = m_playerToArmJoint->GetBodyA();
		pjd.bodyB = m_playerToArmJoint->GetBodyB();
		pjd.collideConnected = false; //so our sword and player dont collide
		pjd.enableLimit = true;
		if (facingLeft)
		{
			pjd.lowerTranslation = -.66;
			pjd.upperTranslation = 0;
		}
		else
		{
			pjd.lowerTranslation = 0;
			pjd.upperTranslation = .66;
		}
		pjd.localAnchorA.Set(m_playerToArmJoint->GetLocalAnchorA().x, m_playerToArmJoint->GetLocalAnchorA().y); //the center of our player
		pjd.localAnchorB.Set(m_playerToArmJoint->GetLocalAnchorB().x * -1, m_playerToArmJoint->GetLocalAnchorB().y);
		world.DestroyJoint(m_playerToArmJoint);
		m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&pjd);

		//Creating the joint between our sword and player
		b2RevoluteJointDef rjd;
		rjd.bodyA = m_forearmBody;
		rjd.bodyB = m_sword.getBody();
		rjd.collideConnected = false; //so our sword and player dont collide
		rjd.localAnchorA.Set(0, 0); //the center of our player
		rjd.localAnchorB.Set(m_handWeaponJoint->GetLocalAnchorB().x * -1, 0);
		world.DestroyJoint(m_handWeaponJoint);
		m_handWeaponJoint = (b2RevoluteJoint*)world.CreateJoint(&rjd);

		//set is facing boolean here
		if (facingLeft)
			m_isFacingLeft = true;
		else
			m_isFacingLeft = false;

		std::cout << "Switched Faces" << std::endl;
	}
}
