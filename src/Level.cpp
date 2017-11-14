#include "Level.h"

Level::Level(Audio& audio) :
	m_player1(sf::Vector2f(1280 - (1280 / 8.f), 720.f - (720.f / 4)), sf::Vector2f(25, 75), "left"),
	m_player2(sf::Vector2f(1280 / 8.f, 720.f - (720.f / 4)), sf::Vector2f(25, 75), "right"),
	m_floor(sf::Vector2f(1280, 35), sf::Vector2f(1280 / 2.f, 720 - (35 / 2))),
	m_audioPlayScreen(audio)
{
	//Set up the contact listener for box2d
	world.SetContactListener(&m_contactListener);
	//Set pointers to our player objects in our contact listener
	m_contactListener.setPlayers(m_player1, m_player2);
}

void Level::update()
{
	//update our players
	m_player1.update();
	m_player2.update();
}

void Level::render(sf::RenderWindow & window)
{
	m_player1.render(window); //draw the first player	
	m_player2.render(window); //draw the second player

	m_floor.render(window); //draw the floor
}

void Level::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	m_player1.handleJoystick(controller1);
	m_player2.handleJoystick(controller2);


	if ((controller1.isButtonPressed("X") && m_player1.getCanAttack() == true) || (controller2.isButtonPressed("X") && m_player2.getCanAttack() == true))
	{
		m_audioPlayScreen.m_soundArray[0].play();
	}
}
