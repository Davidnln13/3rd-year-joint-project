#pragma once
#include <map>
#include "MainMenu.h";
#include "PlayScreen.h"

class PlayScreen;
class MainMenu;

class ScreenManager
{
public:
	ScreenManager();

	void update();

	void render(sf::RenderWindow& window);

	void goToScreen(std::string screenName);

	void handleJoystick(JoystickController& controller1, JoystickController& controller2);

	//Getters
	std::string getCurrentScreenName();

private:
	//Declare our different screens here
	PlayScreen m_playScreen;
	MainMenu m_mainScreen;
	Audio m_audio;
	Screen* m_currentScreen;

	std::map<std::string, Screen*> screens; //a dictionary containing a string and a pointer to a screen
};