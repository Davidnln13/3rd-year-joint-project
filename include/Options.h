#pragma once
#include "Screen.h"

/// <summary>
/// Author: David Nolan
/// Date: 28/11/2017
/// Description: options screen to change volumes
/// </summary>
class Options : public Screen
{
public:
	Options(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	void selectButton(int newIndex);

	//getters
	std::string getName() override;

	//Setters
	void setIconSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);
	void setButtonText(OptionButton* btn, std::string text);
	void setButtonText(OptionButton& btn, int value, std::string conditionText, std::string text, sf::Font& font, sf::Font& conditionFont);
	void setPreviousScreen(std::string screen);

private:

	//Transition Variables
	float m_alpha;
	sf::Color m_transitionCol;
	sf::RectangleShape m_transitionRect;

	int* m_currentOption;
	std::map<OptionButton*, int*> m_optionMapper;

	Button m_back;
	std::map<std::string, Button*> m_buttons; //all buttons
	std::vector<Button*> m_btnList; //select buttons
	int m_btnIndex; //button index
	Audio& m_audioRef;
	std::string m_previousScreen;

	Label m_soundVolumeLabel;
	Label m_musicVolumeLabel;

	OptionButton* m_currentButton;
	OptionButton m_soundVolumeButton;
	OptionButton m_musicVolumeButton;

	std::map<std::string, OptionButton*> m_optionsButtons; 
	std::vector<OptionButton*> m_optionsbtnList; 
	int m_optionsbtnIndex; 
	bool m_optionsButtonPressed; 
};