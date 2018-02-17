#pragma once
#include "Screen.h"
#include "Level.h"

/// <summary>
/// Author: Daryl keogh
/// Date: 25/10/2017
/// Description: The play screen will be our main play screen, in here we will update the players and handle any game specific events
/// </summary>
class PlayScreen : public Screen
{
public:
	Audio & m_audioPlayScreen;

	PlayScreen(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	void selectButton(int newIndex);

	//getters
	std::string getName() override;
	bool paused();
	bool gameOver();
	float gameOverTime();

	void setLevel(int maxKills, int maxTime, int levelNumber, bool ctf, std::map<int, std::string>& levelNames);
private:
	Level m_level;
	bool m_paused;

	//Our buttons for our pause screen
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