#include "Player.h"

Player::Player(sf::Vector2f position, sf::Vector2f size = sf::Vector2f(15, 15), std::string direction = "left") :
	m_canAttack(true),
	m_canJump(false),
	m_canAttackTemp(true),
	m_swordReachedPos(false),
	m_moveSpeed(7.0f),
	m_gravityScale(1.75f),
	m_weaponPos(1), //the centre of our player
	m_attackRate(0.50f),
	m_position(position),
	m_startPosition(position.x / PPM, position.y / PPM),
	m_playerRect(size),
	m_forearmRect(sf::Vector2f(15, 5)),
	m_jumpRect(sf::Vector2f(size.x, 3)),
	m_sword(position),
	RAD_TO_DEG(180.f / thor::Pi),
	DEG_TO_RAD(thor::Pi / 180.f)
{
	m_weaponPosChange = (size.y / 3.75f) / PPM; //how much our weapon changes in position when we change the height it is at

	if (direction == "left")
		m_isFacingLeft = true;
	else if (direction == "right")
		m_isFacingLeft = false;

	//creating our Box2d body and fixture for the player
	m_playerbodyDef.type = b2_dynamicBody;
	m_playerbodyDef.position.Set(m_position.x / PPM, m_position.y / PPM); //spawn the player in the center of the screen
	m_playerbodyDef.fixedRotation = true;
	m_playerbodyDef.bullet = true; // we make our body a bullet so collision detection occurs more often for or player(s)
	m_playerBody = world.CreateBody(&m_playerbodyDef); //add the body to the world
	m_playerBody->SetUserData(this);
	m_playerBody->SetGravityScale(m_gravityScale);

	m_forearmBodyDef.type = b2_dynamicBody;
	m_forearmBodyDef.position.Set(m_position.x / PPM, m_position.y / PPM);
	m_forearmBodyDef.fixedRotation = true;
	m_forearmBody = world.CreateBody(&m_forearmBodyDef); //add the body to the world
	m_forearmBody->SetUserData(this);
	m_forearmBody->SetGravityScale(m_gravityScale);

	//creating our jump body
	b2BodyDef jumpDef;
	jumpDef.type = b2_dynamicBody;
	jumpDef.position.Set(m_position.x / PPM, (m_position.y + m_playerRect.getSize().y / 2.f) / PPM); //place our jumping body at the bottom of the player
	jumpDef.userData = this;
	jumpDef.bullet = true; //we want our jump body to be a bullet so collision detection occurs more often, this will allow for smoother jumping
	m_jumpBody = world.CreateBody(&jumpDef);
	m_jumpBody->SetUserData(this);
	m_jumpBody->SetGravityScale(m_gravityScale);

	b2PolygonShape playerShape;
	playerShape.SetAsBox((m_playerRect.getSize().x / 2.f) / PPM, (m_playerRect.getSize().y / 2.f) / PPM);

	b2PolygonShape forearmShape;
	forearmShape.SetAsBox((m_forearmRect.getSize().x / 2.f) / PPM, (m_forearmRect.getSize().y / 2.f) / PPM);

	b2PolygonShape jumpShape;
	jumpShape.SetAsBox(m_jumpRect.getSize().x / 2.f / PPM, m_jumpRect.getSize().y / 2.f / PPM);

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
	if (m_isFacingLeft)
	{
		m_playerToArmJointDef.lowerTranslation = -2.5;
		m_playerToArmJointDef.upperTranslation = 0;
		m_playerToArmJointDef.localAnchorB.Set(5 / PPM, 0);
	}
	else
	{
		m_playerToArmJointDef.lowerTranslation = 0;
		m_playerToArmJointDef.upperTranslation = 2.5f;
		m_playerToArmJointDef.localAnchorB.Set((-5) / PPM, 0);
	}
	m_playerToArmJointDef.localAnchorA.Set(0, (2.5f) / PPM);

	m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);


	//Creating the joint between our sword and player
	m_armToSwordJointDef.bodyA = m_forearmBody;
	m_armToSwordJointDef.bodyB = m_sword.getBody();
	m_armToSwordJointDef.collideConnected = false; //so our sword and player dont collide
	m_armToSwordJointDef.enableLimit = true; //enable a rotational limit on our sword
	m_armToSwordJointDef.localAnchorA.Set(0, 0); //the center of our player
	if (m_isFacingLeft)
	{
		m_armToSwordJointDef.localAnchorB.Set(28.5f / PPM, 0);
		m_armToSwordJointDef.lowerAngle = 0 * DEG_TO_RAD;
		m_armToSwordJointDef.upperAngle = 45 * DEG_TO_RAD;
	}
	else
	{
		m_armToSwordJointDef.localAnchorB.Set(-28.5f / PPM, 0);
		m_armToSwordJointDef.lowerAngle = -45 * DEG_TO_RAD;
		m_armToSwordJointDef.upperAngle = 0 * DEG_TO_RAD;
	}	
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

	m_attackClock.restart(); //start our clock when the player is created
}

Player::~Player()
{
	
}

void Player::update()
{
	//if our respawn variable is true then respawn our player
	if (m_respawn)
		respawn();

	checkCanAttack(); //checks if we can attack or not

	//if we have attacked then set our linear velocity to move in the direction we are facing
	if(m_canAttack == false)
	{
		//if our sword hasnt reached our 
		if (m_swordReachedPos == false)
		{
			//get the current position fo our arm
			auto armPos = sf::Vector2f(m_forearmBody->GetPosition().x * PPM, m_forearmBody->GetPosition().y * PPM);

			//if our arm has not reached its destination then set its velocity
			if (distance(armPos, m_armPosDest, 10.0f) == false)
			{
				if (m_isFacingLeft)
					m_forearmBody->SetLinearVelocity(b2Vec2(-48, 0)); 
				else
					m_forearmBody->SetLinearVelocity(b2Vec2(48, 0));
			}
			else
				m_swordReachedPos = true;
		}
		else
		{
			applyArmPushBack(); //push back our arm
		}
	}
	else
	{
		applyArmPushBack(); //push back our arm
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
	if (m_canAttack)
	{
		invertPlayerJoint(false);
		m_playerBody->SetLinearVelocity(b2Vec2(m_moveSpeed, m_playerBody->GetLinearVelocity().y));
	}
	else
	{
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}

void Player::moveLeft()
{
	if (m_canAttack)
	{
		invertPlayerJoint(true);
		m_playerBody->SetLinearVelocity(b2Vec2(-m_moveSpeed, m_playerBody->GetLinearVelocity().y));
	}
	else
	{
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}

void Player::attack()
{
	if (m_canAttack)
	{
		m_canAttackTemp = m_canAttack;
		m_swordReachedPos = false; //set our sword reaching its position boolean to false

		m_canAttack = false; //make our bool false so we cannot attack again

		m_attackClock.restart(); //reset our attack clock

		if(m_isFacingLeft)
			m_armPosDest = sf::Vector2f((m_forearmBody->GetPosition().x * PPM) + m_playerToArmJoint->GetLowerLimit() * PPM, m_forearmBody->GetPosition().y * PPM);
		else
			m_armPosDest = sf::Vector2f((m_forearmBody->GetPosition().x * PPM) + m_playerToArmJoint->GetUpperLimit() * PPM, m_forearmBody->GetPosition().y * PPM);
	}
}

void Player::jump()
{
	m_playerBody->ApplyForceToCenter(b2Vec2(0, -3000),true);
}

void Player::setCanJump(bool canJump)
{
	m_canJump = canJump;
}

void Player::handleJoystick(JoystickController & controller)
{
	bool moved = false;
	if (controller.isButtonHeld("LeftThumbStickLeft") && m_canAttack)
	{
		moved = true;
		moveLeft();
		rotateWhileRunning(true);
	}
	if (controller.isButtonHeld("LeftThumbStickRight") && m_canAttack)
	{
		moved = true;
		moveRight();
		rotateWhileRunning(true);
	}

	if (controller.isButtonPressed("LeftThumbStickUp") && moved == false && m_canAttack || controller.isButtonPressed("DpadUp") && moved == false && m_canAttack)
		changeSwordStance("Up");

	if (controller.isButtonPressed("LeftThumbStickDown") && moved == false && m_canAttack || controller.isButtonPressed("DpadDown") && moved == false && m_canAttack)
		changeSwordStance("Down");

	if (controller.isButtonPressed("X"))
		attack();
	if (controller.isButtonPressed("Y"))
		respawn();

	if (controller.isButtonPressed("A") && m_canJump)
	{
		jump();
		m_canJump = false;
	}

	if (moved == false)
	{
		rotateWhileRunning(false);
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}
void Player::invertPlayerJoint(bool facingLeft)
{
	if (m_isFacingLeft == false && facingLeft || m_isFacingLeft && facingLeft == false)
	{
		//get our arm to sword joint
		auto armToSword = m_armToSwordJointDef;

		//Seting parameters to our player to arm joint
		auto playerToArm = m_playerToArmJointDef;
		if (facingLeft)
		{
			armToSword.lowerAngle = 0 * DEG_TO_RAD;
			armToSword.upperAngle = 45 * DEG_TO_RAD;
			playerToArm.lowerTranslation = -2.5f;
			playerToArm.upperTranslation = 0;
		}
		else
		{
			armToSword.lowerAngle = -45 * DEG_TO_RAD;
			armToSword.upperAngle = 0 * DEG_TO_RAD;
			playerToArm.lowerTranslation = 0;
			playerToArm.upperTranslation = 2.5f;
		}
		playerToArm.localAnchorB.Set(m_playerToArmJointDef.localAnchorB.x * -1, m_playerToArmJointDef.localAnchorB.y);
		world.DestroyJoint(m_playerToArmJoint);
		m_playerToArmJointDef = playerToArm;
		m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);

		//Setting parameters for our arm to sword joint
		armToSword.localAnchorB.Set(m_armToSwordJointDef.localAnchorB.x * -1, m_armToSwordJointDef.localAnchorB.y);
		world.DestroyJoint(m_armToSwordJoint);
		m_armToSwordJointDef = armToSword;
		m_armToSwordJoint = (b2RevoluteJoint*)world.CreateJoint(&m_armToSwordJointDef);

		//set is facing boolean here
		if (facingLeft)
			m_isFacingLeft = true;
		else
			m_isFacingLeft = false;
	}
}

void Player::changeSwordStance(std::string direction)
{
	if (direction == "Up")
	{
		//if we can move up and we are not already at the top stance of our player
		if (m_weaponPos < 2)
		{
			m_weaponPos++; //increase our pos

			//Seting parameters to our player to arm joint
			auto playerToArm = m_playerToArmJointDef;
			playerToArm.localAnchorB.Set(m_playerToArmJointDef.localAnchorB.x, m_playerToArmJointDef.localAnchorB.y + m_weaponPosChange);
			world.DestroyJoint(m_playerToArmJoint);
			m_playerToArmJointDef = playerToArm;
			m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);
		}
	}
	else if (direction == "Down")
	{
		if (m_weaponPos > 0)
		{
			m_weaponPos--; //increase our pos

			//Seting parameters to our player to arm joint
			auto playerToArm = m_playerToArmJointDef;
			playerToArm.localAnchorB.Set(m_playerToArmJointDef.localAnchorB.x, m_playerToArmJointDef.localAnchorB.y - m_weaponPosChange);
			world.DestroyJoint(m_playerToArmJoint);
			m_playerToArmJointDef = playerToArm;
			m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);
		}
	}

}

void Player::swordClashed()
{
	m_swordReachedPos = true; //set our sword as reaching its position (incase we are attacking we dont want our sword to keep moving as it has hit a sword)
	m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));

	//bounce our player away
	if(m_isFacingLeft)
		m_playerBody->ApplyForceToCenter(b2Vec2(2500, 0), true);
	else
		m_playerBody->ApplyForceToCenter(b2Vec2(-2500, 0), true);
}

void Player::checkCanAttack()
{
	//if the time elapsed since teh clock was started/restarted is greater than or equal to our attack rate then set out bool
	if (m_attackClock.getElapsedTime().asSeconds() >= m_attackRate)
	{
		m_canAttack = true;
	}
}

void Player::applyArmPushBack()
{
	if (m_isFacingLeft)
		m_forearmBody->ApplyForceToCenter(b2Vec2(4.5f, 0), true);
	else
		m_forearmBody->ApplyForceToCenter(b2Vec2(-4.5f, 0), true);
}

void Player::rotateWhileRunning(bool rotate)
{
	if (rotate)
	{
		if (m_isFacingLeft)
			m_sword.getBody()->ApplyAngularImpulse(.06, true);
		else
			m_sword.getBody()->ApplyAngularImpulse(-.06, true);
	}
	else
	{
		if (m_isFacingLeft)
			m_sword.getBody()->ApplyAngularImpulse(-.06, true);
		else
			m_sword.getBody()->ApplyAngularImpulse(.06, true);
	}
}

void Player::respawn()
{
	//clears forces in the world, this will allow our player to not recieve any forces when respawned
	world.ClearForces();

	//Set all of our bodies to our start position
	m_playerBody->SetTransform(m_startPosition, m_playerBody->GetAngle());
	m_forearmBody->SetTransform(m_startPosition, m_forearmBody->GetAngle());
	m_jumpBody->SetTransform(m_startPosition, m_jumpBody->GetAngle());
	m_sword.setPosition(m_startPosition);

	m_respawn = false;
}

bool Player::distance(sf::Vector2f point1, sf::Vector2f point2, float distanceCuttOff)
{
	auto tempX = std::powf(point2.x - point1.x, 2);
	auto diff = std::sqrtf(tempX);

	//if the difference between the two points is less than or equal to our parameter then return true
	if (diff <= distanceCuttOff)
		return true;

	return false;
}

b2Body * Player::getJumpBody()
{
	return m_jumpBody;
}

b2Body * Player::getArmBody()
{
	return m_forearmBody;
}

b2Body * Player::getPlayerBody()
{
	return m_playerBody;
}

b2Body * Player::getSwordBody()
{
	return m_sword.getBody();
}

void Player::setRespawn(bool respawn)
{
	m_respawn = respawn;
}
bool Player::getCanAttack()
{
	bool temp = m_canAttackTemp;
	m_canAttackTemp = false;
	return temp;
}
