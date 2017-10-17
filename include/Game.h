#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>

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

	//Box 2D variables
	b2Vec2 gravity;
	b2World m_world; //our world variable, we can add forces to this such as gravity so it affects all b2 bodies
};