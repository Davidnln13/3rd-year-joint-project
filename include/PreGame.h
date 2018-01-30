#pragma once
#include "Screen.h"

/// <summary>
/// Author: Daryl Keogh
/// Date: 29/01/2018
/// Description: Pre game screen to configure the game to the players desired choices
/// </summary>

class PreGameScreen : public Screen
{
public:
	PreGameScreen(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	void selectButton(int newIndex);
	void setButtonText(OptionButton* btn, std::string text);

	void setIconSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);

	//getters
	std::string getName() override;

	//Our options variables
	static int m_killLimit;
	static int m_timeLimit;
	static int m_gameMode;
	static int m_map;
private:
	int* m_currentOption;
	std::map<OptionButton*, int*> m_optionMapper; //this allows us to change the options on our pre game screen by mapping options to certain buttons

	//Our buttons
	OptionButton* m_currentButton;
	OptionButton m_gameModeBtn;
	OptionButton m_timeLimitBtn;
	OptionButton m_killLimitBtn;
	OptionButton m_mapBtn;
	std::map<std::string, OptionButton*> m_buttons; //our map of buttons
	std::vector<OptionButton*> m_btnList; //we will use this to select/deselect buttons in our menu
	int m_btnIndex; //the index of the button we are currently focused on
	bool m_buttonPressed; //bool to hold wheter we have pressed on a button or not

	//Our navigation icons
	sf::Sprite m_backIcon;
	sf::Sprite m_startIcon;
	Label m_backLabel;
	Label m_startLabel;
};