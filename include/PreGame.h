#pragma once
#include "Screen.h"


//We will use this struct to control our game mode parameters
static struct GameMode
{
public:
	GameMode(); //Our constructor
	//Declaring our variables
	std::string name;
	static int gameModeNum;
	static int levelNum;
	static int killLimit;
	static int timeLimit;
	static int maxLevels;
	static int maxGameModes;
	static bool isCtf; //Boolean to determine wheter the gameode is ctf or not

	//Declaring our maps
	std::map<int, std::string> gameModes;
	static std::map<int, std::string> levelNames;
};

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

	//Setters
	void setIconSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);
	void setButtonText(OptionButton* btn, std::string text);
	void setButtonText(OptionButton& btn, int value, std::string conditionText, std::string text, sf::Font& font, sf::Font& conditionFont);

	//getters
	std::string getName() override;

	//Our struct variables
	GameMode gameMode;
private:
	int* m_currentOption;
	std::map<OptionButton*, int*> m_optionMapper; //this allows us to change the options on our pre game screen by mapping options to certain buttons

	//Transition Variables
	float m_alpha;
	sf::Color m_transitionCol;
	sf::RectangleShape m_transitionRect;

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