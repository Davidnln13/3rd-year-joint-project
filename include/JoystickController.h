#pragma once
#include "SFML/Window/Joystick.hpp" //Access to the Joystick api

///Author: Daryl Keogh
///Date: 01/11/17
///Class Description: The controller class is used to handle input from a connected
///Xbox 360 or Xbox one controller and determine what buttons/axis have been pressed and/or moved.
///Bugs: No Known bugs

//Our GamepPadState struct, will be used for determinging what buttons are pressed on the controller
struct GamePadState
{
	bool A = false; //button No. 0
	bool B = false;	//button No. 1
	bool X = false;	//button No. 2
	bool Y = false;	//button No. 3
	bool Start = false; //button No. 7

	bool RightThumbClick = false; //button No. 9
	bool LeftThumbClick = false;

	bool DpadUp = false; //Axis PovY
	bool DpadDown = false; //Axis PovY
	bool DpadRight = false; //Axis PovX
	bool DpadLeft = false; //Axis PovX

	bool LeftThumbStickUp = false; //Axis Y
	bool LeftThumbStickDown = false; //Axis Y
	bool LeftThumbStickRight = false; //Axis X
	bool LeftThumbStickLeft = false; //Axis X

	bool RT = false; //Axis Z
	bool LT = false; //Axis Z

	bool RB = false; //button 5
	bool LB = false; //button 4
};

class JoystickController
{
public: 
	JoystickController(int index);

	void update();

	void handleInput();

	void resetStates(); //resets the state of our gamepad

	float getTriggerValue() const;
	float getStickValue() const;

	//Our getters for our gamepad states
	GamePadState& getCurrent();
	GamePadState& getPrevious();
private:
	//Our gamepad states
	GamePadState m_currentPadState;
	GamePadState m_previousPadState;
	GamePadState m_defaultPadState;

	//Deadzone for our thumbsticks/dpad/triggers
	const int dpadDeadzone = 65;
	const int stickDeadzone = 15;
	const int triggerDeadzone = 15;

	float m_triggerAxis; //holds the value of our triggers axis (axisZ)
	float m_stickValue; //holds the value of our stick axis (left thumbstick)

	int m_joystickIndex; //holds the index of the joustick this instance will update/control
};