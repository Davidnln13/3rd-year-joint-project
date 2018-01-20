#pragma once
#include "Screen.h"

/// <summary>
/// Author: Daryl keogh
/// Date: 08/11/2017
/// Description: The screen is our pause screen, players can go to this screen when they press the start button during the play screen
/// </summary>

class PauseScreen : public Screen
{
public:
	PauseScreen(std::string name, Audio& audio, Screen& playScreen);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	void selectButton(int newIndex);

	//getters
	std::string getName() override;

private:
	//Our buttons for our screen
	Button m_continueBtn;
	Button m_optionsBtn;
	Button m_helpBtn;
	Button m_mainMenuBtn;
	Screen* m_playScreen; //a pointer to our play screen, we will use this to draw the player screen in the background during the pause screen
	sf::RectangleShape m_overlay; //the white overlay on our pause screen
	std::map<std::string, Button*> m_buttons; //our map of buttons
	std::vector<Button*> m_btnList; //we will use this to select/deselect buttons in our menu
	int m_btnIndex; //the index of the button we are currently focused on
};