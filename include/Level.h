#pragma once
#include "Player.h"
#include "Boundary.h"
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
	Level(Audio& audio);

	void update();
	void render(sf::RenderWindow& window);
	void handleInput(JoystickController& controller1, JoystickController& controller2);

private:
	//Our player objects
	Player m_player1;
	Player m_player2;

	//since we will have levels, then boundary objects will be in the level and possibly the player too
	Boundary m_floor;

	//Our contact listener, we will use this class to hadnle collisions with sensors
	ContactListener m_contactListener;

	//A reference to our audio
	Audio& m_audioPlayScreen;
};