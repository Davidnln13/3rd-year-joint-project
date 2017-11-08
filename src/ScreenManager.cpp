#include "ScreenManager.h"

ScreenManager::ScreenManager() :
	m_pauseScreen("pause", m_audio),
	m_playScreen("play game", m_audio),
	m_mainScreen("main menu", m_audio),
	m_closeWindow(false)
{
	//asigning our screens to our map 
	screens[m_pauseScreen.getName()] = &m_pauseScreen;
	screens[m_playScreen.getName()] = &m_playScreen;
	screens[m_mainScreen.getName()] = &m_mainScreen;

	goToScreen("main menu"); //go to the main menu
}

void ScreenManager::update()
{
	//update the current screen
	m_currentScreen->update();
}

void ScreenManager::render(sf::RenderWindow& window)
{
	//If our boolean is true, exit the game
	if(m_closeWindow)
		window.close();

	//render the current screen
	m_currentScreen->render(window);
}

void ScreenManager::goToScreen(std::string screenName)
{
	auto temp = screens[screenName]; //get the screen pointer in the dictionary with the key 'screenName'

	if (temp != nullptr) 
	{
		//if our current screen ptr points to something then stop the screen
		if (m_currentScreen != nullptr)
		{
			m_currentScreen->end(); //stop the current screen
		}

		m_currentScreen = temp; //assign the new screen

		m_currentScreen->start(); //start the new screen
	}
}

void ScreenManager::handleJoystick(JoystickController& controller1, JoystickController& controller2)
{
	auto screen = m_currentScreen->handleInput(controller1, controller2);
	//if the returned string is different to our current screens name then change the screen
	if (screen != m_currentScreen->getName())
	{
		if (screen == "exit")
			m_closeWindow = true;
		else
			goToScreen(screen);
	}
}

std::string ScreenManager::getCurrentScreenName()
{
	return m_currentScreen->m_name;
}