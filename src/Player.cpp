#include "Player.h"

Player::Player(sf::Vector2f position, std::string direction = "left") :
	m_canAttack(true),
	m_canJump(false),
	m_canAttackTemp(true),
	m_swordReachedPos(false),
	m_respawn(false),
	m_isAiming(false),
	m_holdingSword(true),
	m_parried(false),
	m_switchedSwordPos(false),
	m_playingPickup(false),
	m_canPickupSword(false),
	m_moveSpeed(7.5f),
	m_gravityScale(2.75f),
	m_weaponPos(1), //the centre of our player
	m_attackRate(0.50f),
	m_position(position),
	m_startPosition(position.x / PPM, position.y / PPM),
	m_playerRect(sf::Vector2f(18, 87)),
	m_forearmRect(sf::Vector2f(15, 5)),
	m_jumpRect(sf::Vector2f(18, 3)),
	m_leftWallSensorRect(sf::Vector2f(3, 87)),
	m_rightWallSensorRect(sf::Vector2f(3, 87)),
	m_sword(position),
	m_animator(m_animationHolder),
	m_idleTime(.5f),
	m_runTime(.7f),
	RAD_TO_DEG(180.f / thor::Pi),
	DEG_TO_RAD(thor::Pi / 180.f)
{
	//Load our shader
	m_recolourShader.loadFromFile(resourceManager.getShaderHolder()["recolourShader"], sf::Shader::Fragment);

	m_weaponPosChange = (50 / 3.0f) / PPM; //how much our weapon changes in position when we change the height it is at

	m_startingDirection = direction; //need to change this eventually so we can respawn in either direction

	if (direction == "left")
		m_isFacingLeft = true;
	else if (direction == "right")
		m_isFacingLeft = false;

	//Creating our bodies using our CreateBody Method
	m_playerBody = &createBody(m_playerBody, b2_dynamicBody, b2Vec2(m_position.x / PPM, (m_position.y + m_playerRect.getSize().y / 2.f) / PPM), true, true, m_gravityScale);
	m_forearmBody = &createBody(m_forearmBody, b2_dynamicBody, b2Vec2(m_position.x / PPM, m_position.y / PPM), true, true, m_gravityScale);
	m_jumpBody = &createBody(m_jumpBody, b2_dynamicBody, b2Vec2(m_position.x / PPM, (m_position.y + m_playerRect.getSize().y / 2.f) / PPM), true, true, m_gravityScale);
	m_leftSensorBody = &createBody(m_leftSensorBody, b2_dynamicBody, b2Vec2((m_position.x - m_playerRect.getSize().x / 2.0f) / PPM, m_position.y / PPM), true, true, m_gravityScale);
	m_rightSensorBody = &createBody(m_rightSensorBody, b2_dynamicBody, b2Vec2((m_position.x + m_playerRect.getSize().x / 2.0f) / PPM, m_position.y / PPM), true, true, m_gravityScale);

	//Creating our fixtures for each of our bodies
	createFixture(m_playerBody, m_playerRect.getSize().x, m_playerRect.getSize().y, false, 1.5f, 0.0f, 0.1f);
	createFixture(m_forearmBody, m_forearmRect.getSize().x, m_forearmRect.getSize().y, true, .25f, 0.0f, 1.0f);
	createFixture(m_jumpBody, m_jumpRect.getSize().x, m_jumpRect.getSize().y, true, 0.0f, 0.0f, 1.0f);
	createFixture(m_leftSensorBody, m_leftWallSensorRect.getSize().x, m_leftWallSensorRect.getSize().y, true, 0.0f, 0.0f, 1.0f);
	createFixture(m_rightSensorBody, m_rightWallSensorRect.getSize().x, m_rightWallSensorRect.getSize().y, true, 0.0f, 0.0f, 1.0f);

	//Creating/Setting up our rectangles for our bodies
	createRectangle(m_playerRect, sf::Vector2f(m_playerRect.getSize().x / 2.f, m_playerRect.getSize().y / 2.f), sf::Color::Transparent, sf::Color::Red, 1);
	createRectangle(m_forearmRect, sf::Vector2f(m_forearmRect.getSize().x / 2.f, m_forearmRect.getSize().y / 2.f), sf::Color::White, sf::Color::Transparent, 0);
	createRectangle(m_jumpRect, sf::Vector2f(m_jumpRect.getSize().x / 2.f, m_jumpRect.getSize().y / 2.f), sf::Color::Transparent, sf::Color::Green, 1);
	createRectangle(m_leftWallSensorRect, sf::Vector2f(m_leftWallSensorRect.getSize().x / 2.f, m_leftWallSensorRect.getSize().y / 2.f), sf::Color::Transparent, sf::Color::Green, 1);
	createRectangle(m_rightWallSensorRect, sf::Vector2f(m_rightWallSensorRect.getSize().x / 2.f, m_rightWallSensorRect.getSize().y / 2.f), sf::Color::Transparent, sf::Color::Green, 1);

	//Creating the joint between our arm and player
	m_playerToArmJointDef.bodyA = m_playerBody;
	m_playerToArmJointDef.bodyB = m_forearmBody;
	m_playerToArmJointDef.collideConnected = false; //so our arm and player dont collide
	m_playerToArmJointDef.enableLimit = true;
	if (m_isFacingLeft)
	{
		m_playerToArmJointDef.lowerTranslation = -2.5f;
		m_playerToArmJointDef.upperTranslation = 0;
		m_playerToArmJointDef.localAnchorB.Set(15 / PPM, 18 / PPM);
	}
	else
	{
		m_playerToArmJointDef.lowerTranslation = 0;
		m_playerToArmJointDef.upperTranslation = 2.5f;
		m_playerToArmJointDef.localAnchorB.Set(-15 / PPM, 18 / PPM);
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
		m_armToSwordJointDef.upperAngle = 15 * DEG_TO_RAD;
	}
	else
	{
		m_armToSwordJointDef.localAnchorB.Set(-28.5f / PPM, 0);
		m_armToSwordJointDef.lowerAngle = -15 * DEG_TO_RAD;
		m_armToSwordJointDef.upperAngle = 0 * DEG_TO_RAD;
	}	
	m_armToSwordJoint = (b2RevoluteJoint*)world.CreateJoint(&m_armToSwordJointDef);

	//creating our jump sensor joint
	m_jumpSensorJointDef.bodyA = m_playerBody;
	m_jumpSensorJointDef.bodyB = m_jumpBody;
	m_jumpSensorJointDef.collideConnected = false; //so our sensor and player dont collide
	m_jumpSensorJointDef.localAnchorA.Set(0, (m_playerRect.getSize().y / 2.f) / PPM); //center of the player
	m_jumpSensorJointDef.localAnchorB.Set(0, 0);
	m_jumpSensorJoint = (b2WeldJoint*)world.CreateJoint(&m_jumpSensorJointDef);

	/*------> TEMPORARY <------*/
	b2WeldJointDef leftWeld;
	leftWeld.bodyA = m_leftSensorBody;
	leftWeld.bodyB = m_playerBody;
	leftWeld.collideConnected = false; //so our sensor and player dont collide
	leftWeld.localAnchorA.Set(-(m_playerRect.getSize().x  / 2.0f) / PPM, 0);
	leftWeld.localAnchorB.Set(0, 0);
	m_leftWallSensorJoint = (b2WeldJoint*)world.CreateJoint(&leftWeld);

	b2WeldJointDef rightWeld;
	rightWeld.bodyA = m_rightSensorBody;
	rightWeld.bodyB = m_playerBody;
	rightWeld.collideConnected = false; //so our sensor and player dont collide
	rightWeld.localAnchorA.Set((m_playerRect.getSize().x / 2.0f) / PPM, 0);
	rightWeld.localAnchorB.Set(0, 0);
	m_rightWallSensorJoint = (b2WeldJoint*)world.CreateJoint(&rightWeld);
	/*------> TEMPORARY <------*/



	m_attackClock.restart(); //start our clock when the player is created

	//Setting up our sprites
	setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerIdle"], sf::Vector2i(42, 87), 24);
	setSpriteTexture(m_lightSprite, resourceManager.getTextureHolder()["playerLight"], sf::Vector2i(300, 300), 150);

	if (m_isFacingLeft)
	{
		m_sword.negateSword();
		m_sprite.setScale(-1, 1);
	}

	//Setup all of our animations
	setUpAnimations();

	if (m_isFacingLeft)
	{
		setColour(sf::Color(244, 241, 66, 255));
		m_forearmRect.setFillColor(sf::Color(244, 241, 66, 255));
	}
	else
	{
		setColour(sf::Color(65, 244, 232));
		m_forearmRect.setFillColor(sf::Color(65, 244, 232));
	}
}

Player::~Player()
{
	
}

b2Body& Player::createBody(b2Body* body, b2BodyType bodyType, b2Vec2 position, bool isBullet, bool fixRotation, float gravityScale)
{
	b2BodyDef bodyDef;//Create a body definition

	bodyDef.type = bodyType; //Set the body type (Dynamic, Static, Kinematic)
	bodyDef.position.Set(position.x, position.y); //Sets the inital position of the body
	bodyDef.fixedRotation = fixRotation; //If this bool is true then the body will keep its rotation when it collides
	bodyDef.bullet = isBullet; //If isBulle tis true then collision detection will occur more frequently
	body = world.CreateBody(&bodyDef); //add the body to the world
	body->SetUserData(this); //Set the user data of this body to this object (Player)
	body->SetGravityScale(gravityScale); //Set the gravity scale of the body
	return *body; //return the body
}

void Player::createFixture(b2Body * body, float width, float height, bool isSensor, float density, float restitution, float friction)
{
	b2PolygonShape shape;
	b2FixtureDef fixDef;

	//Creating our shape
	shape.SetAsBox(width / 2.0f / PPM, height / 2.0 / PPM);

	//Setting the parameters of the fixture
	fixDef.shape = &shape;
	fixDef.isSensor = isSensor;
	fixDef.density = density;
	fixDef.restitution = restitution;
	fixDef.friction = friction;
	
	body->CreateFixture(&fixDef); //Create the fixture for the passed ove rbody
}

void Player::createRectangle(sf::RectangleShape & rect, sf::Vector2f origin, sf::Color fill, sf::Color stroke, float strokeWidth)
{
	rect.setOrigin(origin);
	rect.setFillColor(fill);
	rect.setOutlineColor(stroke);
	rect.setOutlineThickness(strokeWidth);
}

void Player::update()
{

	//if our respawn variable is true then respawn our player
	if (m_respawn && m_respawnClock.getElapsedTime().asSeconds() >= 0.0f)
		respawn();
	else
	{
		//Update our animation
		m_animator.update(m_animationClock.restart());
		m_animator.animate(m_sprite);

		m_sword.update();

		if (m_playingPickup)
		{
			if (m_pickupClock.getElapsedTime().asSeconds() >= 0.125f && m_canPickupSword ||
				m_pickupClock.getElapsedTime().asSeconds() >= 0.125f && distance(sf::Vector2f(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM), m_sword.getPosition(), 75))
			{
				m_pickupSword = true;
				m_canPickupSword = false;
			}
			if (m_pickupClock.getElapsedTime().asSeconds() >= 0.25f)
				m_playingPickup = false;
		}

		if (m_switchedSwordPos)
		{
			//if the time elapsed since the clock was started/restarted is greater than or equal to our stance change rate
			if (m_stanceChangeClock.getElapsedTime().asSeconds() >= 0.125f)
			{
				m_switchedSwordPos = false;
			}
		}

		//if we were parried and we are holding a sword
		if (m_parried && m_holdingSword)
		{
			parried();
		}

		if (m_pickupSword)
		{
			pickUpWeapon();
			m_pickupSword = false;
		}

		if (m_swordClashed)
		{
			swordClashed();
		}

		//If we are in the air then apply a force downwards
		if (m_canJump == false)
		{
			m_playerBody->ApplyForceToCenter(b2Vec2(0, 10), true);
		}

		checkCanAttack(); //checks if we can attack or not

		//if we have attacked then set our linear velocity to move in the direction we are facing
		if (m_canAttack == false)
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

}

void Player::render(sf::RenderWindow & window)
{
	//Set the position of our light texture
	m_lightSprite.setPosition(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM);

	//draw our sprite
	m_sprite.setPosition(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM);
	window.draw(m_sprite, &m_recolourShader);

	//drawing our player
	m_playerRect.setPosition(m_playerBody->GetPosition().x * PPM, m_playerBody->GetPosition().y * PPM);
	m_playerRect.setRotation(m_playerBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	//drawing our arm
	m_forearmRect.setPosition(m_forearmBody->GetPosition().x * PPM, m_forearmBody->GetPosition().y * PPM);
	m_forearmRect.setRotation(m_forearmBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	//drawing our jump sensor
	m_jumpRect.setPosition(m_jumpBody->GetPosition().x * PPM, m_jumpBody->GetPosition().y * PPM);
	m_jumpRect.setRotation(m_jumpBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here

	/*------> TEMPORARY <------*/
	m_leftWallSensorRect.setPosition(m_leftSensorBody->GetPosition().x * PPM, m_leftSensorBody->GetPosition().y * PPM);
	m_leftWallSensorRect.setRotation(m_leftSensorBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here
	m_rightWallSensorRect.setPosition(m_rightSensorBody->GetPosition().x * PPM, m_rightSensorBody->GetPosition().y * PPM);
	m_rightWallSensorRect.setRotation(m_rightSensorBody->GetAngle() * RAD_TO_DEG); //have to convert from radians to degrees here
	/*------> TEMPORARY <------*/


	m_sword.render(window);
	window.draw(m_playerRect);
	window.draw(m_forearmRect);
	window.draw(m_jumpRect);

	/*------> TEMPORARY <------*/
	window.draw(m_rightWallSensorRect);
	window.draw(m_leftWallSensorRect);
	/*------> TEMPORARY <------*/
}

void Player::moveRight()
{
	invertPlayerJoint(false);

	b2Vec2 vel = m_playerBody->GetLinearVelocity();
	float desiredVel = m_moveSpeed;
	float velChange = desiredVel - vel.x;
	float force = m_playerBody->GetMass() * velChange / (1 / 60.0);
	m_playerBody->ApplyForce(b2Vec2(force, 0), m_playerBody->GetWorldCenter(), true);
}

void Player::moveLeft()
{
	invertPlayerJoint(true);

	b2Vec2 vel = m_playerBody->GetLinearVelocity();
	float desiredVel = -m_moveSpeed;
	float velChange = desiredVel - vel.x;
	float force = m_playerBody->GetMass() * velChange / (1 / 60.0);
	m_playerBody->ApplyForce(b2Vec2(force, 0), m_playerBody->GetWorldCenter(), true);
}

void Player::attack()
{
	if (m_canAttack && m_holdingSword) //if we can attack and we have a sword
	{
		setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerAttack"], sf::Vector2i(56, 88), 24.0f);

		m_animator.stop(); //stop any animations playing
		m_animator.play() << "attack";

		m_canAttackTemp = m_canAttack;
		m_swordReachedPos = false; //set our sword reaching its position boolean to false

		m_canAttack = false; //make our bool false so we cannot attack again

		m_attackClock.restart(); //reset our attack clock

		//Setting our rotation of the sword to 0, this avoids our sword from slashing down if we attack while moving
		m_sword.getBody()->SetTransform(b2Vec2(m_sword.getBody()->GetPosition().x, m_forearmBody->GetPosition().y), 0);

		if(m_isFacingLeft)
			m_armPosDest = sf::Vector2f((m_forearmBody->GetPosition().x * PPM) + m_playerToArmJoint->GetLowerLimit() * PPM, m_forearmBody->GetPosition().y * PPM);
		else
			m_armPosDest = sf::Vector2f((m_forearmBody->GetPosition().x * PPM) + m_playerToArmJoint->GetUpperLimit() * PPM, m_forearmBody->GetPosition().y * PPM);
	}
}

void Player::throwSword()
{
	m_holdingSword = false;

	world.DestroyJoint(m_armToSwordJoint); //destroy the arm to sword joint

	m_armToSwordJoint = nullptr;

	if(m_isFacingLeft)
		m_sword.throwWeapon("Left");
	else
		m_sword.throwWeapon("Right");

	m_isAiming = false;
}

void Player::jump()
{
	m_playerBody->ApplyForceToCenter(b2Vec2(0, -5750),true);
	m_canJump = false;

	setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerJump"], sf::Vector2i(52, 87), 27.5f);
	m_animator.stop(); //stop any animation playing at the moment
	m_animator.play() << "jump";
}

void Player::pickupSword()
{
	//If we dont have our sword then play the pickup animation 
	if (false == m_holdingSword && false == m_playingPickup)
	{
		setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerPickup"], sf::Vector2i(42, 87), 23);
		m_animator.stop();
		m_animator.play() << "pickup";
		m_playingPickup = true;
		m_pickupClock.restart();//restart our stance clock
	}
}

void Player::handleJoystick(JoystickController & controller)
{
	bool moved = false;

	//Add the time gone since the last frame to our animation variables
	m_idleTime += m_animationClock.getElapsedTime().asSeconds();
	m_runTime += m_animationClock.getElapsedTime().asSeconds();

	//Set our Boolean to false
	m_isAiming = false;

	if (controller.isButtonHeld("LT") && m_holdingSword && m_canAttack && m_weaponPos != 0)
	{
		rotateWhileRunning(true);
		m_isAiming = true;
	}

	//If we are holding LT to aim and we have a sword then throw our sword
	if (controller.isButtonPressed("X") && m_isAiming && m_holdingSword)
		throwSword();

	if (controller.isButtonHeld("LeftThumbStickLeft") && m_canAttack)
	{
		if (m_runTime >= .7f && m_canJump)
		{
			setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerRun"], sf::Vector2i(63, 87), 36);
			m_animator.stop();
			m_animator.play() << "run";
			m_runTime = 0;
			m_idleTime = 0.5f;
		}
		moved = true;
		moveLeft();
		rotateWhileRunning(true);
	}
	if (controller.isButtonHeld("LeftThumbStickRight") && m_canAttack)
	{
		if (m_runTime >= .7f && m_canJump)
		{
			setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerRun"], sf::Vector2i(63, 87), 36);
			m_animator.stop();
			m_animator.play() << "run";
			m_runTime = 0;
			m_idleTime = 0.5f;
		}
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
		pickupSword();

	if (controller.isButtonPressed("A") && m_canJump)
		jump();

	if (moved == false)
	{
		if (m_idleTime >= 0.5f && m_canAttack && m_canJump && m_playingPickup == false)
		{
			m_idleTime = 0;
			setSpriteTexture(m_sprite, resourceManager.getTextureHolder()["playerIdle"], sf::Vector2i(42, 87), 24);
			m_animator.stop(); //stop any animation playing at the moment
			m_animator.play() << "idle";
		}
		m_runTime = .7f;
		rotateWhileRunning(false);
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_playerBody->SetLinearVelocity(b2Vec2(0, m_playerBody->GetLinearVelocity().y));
	}
}
void Player::invertPlayerJoint(bool facingLeft)
{
	//if we are facing right and we want to face left OR if we are facing left and we want to face right
	if (m_isFacingLeft == false && facingLeft || m_isFacingLeft && facingLeft == false)
	{
		//set is facing boolean here and also invoke our joint setter methods
		if (facingLeft)
		{
			setPlayerToArmJoint(-2.5f, 0, b2Vec2(15 / PPM, m_playerToArmJoint->GetLocalAnchorB().y));
			if (m_holdingSword) //if we are holding a sword then change its joint
				setArmToSwordJoint(0, 15, b2Vec2(28.5f / PPM, m_armToSwordJoint->GetLocalAnchorB().y));
			m_isFacingLeft = true;
		}
		else
		{
			setPlayerToArmJoint(0, 2.5f, b2Vec2(-15 / PPM, m_playerToArmJoint->GetLocalAnchorB().y));
			if(m_holdingSword)//if we are holding a sword then change its joint
				setArmToSwordJoint(-15, 0, b2Vec2(-28.5f / PPM, m_armToSwordJoint->GetLocalAnchorB().y));
			m_isFacingLeft = false;
		}

		m_sprite.setScale(m_sprite.getScale().x * -1, 1);
		if(m_holdingSword)
			m_sword.negateSword(); //negate the sprite scale of our sword
	}
}

void Player::changeSwordStance(std::string direction)
{
	if (direction == "Up" && m_switchedSwordPos == false)
	{
		//if we can move up and we are not already at the top stance of our player
		if (m_weaponPos < 2)
		{
			m_weaponPos++; //increase our pos

			//Invoke our method to change the y position of our arm and sword
			setSwordStance(m_weaponPosChange);
			m_switchedSwordPos = true;
			m_stanceChangeClock.restart(); //restart our stance clock
		}
	}
	else if (direction == "Down" && m_switchedSwordPos == false)
	{
		if (m_weaponPos > 0)
		{
			m_weaponPos--; //decrease our pos

			//Invoke our method to change the y position of our arm and sword
			setSwordStance(-m_weaponPosChange);
			m_switchedSwordPos = true;
			m_stanceChangeClock.restart();//restart our stance clock
		}
	}

}

void Player::swordClashed()
{
	//bounce our player away
	if (m_isFacingLeft)
		m_playerBody->ApplyForceToCenter(b2Vec2(150, 0), true);
	else
		m_playerBody->ApplyForceToCenter(b2Vec2(-150, 0), true);
}

void Player::checkCanAttack()
{
	//if the time elapsed since the clock was started/restarted is greater than or equal to our attack rate then set our bool
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
	if (m_holdingSword)
	{
		if (m_isAiming)
		{
			rotateSword(55, 20);
		}
		else if (rotate)
		{
			rotateSword(15, 10);
		}
		else
		{
			rotateSword(15, -10);
		}
	}
}

void Player::respawn()
{
	//Set all of our bodies to our start position and reset their velocities
	respawnBody(m_startPosition, m_playerBody);
	respawnBody(m_startPosition, m_forearmBody);
	respawnBody(m_startPosition, m_jumpBody);
	respawnBody(m_startPosition, m_leftSensorBody);
	respawnBody(m_startPosition, m_rightSensorBody);
	respawnBody(m_startPosition, m_sword.getBody());

	//Reset our booleans
	m_canAttack = true;
	m_canJump = false;
	m_canAttackTemp = true;
	m_swordReachedPos = false;
	m_respawn = false;
	m_isAiming = false;
	m_holdingSword = true;

	//Set the y position of our weapon to the middle of our player
	m_weaponPos = 1;

	//Reset our sword
	m_sword.respawn();

	//Setting our isFacing boolean annd also our sword joint
	if (m_startingDirection == "left")
	{
		m_sprite.setScale(-1, 1);
		setPlayerToArmJoint(-2.5f, 0, b2Vec2(15 / PPM, 18 / PPM));
		setArmToSwordJoint(0, 15, b2Vec2(28.5f / PPM, 0));
		m_sword.setSwordDirection("Left");
		m_isFacingLeft = true;
	}
	else
	{
		m_sprite.setScale(1, 1);
		setPlayerToArmJoint(0, 02.5f, b2Vec2(-15 / PPM, 18 / PPM));
		setArmToSwordJoint(-15, 0, b2Vec2(-28.5f / PPM, 0));
		m_sword.setSwordDirection("Right");
		m_isFacingLeft = false;
	}

	m_respawn = false;
}

void Player::respawnBody(b2Vec2 position, b2Body* body)
{
	body->SetTransform(position, body->GetAngle());
	body->SetLinearVelocity(b2Vec2(0, 0));
	body->SetAngularVelocity(0);
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

void Player::pickUpWeapon()
{
	m_sword.pickUp(); //invoke the pickup method in the sword to reset the sword values

	m_holdingSword = true;

	if (m_isFacingLeft)
	{
		m_sword.setSwordDirection("Left");
		setArmToSwordJoint(0, 15, b2Vec2(28.5f / PPM, 0));
	}
	else
	{
		m_sword.setSwordDirection("Right");
		setArmToSwordJoint(-15, 0, b2Vec2(-28.5f / PPM, 0));
	}
}

void Player::setArmToSwordJoint(float lowerAngle, float upperAngle, b2Vec2 anchorPos)
{
	//If we are holding a sword 
	if (m_holdingSword)
	{
		//get our arm to sword joint
		auto armToSword = m_armToSwordJointDef;

		//Set the upper and lower limits of our joint
		armToSword.lowerAngle = lowerAngle * DEG_TO_RAD;
		armToSword.upperAngle = upperAngle * DEG_TO_RAD;

		//Set the local ancjorB point for our joint
		armToSword.localAnchorB.Set(anchorPos.x, anchorPos.y);

		//If our arm to swor djoint is not a nullptr then delete the current joint from the world and recreate it below
		if (nullptr != m_armToSwordJoint)
		{
			//Only destroy and recreate our arm to sword joint if we are actualy holding a sword
			world.DestroyJoint(m_armToSwordJoint);
		}
		//assign our armToSwordDefinition our new definition
		m_armToSwordJointDef = armToSword;
		//Create the joint in the world so it is active
		m_armToSwordJoint = (b2RevoluteJoint*)world.CreateJoint(&m_armToSwordJointDef);
	}
}

void Player::setPlayerToArmJoint(float lowerLimit, float upperLimit, b2Vec2 anchorPos)
{
	//Seting parameters to our player to arm joint
	auto playerToArm = m_playerToArmJointDef;

	//Set the properties of our player to arm joint
	playerToArm.lowerTranslation = lowerLimit;
	playerToArm.upperTranslation = upperLimit;
	playerToArm.localAnchorB.Set(anchorPos.x, anchorPos.y);

	//If our joint is not nullptr then destroy the joint and recreate it below
	if(nullptr != m_playerToArmJoint)
		world.DestroyJoint(m_playerToArmJoint);

	m_playerToArmJointDef = playerToArm;
	m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);
}

void Player::setSwordStance(float posChange)
{
	//Seting parameters to our player to arm joint
	auto playerToArm = m_playerToArmJointDef;
	playerToArm.localAnchorB.Set(playerToArm.localAnchorB.x, playerToArm.localAnchorB.y + posChange);
	world.DestroyJoint(m_playerToArmJoint);
	m_playerToArmJointDef = playerToArm;
	m_playerToArmJoint = (b2PrismaticJoint*)world.CreateJoint(&m_playerToArmJointDef);
	if(m_holdingSword)
		//Set the position of the sword
		m_sword.getBody()->SetTransform(b2Vec2(m_sword.getBody()->GetPosition().x, m_sword.getBody()->GetPosition().y - (posChange / 2)), m_sword.getBody()->GetAngle());
}

void Player::rotateSword(float angle, float speed)
{
	if (m_isFacingLeft)
	{
		m_armToSwordJoint->SetLimits(0, angle * DEG_TO_RAD);
		m_sword.getBody()->SetAngularVelocity(speed);
	}
	else
	{
		m_armToSwordJoint->SetLimits(-angle * DEG_TO_RAD, 0);
		m_sword.getBody()->SetAngularVelocity(-speed);
	}
}

void Player::parried()
{
	//Destroy the arm to sword joint
	world.DestroyJoint(m_armToSwordJoint);

	m_holdingSword = false;
	m_armToSwordJoint = nullptr;

	m_sword.parried();

	m_parried = false;
}

void Player::setUpAnimations()
{
	m_animationClock.restart(); //starting our animation clock

	auto idleFrameSize = sf::Vector2i(42, 87);
	auto attackFrameSize = sf::Vector2i(56, 88);
	auto runFrameSize = sf::Vector2i(63, 87);
	auto jumpFrameSize = sf::Vector2i(52, 87);
	auto pickupFrameSize = sf::Vector2i(42, 87);

	//Adding all of our animations to our animation holder
	addFramesToAnimation(0.1f, 5, m_idleAnimation, idleFrameSize, "idle", .5f);
	addFramesToAnimation(0.1f, 10, m_attackAnimation, attackFrameSize, "attack", .35f);
	addFramesToAnimation(0.1f, 20, m_runAnimation, runFrameSize, "run", .7f);
	addFramesToAnimation(0.1f, 10, m_jumpAnimation, jumpFrameSize, "jump", .15f);
	addFramesToAnimation(0.1f, 20, m_pickupAnimation, pickupFrameSize, "pickup", 0.25f);
}

void Player::addFramesToAnimation(float lengthOfOneFrame, int numOfFrames, thor::FrameAnimation & animation, sf::Vector2i & frameSize, std::string animationName, float lengthOfAnimation)
{
	//loop for the amount of frames passed to the method
	for (int i = 0; i < numOfFrames; i++)
	{
		//add a frame to the animation each loop
		auto frame = sf::IntRect(0 + (frameSize.x * i), 0, frameSize.x, frameSize.y);
		animation.addFrame(lengthOfOneFrame, frame);
	}
	//add the animation to our animation holder with the specified length and name
	m_animationHolder.addAnimation(animationName, animation, sf::seconds(lengthOfAnimation));
}

void Player::setSpriteTexture(sf::Sprite& sprite, sf::Texture & texture, sf::Vector2i frameSize, float xOffset)
{
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0, frameSize.x, frameSize.y));
	sprite.setOrigin(sprite.getLocalBounds().left + xOffset, sprite.getLocalBounds().top + sprite.getLocalBounds().height / 2);
}

void Player::setColour(sf::Color color)
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

void Player::setCanJump(bool canJump)
{
	m_canJump = canJump;
}

void Player::setRespawn(bool respawn)
{
	m_respawn = respawn;

	if (m_respawn)
		m_respawnClock.restart();
}
void Player::setClashed(bool clashed)
{
	m_swordClashed = clashed;
	if (m_swordClashed)
	{
		m_swordReachedPos = true; //set our sword as reaching its position (incase we are attacking we dont want our sword to keep moving as it has hit a sword)
		m_forearmBody->SetLinearVelocity(b2Vec2(0, m_forearmBody->GetLinearVelocity().y));
		m_sword.getBody()->SetLinearVelocity(b2Vec2(0, m_sword.getBody()->GetLinearVelocity().y));
	}
}
void Player::setParried(bool parried)
{
	m_parried = parried;
}
void Player::setSwordThrown()
{
	m_sword.setSwordThrown();
}
void Player::setPickupWeapon()
{
	m_canPickupSword = true;
}
bool Player::getCanAttack()
{
	bool temp = m_canAttackTemp;
	m_canAttackTemp = false;
	return temp;
}

bool Player::holdingSword()
{
	return m_holdingSword;
}

bool Player::switchedWeaponPos()
{
	return m_switchedSwordPos;
}

sf::Sprite & Player::getLight()
{
	return m_lightSprite;
}

sf::Sprite & Player::getSwordLight()
{
	return m_sword.getLight();
}
