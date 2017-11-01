#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name) :
	Screen(),
	m_player1(sf::Vector2f(25, 75)),
	m_floor(sf::Vector2f(1280, 35), sf::Vector2f(1280 / 2.f, 720 - (35 / 2)))
{
	m_name = name;
}

void PlayScreen::update()
{
	m_player1.update();
}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
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

void PlayScreen::handleInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_player1.moveLeft();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_player1.moveRight();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
		m_player1.attack();
}

std::string PlayScreen::getName()
{
	return m_name;
}