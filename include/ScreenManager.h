#pragma once
#include "Screen.h"
#include "PlayScreen.h"
#include "MainMenu.h"
#include <map>

class ScreenManager
{
public:
	ScreenManager();

	void update();

	void render(sf::RenderWindow& window);

	void goToScreen(std::string screenName);

private:
	//Declare our different screens here
	PlayScreen m_playScreen;
	MainMenu m_mainScreen;

	Screen* m_currentScreen;
	std::map<std::string, Screen*> screens; //a dictionary containing a string and a pointer to a screen
};