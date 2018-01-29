#pragma once
#include "Screen.h"

/// <summary>
/// Author: Daryl Keogh
/// Date: 29/01/2018
/// Description: Pre game screen to configure the game to the players desired choices
/// </summary>

class PreGameScreen : public Screen
{
public:
	PreGameScreen(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	//getters
	std::string getName() override;
private:
};