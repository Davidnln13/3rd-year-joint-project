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


	//getters
	std::string getName() override;

	//Setters
	void setPreviousScreen(std::string screen);
	void setIconSprite(sf::Sprite& sprite, sf::Texture& texture, sf::Vector2f position);

private:
	Audio& m_audioRef;
	std::string m_previousScreen;

	Label m_soundVolumeLabel;
	Label m_musicVolumeLabel;
	Label m_backLabel;

	Slider m_soundSlider;
	Slider m_musicSlider;

	sf::Sprite m_bIcon;
};