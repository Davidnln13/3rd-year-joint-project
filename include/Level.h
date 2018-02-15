#pragma once
#include "LevelLoader.h"
#include "Player.h"
#include "Obstacle.h"
#include "ContactListener.h"
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

	b2Body* createKillBox(float x, float y, float w, float h);


	std::string handleInput(JoystickController& controller1, JoystickController& controller2);
	void setUpLevel(std::string levelName);

	void checkForRespawn(Player& deadPlayer, Player& otherPlayer);
	void setupAnimations(std::string levelName);
	void setLevelParameters(int maxKills, int maxTime, int levelNumber, bool ctf, std::map<int, std::string>& levelNames);
	float distance(sf::Vector2f a, sf::Vector2f b);

	void clearLevel();

	//helper function
	sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float t);
private:
	//Variables for starting our match
	bool m_isCtf; //Bool to hold wheter the game mod eis capture the flag or not
	bool m_gameOver;
	bool m_hasTimeLimit;
	bool m_hasKillLimit;
	int m_levelNumber;
	int m_killLimit;
	int m_timeLimit;
	sf::Clock m_timeLimitClock;

	//Our level background sprite
	sf::Sprite m_bg;
	sf::Sprite m_overlaySprite;
	sf::RenderTexture m_overlayTexture;

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

	//Our window sprites
	std::vector<sf::Sprite> m_windowSprites;

	//Our spawn points
	std::vector<sf::Vector2f> m_spawnPoints;

	//Our contact listener, we will use this class to handle collisions with sensors
	ContactListener m_contactListener;

	//A reference to our audio
	Audio& m_audioRef;

	//Our level loader 
	LevelLoader m_levelLoader;

	std::vector<b2Body*> m_killBoxes;

	std::vector<Label*> m_labels;
	Label m_timeLabel, m_timeLabelValue;
};