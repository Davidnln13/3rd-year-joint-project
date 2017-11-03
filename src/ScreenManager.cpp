#include "ScreenManager.h"

ScreenManager::ScreenManager() :
	m_playScreen("playScreen"),
	m_mainScreen("mainMenu"),
	m_currentScreen(&m_mainScreen) //temporary for now
{
	//asigning our screens to our map 
	screens[m_playScreen.getName()] = &m_playScreen;
	screens[m_mainScreen.getName()] = &m_mainScreen;
}

void ScreenManager::update()
{
	//update the current screen
	m_currentScreen->update();
}

void ScreenManager::render(sf::RenderWindow& window)
{
	//render the current screen
	m_currentScreen->render(window);
}

void ScreenManager::goToScreen(std::string screenName)
{
	auto temp = screens[screenName]; //get the screen pointer in the dictionary with the key 'screenName'

	if (temp != nullptr) 
	{
		m_currentScreen->end(); //stop the current screen

		m_currentScreen = temp; //assign the new screen

		m_currentScreen->start(); //start the new screen
	}
}

void ScreenManager::handleJoystick(JoystickController& controller1, JoystickController& controller)
{
	m_currentScreen->handleInput(controller1, controller);
}

std::string ScreenManager::getCurrentScreenName()
{
	return m_currentScreen->m_name;
}
