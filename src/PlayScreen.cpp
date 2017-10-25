#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name) :
	Screen()
{
	m_name = name;
}

void PlayScreen::update()
{

}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Blue);
}

void PlayScreen::start()
{
	m_active = true;
}

void PlayScreen::end()
{
	m_active = false;
}

std::string PlayScreen::getName()
{
	return m_name;
}