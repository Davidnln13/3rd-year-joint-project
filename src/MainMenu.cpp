#include "MainMenu.h"

MainMenu::MainMenu(std::string name, Audio& audio) :
	Screen(),
	m_playGameBtn(sf::Vector2f(640, 144), "play game"),
	m_optionsBtn(sf::Vector2f(640, 288), "options"),
	m_helpBtn(sf::Vector2f(640, 432), "help"),
	m_exitBtn(sf::Vector2f(640, 576), "exit")
{
	m_name = name;

	//adding our buttons to our buttons map
	m_buttons[m_playGameBtn.getName()] = &m_playGameBtn; 
	m_buttons[m_optionsBtn.getName()] = &m_optionsBtn;
	m_buttons[m_helpBtn.getName()] = &m_helpBtn;
	m_buttons[m_exitBtn.getName()] = &m_exitBtn;
}

void MainMenu::update()
{

}

void MainMenu::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->render(window);

}

void MainMenu::start()
{
	m_active = true;
}

void MainMenu::end()
{
	m_active = false;
}

std::string MainMenu::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "mainMenu"
	//if we press start return a string with the value "playScreen"
	if (controller1.isButtonPressed("Start"))
	{
		currentScreen = "playScreen";
	}

	return currentScreen;
}

std::string MainMenu::getName()
{
	return m_name;
}