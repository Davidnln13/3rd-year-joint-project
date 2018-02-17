#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Vector2Helper.h"
#include "Audio.h"
#include "ResourceManager.h"
#include "Button.h"
#include "OptionButton.h"
#include "Slider.h"
#include "JoystickController.h"

//Creating a method that outputs a string to the console
//This eleminates the need to write cout/endl every time
static void printLog(std::string text)
{
	std::cout << text << std::endl;
}

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The screen is our base class, other screen classes will be derived from here
/// </summary>

class Screen
{
public:
	Screen(std::string name) : m_active(false), m_name(name) {};
	virtual void update() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual std::string handleInput(JoystickController& controller1, JoystickController& controller2) = 0;
	virtual void start() = 0;
	virtual void end() = 0;

	//Getters
	virtual std::string getName() = 0;

	//these member variable should be available from the derived class
	bool m_active;
	std::string m_name;

	//Our static methods, we declare them here so that any screens that use similar methods can invoke this
	static float lerpValue(float a, float b, float t)
	{
		//If our values are not the same
		if (a != b)
		{
			if (a > b)
			{
				a -= t; //Reduce A by T
				if (a < b) //If A is now less than B, set A to equal to B
					a = b;
			}
			else if (a < b)
			{
				a += t; //Increase A by T
				if (a > b) //If A is now greater than B, set A to equal to B
					a = b;
			}
		}

		return a;
	};
};