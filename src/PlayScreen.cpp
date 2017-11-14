#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name, Audio& audio) :
	Screen(name),
	m_level(audio, resourceManager.getTextureHolder()["castleBG"])
{

}

void PlayScreen::update()
{
	m_level.update();
}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	m_level.render(window);
}

void PlayScreen::start()
{
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
	m_level.handleInput(controller1, controller2);

	//if either player has pressed tsart then pause the game
	if (controller1.isButtonPressed("Start") || controller2.isButtonPressed("Start"))
		currentScreen = "pause"; //go to pause screen

	return currentScreen;
}

std::string PlayScreen::getName()
{
	return m_name;
}