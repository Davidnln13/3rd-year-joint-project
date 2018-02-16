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
	void setPreviousScreen(std::string screen);
private:
	Button m_back;
	std::map<std::string, Button*> m_buttons; //all buttons
	std::vector<Button*> m_btnList; //select buttons
	int m_btnIndex; //button index
	Audio& m_audioRef;
	std::string m_previousScreen;
	Label m_soundVolumeLabel;
	Label m_musicVolumeLabel;
};