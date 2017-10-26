#pragma once
#include <iostream>
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>
#include <Thor\Math.hpp>
#include "ScreenManager.h"

/// <summary>
/// Startup project, will have all the necessary project settings done and ready to go
/// for Box2D and Thor. I have set up th eproject in such a way that does not require
/// any external installation.
/// Author: Daryl keogh
/// Date: 17/10/2017
/// </summary>

//our game class is responsible for our game loop
class Game
{
public:
	Game();
	void run();
	void init();
	void processEvents();

private:
	void update();
	void render();

	sf::RenderWindow m_window;

	ScreenManager screenManager;

	//Box 2D variables
	b2Vec2 gravity;
	
};