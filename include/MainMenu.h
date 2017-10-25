#pragma once
#include "Screen.h"

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The screen is our main menu, in here we can navigate to other menus using the joystick
/// </summary>
class MainMenu : public Screen
{
public:
	MainMenu(std::string name);
	
	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;

	//getters
	std::string getName() override;

private:
};