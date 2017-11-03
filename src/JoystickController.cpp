#include "JoystickController.h"

JoystickController::JoystickController(int index = 0):
	m_joystickIndex(index)
{
	m_currentState["A"] = false;
	m_currentState["B"] = false;
	m_currentState["X"] = false;
	m_currentState["Y"] = false;
	m_currentState["Start"] = false;
	m_currentState["RightThumbClick"] = false;
	m_currentState["LeftThumbClick"] = false;
	m_currentState["DpadUp"] = false;
	m_currentState["DpadDown"] = false;
	m_currentState["DpadRight"] = false;
	m_currentState["DpadLeft"] = false;
	m_currentState["LeftThumbStickUp"] = false;
	m_currentState["LeftThumbStickDown"] = false;
	m_currentState["LeftThumbStickRight"] = false;
	m_currentState["LeftThumbStickLeft"] = false;
	m_currentState["RT"] = false;
	m_currentState["LT"] = false;
	m_currentState["RB"] = false;
	m_currentState["LB"] = false;

	m_previousState = m_currentState;
	m_defualtState = m_currentState;
}

void JoystickController::update()
{
	//Updates connected controller
	sf::Joystick::update();
}

void JoystickController::handleInput()
{
	m_previousState = m_currentState; //set the previous state as the current
	m_currentState = m_defualtState; //Sets our current state to the default state (setting all bools to false)

	//Get the position of our dpad axis : PovY & PovX
	float povY = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovY);
	float povX = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::PovX);

	//gets the position of our left thumbstick
	m_stickValue = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::X);
	float axisY = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Y);

	m_triggerAxis = sf::Joystick::getAxisPosition(m_joystickIndex, sf::Joystick::Axis::Z);

	//asigning our bools to our current gamepadstate: Checking face Buttons
	m_currentState["A"] = sf::Joystick::isButtonPressed(m_joystickIndex, 0);
	m_currentState["B"] = sf::Joystick::isButtonPressed(m_joystickIndex, 1);
	m_currentState["X"] = sf::Joystick::isButtonPressed(m_joystickIndex, 2);
	m_currentState["Y"] = sf::Joystick::isButtonPressed(m_joystickIndex, 3);

	m_currentState["LB"] = sf::Joystick::isButtonPressed(m_joystickIndex, 4);
	m_currentState["RB"] = sf::Joystick::isButtonPressed(m_joystickIndex, 5);

	m_currentState["RightThumbClick"] = sf::Joystick::isButtonPressed(m_joystickIndex, 9);
	m_currentState["LeftThumbClick"] = sf::Joystick::isButtonPressed(m_joystickIndex, 8);

	//Checking if the start button has been pressed
	m_currentState["Start"] = sf::Joystick::isButtonPressed(m_joystickIndex, 7);

	//Checking if the dpad has been hit
	if (povY < -dpadDeadzone)
		m_currentState["DpadDown"] = true;
	else if (povY > dpadDeadzone)
		m_currentState["DpadUp"] = true;
	else if (povX > dpadDeadzone)
		m_currentState["DpadRight"] = true;
	else if (povX < -dpadDeadzone)
		m_currentState["DpadLeft"] = true;

	//Check if our thumbstick has been moved
	if (axisY < -dpadDeadzone)
		m_currentState["LeftThumbStickUp"] = true;
	else if (axisY > dpadDeadzone)
		m_currentState["LeftThumbStickDown"] = true;
	else if (m_stickValue > dpadDeadzone)
		m_currentState["LeftThumbStickRight"] = true;
	else if (m_stickValue < -dpadDeadzone)
		m_currentState["LeftThumbStickLeft"] = true;

	if (m_triggerAxis > triggerDeadzone)
		m_currentState["LT"] = true;
	else if(m_triggerAxis < -triggerDeadzone)
		m_currentState["RT"] = true;
}

bool JoystickController::isButtonPressed(std::string buttonName)
{
	if (m_currentState[buttonName] && m_previousState[buttonName] == false)
		return true;

	return false;
}

float JoystickController::getTriggerValue() const
{
	return m_triggerAxis;
}
float JoystickController::getStickValue() const
{
	return m_stickValue;
}