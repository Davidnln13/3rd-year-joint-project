#pragma once
#include "Weapon.h"

class JoystickController; //forward reference for our joystick class

/// <summary>
/// Author: David Nolan & Daryl Keogh
/// Date: 03/11/2017
/// Description: The player class is controlled by a user using a joystick (Xbox 360 or Xbox One Controller), the user can jump, attack and move.
/// </summary>

class Player
{
public:
	Player(sf::Vector2f position, std::string direction);
	~Player(); //Any classes that have a box2d body should have that body deleted in here

	void update();
	void render(sf::RenderWindow& window);
	void moveRight();
	void moveLeft();
	void attack();
	void throwSword();
	void jump();
	void handleJoystick(JoystickController& controller);
	void invertPlayerJoint(bool facingLeft); //inverts the joints for our player, so we can swap where the weapon and arm is jointed to
	void changeSwordStance(std::string direction); //changes teh position our sword is will be in the y position
	void swordClashed(); //method is invoked when two swords clash with one another
	void checkCanAttack(); //checks if we can attack again
	void applyArmPushBack(); //aplies a force on our arm so we push it back into place
	void rotateWhileRunning(bool rotate); //rotates our arm and sword while running
	void respawn(); //respawns the player at the position of m_startPosition, this needs to change so the position can be dynamic
	void respawnBody(b2Vec2 position, b2Body* body); //resets our players bodies position and velocity so they do not start with extra forces applied to them
	bool distance(sf::Vector2f point1, sf::Vector2f point2, float distanceCuttOff);
	void pickUpWeapon();
	void setArmToSwordJoint(float lowerAngle, float upperAngle, b2Vec2 anchorPos);
	void setPlayerToArmJoint(float lowerLimit, float upperLimit, b2Vec2 anchorPos);
	void setSwordStance(float posChange); //change sthe y position of our arm local to the player, this will be used to switch the heights we hold our sword at
	void rotateSword(float angle, float speed); //sets the limits of the rotation of our sword and then set the speed of the rotation
	void parried();
	void setUpAnimations(); //setups our animations for the player
	void setSpriteTexture(sf::Texture& texture, sf::Vector2i frameSize); //we will use this to set the texture of our sprite so we can change between animations

	//Getters
	b2Body* getJumpBody();
	b2Body* getArmBody();
	b2Body* getPlayerBody();
	b2Body* getSwordBody();
	bool getCanAttack();
	bool holdingSword();
	bool switchedWeaponPos();

	//Setters
	void setCanJump(bool canJump);
	void setRespawn(bool respawn);
	void setClashed(bool clashed);
	void setParried(bool parried);
	void setSwordThrown();
	void setPickupWeapon();

private:
	bool m_isFacingLeft;
	bool m_canAttack;
	bool m_canAttackTemp;
	bool m_canJump;
	bool m_swordReachedPos; //bool to show if our sword has reached its max position from th eplayer when they attack
	bool m_respawn; //bool to determine if we respawn our player or not
	bool m_isAiming; //bool to show if the player is holding LT to aim their sword to throw it
	bool m_holdingSword; //bool to show if the player is holding a sword or not
	bool m_swordClashed; // bool to set sword clash
	bool m_pickupSword; //bool to determine whteer to pickup a sword or not
	bool m_parried; //bool to determine if we have bene parried
	bool m_switchedSwordPos; //bool to show if we have switched sword positions on the y axis
	float m_moveSpeed; //the speed at which our player move shorizontally
	float m_gravityScale; //the scale of gravity on our bodies

	std::string m_startingDirection; //temporary for now

	int m_weaponPos;
	float m_weaponPosChange;

	//Attack variables
	float m_attackRate;
	sf::Clock m_attackClock;
	sf::Clock m_stanceChangeClock;

	sf::Vector2f m_position;
	b2Vec2 m_startPosition;
	sf::Vector2f m_armPosDest; //the destination of our arm position

	//Our rectangles to draw our player and forearm
	sf::RectangleShape m_playerRect;
	sf::RectangleShape m_forearmRect;
	sf::RectangleShape m_jumpRect;

	//Our weapon variable
	Weapon m_sword;

	//Box2D Variables
	b2Body* m_playerBody;
	b2Body* m_forearmBody;
	b2Body* m_jumpBody; //our jump body, we will use this body to determine if we can jump or not
	b2PrismaticJoint* m_playerToArmJoint; //the joint between our the players body and the players arm
	b2PrismaticJoint* m_jumpSensorJoint; //the joint between our jump sensor and the player
	b2RevoluteJoint* m_armToSwordJoint; //the joitnt bewteen our players arm and the sword

	//We hold the definitions as variables so we can easily change joint definition variables on the fly without recreating them each time
	b2PrismaticJointDef m_playerToArmJointDef;
	b2PrismaticJointDef m_jumpSensorJointDef;
	b2RevoluteJointDef m_armToSwordJointDef;

	//Our body defs, we will hold a reference to our bodie defeinitions so we can easily modify the bodies on the fly
	b2BodyDef m_playerbodyDef;
	b2BodyDef m_forearmBodyDef;

	//Sprite variables
	sf::Sprite m_sprite;

	//Animation variables
	sf::Clock m_animationClock; //our animation clock, we will use this to update our animations
	thor::FrameAnimation m_idleAnimation, m_attackAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	thor::Animator<sf::Sprite, std::string> m_animator;
	float m_idleTime; //holds teh amount of time gone since we last played our idle animation

	//Const variables
	float const RAD_TO_DEG;
	float const DEG_TO_RAD;
};

#include "JoystickController.h"