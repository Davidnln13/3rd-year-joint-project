#pragma once
#include "Screen.h"
#include "Level.h"

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The play screen will be our main play screen, in here we will update the players and handle any game specific events
/// </summary>
class PlayScreen : public Screen
{
public:
	PlayScreen(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;
	Audio& m_audioPlayScreen;
	//getters
	std::string getName() override;

	void setLevel(int maxKills, int maxTime, int levelNumber);
private:
	Level m_level;
};