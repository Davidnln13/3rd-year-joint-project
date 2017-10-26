#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name) :
	Screen(),
	m_player1(),
	m_floor(sf::Vector2f(1280, 35), sf::Vector2f(1280 / 2.f, 720 - (35 / 2)))
{
	m_name = name;
}

void PlayScreen::update()
{

}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::Blue);
	m_player1.render(window); //draw the player
	m_floor.render(window); //draw the floor
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