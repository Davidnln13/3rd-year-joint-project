#include "ScreenManager.h"

ScreenManager::ScreenManager() :
	m_playScreen("play game", m_audio),
	m_pauseScreen("pause", m_audio, m_playScreen),
	m_mainScreen("main menu", m_audio),
	m_helpScreen("help", m_audio),
	m_optionsScreen("options",m_audio),
	m_preGameScreen("pre game", m_audio),
	m_closeWindow(false)
{
	//asigning our screens to our map 
	screens[m_pauseScreen.getName()] = &m_pauseScreen;
	screens[m_playScreen.getName()] = &m_playScreen;
	screens[m_mainScreen.getName()] = &m_mainScreen;
	screens[m_helpScreen.getName()] = &m_helpScreen;
	screens[m_optionsScreen.getName()] = &m_optionsScreen;
	screens[m_preGameScreen.getName()] = &m_preGameScreen;
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

	std::string currentScreenName;

	if(nullptr != m_currentScreen)
		currentScreenName = m_currentScreen->getName();

	if (temp != nullptr) 
	{
		//if our current screen ptr points to something then stop the screen
		if (m_currentScreen != nullptr)
		{
			m_currentScreen->end(); //stop the current screen
		}

		m_currentScreen = temp; //assign the new screen

		m_currentScreen->start(); //start the new screen

		//setPreviousScreen("help", Help, currentScreenName);
		if (m_currentScreen->getName() == "help")
		{
			auto tempHelp = dynamic_cast<Help*>(m_currentScreen);
			tempHelp->setPreviousScreen(currentScreenName);
		}
		if (m_currentScreen->getName() == "options")
		{
			auto tempOptions= dynamic_cast<Options*>(m_currentScreen);
			tempOptions->setPreviousScreen(currentScreenName);
		}
		
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