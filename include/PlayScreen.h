#pragma once
#include "Screen.h"
#include "Player.h"
#include "Boundary.h"
#include "ContactListener.h"

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

	//getters
	std::string getName() override;
private:
	Player m_player1;
	Player m_player2;
	Audio& m_audioPlayScreen;
	//since we will have levels, then boundary objects will be in the level and possibly the player too
	Boundary m_floor;
	ContactListener m_contactListener;
};