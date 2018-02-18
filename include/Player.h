#pragma once
#include "Label.h"
#include "Weapon.h"
#include "Audio.h"

class JoystickController; //forward reference for our joystick class

/// <summary>
/// Author: David Nolan & Daryl Keogh
/// Date: 03/11/2017
/// Description: The player class is controlled by a user using a joystick (Xbox 360 or Xbox One Controller), the user can jump, attack and move.
/// </summary>

class Player
{
public:
	Player(Audio& audio, sf::Vector2f position, std::string direction);
	~Player(); //Any classes that have a box2d body should have that body deleted in here

	//This method creates a box2d body using the passed over arguments
	b2Body& createBody(b2Body* body, b2BodyType bodyType, b2Vec2 position, bool isBullet, bool fixRotation, float gravityScale);
	void createFixture(b2Body* body, float width, float height, bool isSensor, float density, float restitution, float friction);
	void createRectangle(sf::RectangleShape& rect, sf::Vector2f origin, sf::Color fill, sf::Color stroke, float strokeWidth);

	void update();
	void render(sf::RenderWindow& window);
	void moveRight();
	void moveLeft();
	void attack();
	void throwSword();
	void jump();
	void pickupSword();
	void handleJoystick(JoystickController& controller);
	void invertPlayerJoint(bool facingLeft); //inverts the joints for our player, so we can swap where the weapon and arm is jointed to
	void changeSwordStance(std::string direction); //changes the y position of our sword
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
	void playUIAnimations();
	void setUpAnimations(); //setups our animations for the player
	void addFramesToAnimation(float lengthOfOneFrame, int numOfFrames, thor::FrameAnimation& animation, sf::Vector2i& frameSize, std::string animationName, float lengthOfAnimation); //adds a specified amount of frames to an animation
	void addFramesToAnimation(float lengthOfOneFrame, int numOfFrames, thor::FrameAnimation & animation, thor::FrameAnimation & oppositeAnimation, sf::Vector2i & frameSize, std::string animationName, std::string oppositeAnimationName, float lengthOfAnimation);
	//we will use this to set the texture of our sprite so we can change between animations, we use the offset because the plaeyr will not always be in the center of the image on each animation
	void setSpriteTexture(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2i frameSize, float xOffset); 
	void setColour(sf::Color color); //set the colour of the player
	void spawnPlayer(float x, float y, bool facingLeft);
	void resetPlayerParameters(); //Resets our players parameters, Lives/kills

	//Getters
	Weapon* getWeapon();
	b2Body* getJumpBody();
	b2Body* getArmBody();
	b2Body* getPlayerBody();
	b2Body* getSwordBody();
	b2Body* getLeftSensorBody();
	b2Body* getRightSensorBody();
	bool getCanAttack();
	bool holdingSword();
	bool switchedWeaponPos();
	bool facingLeft();
	bool& dead();
	bool& hasFlag();
	int& lives();
	int& kills();
	int& captures();
	bool& respawning();
	sf::Vector2f position();
	sf::Vector2f& lastSpawnPos();
	sf::Sprite& getLight();
	sf::Sprite& getSwordLight();
	sf::RectangleShape& hitBox();

	//Setters
	void setCanJump(bool canJump);
	void setDead(bool isDead);
	void setClashed(bool clashed);
	void setParried(bool parried);
	void setCanMoveLeft(bool canMove);
	void setCanMoveRight(bool canMove);
	void setSwordThrown();
	void setPickupWeapon();
	void setSpawnPoint(sf::Vector2f& position, bool facingLeft);
	void setParameters(int killLimit);
	void increaseKills();
	//play audio from contact listener
	void playAudio(std::string soundName);
	void setHasFlag(bool has);
	void captureFlag();

private:
	bool m_canMoveLeft, m_canMoveRight; //Booleans to hold wheter we can move left or right
	bool m_isFacingLeft;
	bool m_canAttack;
	bool m_canAttackTemp;
	bool m_canJump;
	bool m_lostLife; //Bool to hold wheter the playe rhas lost a life or not
	bool m_playIconIn, m_playIconOut, m_playIcon, m_playScoreIcon;
	bool m_dead; //bool to show whether our player is dead or not
	bool m_swordReachedPos; //bool to show if our sword has reached its max position from th eplayer when they attack
	bool m_respawn; //bool to determine if we respawn our player or not
	bool m_isAiming; //bool to show if the player is holding LT to aim their sword to throw it
	bool m_holdingSword; //bool to show if the player is holding a sword or not
	bool m_swordClashed; // bool to set sword clash
	bool m_pickupSword; //bool to determine wheter to pickup a sword or not
	bool m_canPickupSword; //bool to determine wheter we CAN pickup a sword or not
	bool m_parried; //bool to determine if we have bene parried
	bool m_switchedSwordPos; //bool to show if we have switched sword positions on the y axis
	bool m_hasFlag;
	float m_moveSpeed; //the speed at which our player move shorizontally
	float m_gravityScale; //the scale of gravity on our bodies
	sf::Clock m_respawnClock; //Our clock to respawn the player, we will use this clock to put a timer between spawns

	//game specific parameters such as kill limit
	int m_lives;
	int m_kills;
	int m_ctfCaptures;

	std::string m_startingDirection; //temporary for now

	int m_weaponPos;
	float m_weaponPosChange;

	//Attack variables
	float m_attackRate;
	sf::Clock m_attackClock;
	sf::Clock m_stanceChangeClock;
	sf::Clock m_pickupClock;
	bool m_playingPickup; //bool to show that we are playing the pickup animation

	sf::Vector2f m_position, m_lastSpawnPos;
	b2Vec2 m_spawnPosition;
	sf::Vector2f m_armPosDest; //the destination of our arm position

	//Our rectangles to draw our player and forearm
	sf::RectangleShape m_playerRect, m_forearmRect, m_jumpRect, m_leftWallSensorRect, m_rightWallSensorRect;

	//Our weapon variable
	Weapon m_sword;

	//Box2D Variables
	b2Body* m_playerBody;
	b2Body* m_forearmBody;
	b2Body* m_jumpBody; //our jump body, we will use this body to determine if we can jump or not
	b2Body* m_leftSensorBody;
	b2Body* m_rightSensorBody;
	b2PrismaticJoint* m_playerToArmJoint; //the joint between our the players body and the players arm
	b2WeldJoint* m_jumpSensorJoint, *m_leftWallSensorJoint, *m_rightWallSensorJoint; //the joints between our jump and wall sensors and the player
	b2RevoluteJoint* m_armToSwordJoint; //the joitnt bewteen our players arm and the sword

	//We hold the definitions as variables so we can easily change joint definition variables on the fly without recreating them each time
	b2PrismaticJointDef m_playerToArmJointDef;
	b2WeldJointDef m_jumpSensorJointDef;
	b2RevoluteJointDef m_armToSwordJointDef;

	//Sprite variables
	sf::Sprite m_sprite;
	sf::Sprite m_iconSprite;
	sf::Sprite m_scorePlusSprite;
	sf::Sprite m_scoreMinusSprite;
	sf::Sprite m_lightSprite;

	//Animation variables
	sf::Clock m_animationClock, m_iconAnimationClock, m_iconLoopClock, m_scorePlusIconClock, m_scoreMinusIconClock; //our animation clocks, we will use this to update our animations
	thor::FrameAnimation m_idleAnimation, m_attackAnimation, m_runAnimation, m_jumpAnimation, m_pickupAnimation, m_iconAnimationIn, m_iconAnimationOut;
	thor::FrameAnimation m_plusScoreAnimation, m_minusScoreAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	thor::Animator<sf::Sprite, std::string> m_animator, m_iconAnimator, m_plusScoreAnimator, m_minusScoreAnimator;
	float m_idleTime; //holds the amount of time gone since we last played our idle animation
	float m_runTime; //holds the amount of time gone since we last played our run animation

	//Shaders
	sf::Shader m_recolourShader;

	//Our icon labels
	Label m_killLabel;
	Label m_livesLabel;

	//Const variables
	float const RAD_TO_DEG;
	float const DEG_TO_RAD;
	
	//A reference to our audio
		Audio& m_audioRef;
};

#include "JoystickController.h"