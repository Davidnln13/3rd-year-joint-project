#include "Game.h"

//Globals need
static sf::Int32 MS_PER_UPDATE = 10; //updates per millisecond
float PPM = 30.0f; //Pixels Per Metre, scalar for use with box2D
static b2Vec2 GRAVITY = b2Vec2(0.0f, 9.8f);
b2World world = b2World(GRAVITY); //our world variable, we can add forces to this such as gravity so it affects all b2 bodies
ResourceManager resourceManager;

/// <summary>
/// Author: Daryl Keogh & David Nolan
/// Date: 17/10/2017
/// Description: The game class is the basis for our game, we will create any needed objects and will control the main game loop from here
/// </summary>

Game::Game() :
	m_window(sf::VideoMode(1280, 720), "David & Daryl Year 3 Project"),
	m_screenManager(),
	m_controller1(0), //creating our first controller with index 0, so the first controller connected to the pc will be controller 1/player 1
	m_controller2(1)
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

		//if we are on the play screen and the game isnt paused then simiulate the box2d world physics
		if (m_screenManager.getCurrentScreenName() == "play game"
			&& m_screenManager.playScreen().paused() == false)
			world.Step(1 / 60.0f, 8, 3); //simulates the world


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
		//if the veent is a close event then close out window
		if (event.type == sf::Event::Closed)
			m_window.close();
	}
}

void Game::update()
{
	if (m_screenManager.getCurrentScreenName() == "play game")
	{	
		//Update our second player controller
		m_controller2.update();

		//check what buttons are pressed 
		m_controller2.handleInput();
	}

	m_controller1.update(); //update the first controller
	m_controller1.handleInput(); //get input from the first controller
	//pass over the joysticks to the screen manager
	m_screenManager.handleJoystick(m_controller1, m_controller2);

	m_screenManager.update();
}

void Game::render()
{
	m_window.clear();

	m_screenManager.render(m_window);

	//display anything drawn to the window since the last time we called clear
	m_window.display();
}