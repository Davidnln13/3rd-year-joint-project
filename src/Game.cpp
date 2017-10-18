#include "Game.h"

//updates per millisecond
static sf::Int32 MS_PER_UPDATE = 10;
static int METRES_TO_PIXELS = 30; //for use with box2D

/// <summary>
/// Startup project, will have all the necessary project settings done and ready to go
/// for Box2D and Thor. I have set up the project in such a way that does not require
/// any external installation.
/// Author: Daryl keogh
/// Date: 17/10/2017
/// </summary>

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "David & Daryl year 3 Project"),
	gravity(0.0f, 9.8f),
	m_world(gravity) //adding gravity to the world
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

void Game::processEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		//handle keyboard input here
	}
}

void Game::update()
{
	//update game entities here
}

void Game::render()
{
	m_window.clear(sf::Color::Blue);

	//display anything drawn to the window since the last time we called clear
	m_window.display();
}