#pragma once
#include "Screen.h"
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

	//getters
	std::string getName() override;
private:
};