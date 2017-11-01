#include "MainMenu.h"

MainMenu::MainMenu(std::string name) :
	Screen()
{
	m_name = name;
}

void MainMenu::update()
{

}

void MainMenu::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Red);
}

void MainMenu::start()
{
	m_active = true;
}

void MainMenu::end()
{
	m_active = false;
}

void MainMenu::handleInput()
{
}

std::string MainMenu::getName()
{
	return m_name;
}