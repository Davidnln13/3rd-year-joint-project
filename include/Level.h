#pragma once
#include "LevelLoader.h"
#include "Player.h"
#include "Obstacle.h"
#include "ContactListener.h"
#include "Flag.h"
#include "Audio.h"

/// <summary>
/// Author: Daryl Keogh
/// Date: 14/11/2017
/// Description: The level class is responsible for creating and updating the player and game objects during a level
/// We will load in data from a file to determine what objects to load, where to put them and also keep track of where the spawn points are in the map
/// </summary>

class Level
{
public:
	Level(Audio& audio, int levelNum);

	void update();
	void render(sf::RenderWindow& window);
	void drawToOverlay(sf::Sprite sprite);
	void playAnimation();
	void setUpAnimation();
	void checkFlagPickup(Player&  player, Flag& flag, sf::Clock& flagClock);
	void checkFlagReset(Player&  player1, Player&  player2, Flag& flag, sf::RectangleShape& base);
	void checkFlagCapture(Player& player, Flag& flag, sf::RectangleShape& base);

	b2Body* createKillBox(float x, float y, float w, float h);
	std::string handleInput(JoystickController& controller1, JoystickController& controller2);
	void setUpLevel(std::string levelName);
	void checkForRespawn(Player& deadPlayer, Player& otherPlayer);
	void setupAnimations(std::string levelName);
	void setLevelParameters(int maxKills, int maxTime, int levelNumber, bool ctf, std::map<int, std::string>& levelNames);
	float distance(sf::Vector2f a, sf::Vector2f b);
	float lerpValue(float a, float b, float t);

	void clearLevel();

	//Getters
	bool gameOver();
	float gameOverTime();

	//helper function
	sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t);
private:
	//Variables for starting our match
	bool m_isCtf; //Bool to hold wheter the game mod eis capture the flag or not
	bool m_setView = false;
	bool m_gameOver;
	bool m_hasTimeLimit;
	bool m_hasKillLimit;
	bool m_playGameOverIndicator;
	bool m_setWinner;
	bool m_hasWinner;
	int m_levelNumber;
	int m_killLimit;
	int m_timeLimit;
	float m_transitionAlpha;
	sf::Clock m_timeLimitClock, m_gameOverClock, m_transitionClock;
	sf::RectangleShape m_transitionRect;
	sf::Color m_transitionCol;

	//Ctf variables
	Flag m_yellowFlag, m_blueFlag;
	sf::Clock m_blueFlagClock, m_yellowFlagClock;
	sf::RectangleShape m_yellowBase, m_blueBase;

	sf::View m_testView, m_bottomView, m_topView;
	std::vector<sf::View*> m_viewVector;

	//Our level background sprite
	sf::Sprite m_bg;
	sf::Sprite m_overlay;
	sf::RenderTexture m_overlayTexture;

	//Animation variables
	sf::Clock m_winAnimationClock, m_loseAnimationClock, m_draw1AnimationClock, m_draw2AnimationClock;
	thor::FrameAnimation m_winAnimation, m_loseAnimation, m_drawAnimation;
	thor::Animator<sf::Sprite, std::string> m_winAnimator, m_loseAnimator, m_draw1Animator, m_draw2Animator;
	sf::Sprite m_winSprite, m_loseSprite, m_draw1Sprite, m_draw2Sprite;

	//Torch variables
	std::vector<sf::Sprite> m_torchSprites, m_torchLightSprites;
	std::vector<sf::Clock> m_torchClocks, m_torchLightClocks;//our animation clocks, we will use this to update our animations
	sf::Clock m_loopClock; 
	thor::FrameAnimation m_torchAnimation, m_torchLightAnimation;
	thor::AnimationMap<sf::Sprite, std::string> m_animationHolder;//our select and deselect animations
	std::vector<thor::Animator<sf::Sprite, std::string>*> m_torchAnimators, m_torchLightAnimators;

	//Obstacle variables
	std::vector<Obstacle> m_floors;
	//Our floors are made up of several tiles, therefore we need a collection to hold our tiles
	std::vector<sf::Sprite> m_floorSprites;

	//Our player objects
	Player m_player1;
	Player m_player2;
	std::vector<Player*> m_players;

	//Our window sprites
	std::vector<sf::Sprite> m_windowSprites;

	//Our spawn points
	std::vector<sf::Vector2f> m_spawnPoints;

	//Our walls
	std::vector<sf::Sprite> m_wallSprites;
	std::vector<Obstacle> m_walls;

	//Our contact listener, we will use this class to handle collisions with sensors
	ContactListener m_contactListener;

	//A reference to our audio
	Audio& m_audioRef;

	//Our level loader 
	LevelLoader m_levelLoader;

	std::vector<b2Body*> m_killBoxes;

	std::vector<Label*> m_labels;
	Label m_timeLabel, m_timeLabelValue;
	Label m_p1CapLabel, m_p2CapLabel;
};