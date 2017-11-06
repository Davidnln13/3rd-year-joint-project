#include "MainMenu.h"

MainMenu::MainMenu(std::string name, Audio& audio) :
	Screen(),
	m_playGameBtn(sf::Vector2f(640, 144), "play game")
{
	m_name = name;
}

void MainMenu::update()
{

}

void MainMenu::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Red);
	m_playGameBtn.render(window); //draw our button
}

void MainMenu::start()
{
	m_active = true;
}

void MainMenu::end()
{
	m_active = false;
}

void MainMenu::handleInput(JoystickController& controller1, JoystickController& controller2)
{
}

std::string MainMenu::getName()
{
	return m_name;
}