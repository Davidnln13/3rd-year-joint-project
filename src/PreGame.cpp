#include "PreGame.h"

PreGameScreen::PreGameScreen(std::string name, Audio & audio):
	Screen(name)
{
}

void PreGameScreen::update()
{
}

void PreGameScreen::render(sf::RenderWindow & window)
{
}

void PreGameScreen::start()
{
	m_active = true;
}

void PreGameScreen::end()
{
	m_active = false;
}

std::string PreGameScreen::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "PreGame"

	return currentScreen;
}

std::string PreGameScreen::getName()
{
	return m_name;
}
