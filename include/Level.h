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
	void handleInput(JoystickController& controller1, JoystickController& controller2);
	void setUpFloor();

	void setupAnimations();

private:
	int m_levelNumber;

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

	//Our player objects
	Player m_player1;
	Player m_player2;

	//Our floors are made up of sever tiles, therefore we need a collection to hold our tiles
	std::vector<sf::Sprite> m_floorSprites;

	//Our contact listener, we will use this class to handle collisions with sensors
	ContactListener m_contactListener;

	//A reference to our audio
	Audio& m_audioRef;

	//Our level loader 
	LevelLoader m_levelLoader;
};