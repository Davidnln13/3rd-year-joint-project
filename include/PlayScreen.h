#pragma once
#include "Screen.h"
#include "Player.h"
#include "Boundary.h"

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The play screen will be our main play screen, in here we will update the players and handle any game specific events
/// </summary>
class PlayScreen : public Screen
{
public:
	PlayScreen(std::string name);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	void handleInput(JoystickController& controller1, JoystickController& controller) override;

	//getters
	std::string getName() override;
private:
	Player m_player1;
	//since we will have levels, then boundary objects will be in the level and possibly the player too
	Boundary m_floor;
};