#include "Options.h"

Options::Options(std::string name, Audio& audio) :
	Screen(name),
	m_back(sf::Vector2f(640, 676), "Back", "Arrow Icon"),
	m_btnIndex(0),
	m_previousScreen(name),
	m_soundVolumeLabel("Sound", sf::Vector2f(640,200), resourceManager.getFontHolder()["oxinFont"]),
	m_musicVolumeLabel("Music", sf::Vector2f(640, 400), resourceManager.getFontHolder()["oxinFont"])
{
	//adding our buttons to our buttons map
	m_buttons[m_back.getName()] = &m_back;
	m_btnList.push_back(&m_back);
}

void Options::update()
{
	//loop through our buttons map and update each one
	for (auto key : m_buttons)
		key.second->update();
}

void Options::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->render(window);
	m_soundVolumeLabel.draw(window);
	m_musicVolumeLabel.draw(window);
}

void Options::start()
{
	m_active = true;

	//Selects the first button in our list as the currently selected button
	selectButton(0);
}

void Options::end()
{
	m_active = false;
}

std::string Options::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "mainMenu"

	if (controller1.isButtonPressed("A"))
		currentScreen = m_previousScreen;

	return currentScreen;
}

void Options::selectButton(int newIndex)
{
	m_btnList.at(m_btnIndex)->deSelect(); //deselect the current button

	m_btnIndex = newIndex; //reassign the button index to the new index

	m_btnList.at(m_btnIndex)->select(); //select the button at the new index

}

std::string Options::getName()
{
	return m_name;
}

void Options::setPreviousScreen(std::string screen)
{
	m_previousScreen = screen;
}
