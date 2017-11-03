#pragma once
#include "SFML/Window/Joystick.hpp" //Access to the Joystick api
#include <map> //allows us to use std::map

///Author: Daryl Keogh
///Date: 01/11/17
///Class Description: The controller class is used to handle input from a connected
///Xbox 360 or Xbox one controller and determine what buttons/axis have been pressed and/or moved.
///Bugs: No Known bugs

class JoystickController
{
public: 
	JoystickController(int index);

	void update();

	void handleInput();

	bool isButtonPressed(std::string buttonName); //checks if a button was pressed once
	bool isButtonHeld(std::string buttonName); //Checks if a button is held down on the controller

	//our getters for our triggers and thumbsticks
	float getTriggerValue() const;
	float getStickValue() const;

private:
	//Our maps, we will use these to hold the state of our gamepad
	std::map<std::string, bool> m_currentState;
	std::map<std::string, bool> m_previousState;
	std::map<std::string, bool> m_defualtState;

	//Deadzone for our thumbsticks/dpad/triggers
	const int dpadDeadzone = 65;
	const int stickDeadzone = 55;
	const int triggerDeadzone = 15;

	float m_triggerAxis; //holds the value of our triggers axis (axisZ)
	float m_stickValue; //holds the value of our stick axis (left thumbstick)

	int m_joystickIndex; //holds the index of the joystick this instance will update/control
};