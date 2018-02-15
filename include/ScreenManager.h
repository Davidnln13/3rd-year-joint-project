#pragma once
#include <map>
#include "MainMenu.h";
#include "PlayScreen.h"
#include "PauseScreen.h"
#include "Help.h"
#include "Options.h"
#include "PreGame.h"

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
	PlayScreen& playScreen();

private:
	//Declare our different screens here
	Options m_optionsScreen;
	PlayScreen m_playScreen;
	PreGameScreen m_preGameScreen;
	MainMenu m_mainScreen;
	Help m_helpScreen;
	Audio m_audio;
	Screen* m_currentScreen;
	bool m_closeWindow; //boolean which will be used to close the game/window
	std::map<std::string, Screen*> screens; //a dictionary containing a string and a pointer to a screen
};
