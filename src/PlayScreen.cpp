#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name, Audio& audio) :
	Screen(name),
	m_audioPlayScreen(audio),
	m_level(audio, 1)
{
	
}

void PlayScreen::update()
{
	//update our level
	m_level.update();
}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	m_level.render(window);
}

void PlayScreen::start()
{
	m_audioPlayScreen.updateMusic("Game");
	m_active = true;
}

void PlayScreen::end()
{
	m_active = false;
}

std::string PlayScreen::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name;

	//Handle input in the level
	currentScreen = m_level.handleInput(controller1, controller2);

	//if either player has pressed tsart then pause the game
	if (controller1.isButtonPressed("Start") || controller2.isButtonPressed("Start"))
		currentScreen = "pause"; //go to pause screen

	return currentScreen;
}

std::string PlayScreen::getName()
{
	return m_name;
}

void PlayScreen::setLevel(int maxKills, int maxTime, int levelNumber)
{
	m_level.setLevelParameters(maxKills, maxTime, levelNumber);
}
