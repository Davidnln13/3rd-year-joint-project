#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The screen is our base class, other screen classes will be derived from here
/// </summary>
class Screen
{
public:
	Screen() : m_active(false) {};
	virtual void update() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void start() = 0;
	virtual void end() = 0;

	//Getters
	virtual std::string getName() = 0;

	//these member variable should be available from the derived class
	bool m_active;
	std::string m_name;
};