#include "Game.h"

static sf::Int32 MS_PER_UPDATE = 10; //updates per millisecond
static int METRES_TO_PIXELS = 30; //for use with box2D

/// <summary>
/// Author: Daryl Keogh & David Nolan
/// Date: 17/10/2017
/// Description: The game class is the basis for our game, we will create any needed objects and will control the main game loop from here
/// </summary>

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "David & Daryl Year 3 Project"),
	gravity(0.0f, 9.8f),
	m_world(gravity), //adding gravity to the world
	screenManager()
{
	init();
}

void Game::init()
{
	m_window.setFramerateLimit(60); //limit the game to 60 frames per second
}

void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	sf::Time dt;
	while (m_window.isOpen())
	{
		processEvents();
		dt = clock.restart();
		lag += dt.asMilliseconds();

		m_world.Step(1 / 60.0f, 8, 3); //simulates the world


		while (lag > MS_PER_UPDATE)
		{
			update();
			lag -= MS_PER_UPDATE;
		}

		update();
		render();

	}
}

//in here we will update the joystick and key handler objects so we can handle input
void Game::processEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		if (event.KeyReleased) //if the current event is a keyReleased event
		{
			//if the space key is pressed go to the play screen
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			{
				screenManager.goToScreen("playScreen");
			}
		}

	}
}

void Game::update()
{
	screenManager.update();
}

void Game::render()
{
	m_window.clear();

	screenManager.render(m_window);

	//display anything drawn to the window since the last time we called clear
	m_window.display();
}