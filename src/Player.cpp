#include "Player.h"

Player::Player(sf::Vector2f position, sf::Vector2f size = sf::Vector2f(15,15)) :
	m_canAttack(true),
	m_canJump(false),
	m_speed(0.0f),
	m_position(position),
	m_playerRect(size),
	m_forearmRect(sf::Vector2f(15, 5)),
	m_jumpRect(sf::Vector2f(size.x, 3)),
	m_sword(),
	RAD_TO_DEG(180.f / thor::Pi),
	DEG_TO_RAD(thor::Pi / 180.f)
{
	//creating our Box2d body and fixture for the player
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(m_position.x / PPM, m_position.y / PPM); //spawn the player in the center of the screen
	bodyDef.fixedRotation = true;
	m_playerBody = world.CreateBody(&bodyDef); //add the body to the world
	m_playerBody->SetUserData(this);

	b2BodyDef forearmDef;
	forearmDef.type = b2_dynamicBody;
	forearmDef.position.Set(m_position.x / PPM, m_position.y / PPM);
	forearmDef.fixedRotation = true;
	m_forearmBody = world.CreateBody(&forearmDef); //add the body to the world
	m_forearmBody->SetUserData(this);

	//creating our jump body
	b2BodyDef jumpDef;
	jumpDef.type = b2_dynamicBody;
	jumpDef.position.Set(m_position.x / PPM, (m_position.y + m_playerRect.getSize().y / 2.f) / PPM); //place our jumping body at the bottom of the player
	jumpDef.userData = this;
	m_jumpBody = world.CreateBody(&jumpDef);
	m_jumpBody->SetUserData(this);

	b2PolygonShape playerShape;
	playerShape.SetAsBox((m_playerRect.getSize().x / 2.f) / PPM, (m_playerRect.getSize().y / 2.f) / PPM);

	b2PolygonShape forearmShape;
	forearmShape.SetAsBox((m_forearmRect.getSize().x / 2.f) / PPM, (m_forearmRect.getSize().y / 2.f) / PPM);

	b2PolygonShape jumpShape;
	jumpShape.SetAsBox(5 / 2.f / PPM, 5 / 2.f / PPM);

	b2FixtureDef playerDef;
	playerDef.shape = &playerShape;
	playerDef.density = 1.5; //giving the player a mass of 1.5
	m_playerBody->CreateFixture(&playerDef);

	b2FixtureDef forearmFixDef;
	forearmFixDef.shape = &forearmShape;
	forearmFixDef.density = .25f; //giving the arm a mass of 0.1
	forearmFixDef.isSensor = true;
	m_forearmBody->CreateFixture(&forearmFixDef);

	b2FixtureDef jumpFixDef;
	jumpFixDef.shape = &jumpShape;
	jumpFixDef.isSensor = true;
	m_jumpBody->CreateFixture(&jumpFixDef);

	m_playerRect.setOrigin(m_playerRect.getSize().x / 2.f, m_playerRect.getSize().y / 2.f); //setting the origin to the center of the box
	m_playerRect.setFillColor(sf::Color::Transparent);
	m_playerRect.setOutlineColor(sf::Color::Red);
	m_playerRect.setOutlineThickness(1);

	m_forearmRect.setOrigin(m_forearmRect.getSize().x / 2.f, m_forearmRect.getSize().y / 2.f); //setting the origin to the center of the box
	m_forearmRect.setFillColor(sf::Color::Transparent);
	m_forearmRect.setOutlineColor(sf::Color::Red);
	m_forearmRect.setOutlineThickness(1);

	m_jumpRect.setOrigin(m_jumpRect.getSize().x / 2.f, m_jumpRect.getSize().y / 2.f); //setting the origin to the center of the box
	m_jumpRect.setFillColor(sf::Color::Transparent);
	m_jumpRect.setOutlineColor(sf::Color::Green);
	m_jumpRect.setOutlineThickness(1);

	//Creating the joint between our arm and player
	m_playerToArmJointDef.bodyA = m_playerBody;
	m_playerToArmJointDef.bodyB = m_forearmBody;
	m_playerToArmJointDef.collideConnected = false; //so our arm and player dont collide
	m_playerToArmJointDef.enableLimit = true;
	m_playerToArmJointDef.lowerTranslation = 0;
	m_playerToArmJointDef.upperTranslation = .66f;
	m_playerToArmJointDef.localAnchorA.Set(0, (2.5f) / PPM);
	m_playerToArmJointDef.localAnchorB.Set((-5) / PPM,(-5) /PPM);
	m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);


	//Creating the joint between our sword and player
	m_armToSwordJointDef.bodyA = m_forearmBody;
	m_armToSwordJointDef.bodyB = m_sword.getBody();
	m_armToSwordJointDef.collideConnected = false; //so our sword and player dont collide
	m_armToSwordJointDef.localAnchorA.Set(0, 0); //the center of our player
	m_armToSwordJointDef.localAnchorB.Set(-28.5f / PPM, 0);
	m_armToSwordJoint = (b2RevoluteJoint*)world.CreateJoint(&m_armToSwordJointDef);

	//creating our jump sensor joint
	m_jumpSensorJointDef.bodyA = m_playerBody;
	m_jumpSensorJointDef.bodyB = m_jumpBody;
	m_jumpSensorJointDef.collideConnected = false; //so our sensor and player dont collide
	m_jumpSensorJointDef.localAnchorA.Set(0, (m_playerRect.getSize().y / 2.f) / PPM); //center of the player
	m_jumpSensorJointDef.localAnchorB.Set(0, 0);
	m_jumpSensorJointDef.enableLimit = true;
	m_jumpSensorJointDef.lowerTranslation = 0;
	m_jumpSensorJointDef.upperTranslation = 0;
	m_jumpSensorJoint = (b2PrismaticJoint*)world.CreateJoint(&m_jumpSensorJointDef);
}

Player::~Player()
{
	//deleting our box2d variables from the world
	world.DestroyBody(m_playerBody);
	world.DestroyJoint(m_armToSwordJoint);
	world.DestroyJoint(m_playerToArmJoint);
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
	//drawing our player
	m_playerRect.setPosition(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM);
	m_playerRect.setRotation(m_playerBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	//drawing our arm
	m_forearmRect.setPosition(m_forearmBody->GetPosition().x * PPM, m_forearmBody->GetPosition().y * PPM);
	m_forearmRect.setRotation(m_forearmBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	//drawing our jump sensor
	m_jumpRect.setPosition(m_jumpBody->GetPosition().x * PPM, m_jumpBody->GetPosition().y * PPM);
	m_jumpRect.setRotation(m_jumpBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	window.draw(m_playerRect);
	window.draw(m_forearmRect);
	window.draw(m_jumpRect);
	m_sword.render(window);
}

void Player::moveRight()
{
	invertPlayerJoint(false);
	m_playerBody->SetLinearVelocity(b2Vec2(m_speed, m_playerBody->GetLinearVelocity().y));
}

void Player::moveLeft()
{
	invertPlayerJoint(true);
	m_playerBody->SetLinearVelocity(b2Vec2(m_speed, m_playerBody->GetLinearVelocity().y));
}

void Player::attack()
{
	if (m_canAttack)
	{
		if (m_isFacingLeft)
			m_forearmBody->ApplyForceToCenter(b2Vec2(-50, 0), true);
		else
			m_forearmBody->ApplyForceToCenter(b2Vec2(50,0), true);
	}

}

void Player::jump()
{
	m_playerBody->ApplyForceToCenter(b2Vec2(0, -2500),true);
}

void Player::setCanJump(bool canJump)
{
	m_canJump = canJump;
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

	std::cout << m_canJump << std::endl;

	if (controller.isButtonPressed("A") && m_canJump)
	{
		jump();
		m_canJump = false;
	}

	if (moved == false)
	{
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}
void Player::invertPlayerJoint(bool facingLeft)
{
	if (m_isFacingLeft == false && facingLeft || m_isFacingLeft && facingLeft == false)
	{
		//Seting parameters to our player to arm joint
		auto playerToArm = m_playerToArmJointDef;
		if (facingLeft)
		{
			playerToArm.lowerTranslation = -.66f;
			playerToArm.upperTranslation = 0;
		}
		else
		{
			playerToArm.lowerTranslation = 0;
			playerToArm.upperTranslation = .66f;
		}
		playerToArm.localAnchorB.Set(m_playerToArmJointDef.localAnchorB.x * -1, m_playerToArmJointDef.localAnchorB.y);
		world.DestroyJoint(m_playerToArmJoint);
		m_playerToArmJointDef = playerToArm;
		m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);

		//Setting parameters for our arm to sword joint
		auto armToSword = m_armToSwordJointDef;
		armToSword.localAnchorB.Set(m_armToSwordJointDef.localAnchorB.x * -1, m_armToSwordJointDef.localAnchorB.y);
		world.DestroyJoint(m_armToSwordJoint);
		m_armToSwordJointDef = armToSword;
		m_armToSwordJoint = (b2RevoluteJoint*)world.CreateJoint(&m_armToSwordJointDef);

		//set is facing boolean here
		if (facingLeft)
			m_isFacingLeft = true;
		else
			m_isFacingLeft = false;

		m_playerBody->SetUserData(this);
		m_forearmBody->SetUserData(this);
		m_jumpBody->SetUserData(this);
	}
}

b2Body * Player::getJumpSensor()
{
	return m_jumpBody;
}
