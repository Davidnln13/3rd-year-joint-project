#pragma once
#include "Screen.h"

/// <summary>
/// Author: David Nolan
/// Date: 28/11/2017
/// Description: Help screen to display controls
/// </summary>
class Help : public Screen
{
public:
	Help(std::string name, Audio& audio);

	void update() override;
	void render(sf::RenderWindow& window) override;
	void start() override;
	void setIconSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);
	void end() override;
	std::string handleInput(JoystickController& controller1, JoystickController& controller2) override;

	void selectButton(int newIndex);

	//getters
	std::string getName() override;

	//Setters
	void setPreviousScreen(std::string screen);
private:
	Button m_back;
	std::map<std::string, Button*> m_buttons; //all buttons
	std::vector<Button*> m_btnList; //select buttons
	int m_btnIndex; //button index
	std::string m_previousScreen;

	sf::Sprite m_ltIcon;
	sf::Sprite m_lAnalogIcon;
	sf::Sprite m_dpadIcon;
	sf::Sprite m_yIcon;
	sf::Sprite m_xIcon;
	sf::Sprite m_aIcon;

	Label m_ltLabel;
	Label m_lAnalogLabel;
	Label m_dpadLabel;
	Label m_yLabel;
	Label m_xLabel;
	Label m_aLabel;
};