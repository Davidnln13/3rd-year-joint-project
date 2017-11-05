#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name) :
	Screen(),
	m_player1(sf::Vector2f(1280 - ( 1280 / 8.f), 720.f - ( 720.f / 4)), sf::Vector2f(25, 75 ), "left"),
	m_player2(sf::Vector2f(1280 / 8.f, 720.f - (720.f / 4)), sf::Vector2f(25, 75), "right"),
	m_floor(sf::Vector2f(1280, 35), sf::Vector2f(1280 / 2.f, 720 - (35 / 2)))
{
	m_name = name;
	world.SetContactListener(&m_contactListener);
	m_contactListener.setPlayers(m_player1, m_player2);
}

void PlayScreen::update()
{
	//update our players
	m_player1.update();
	m_player2.update();
}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	m_player1.render(window); //draw the first player	
	m_player2.render(window); //draw the second player

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

void PlayScreen::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	m_player1.handleJoystick(controller1);
	m_player2.handleJoystick(controller2);
}

std::string PlayScreen::getName()
{
	return m_name;
}