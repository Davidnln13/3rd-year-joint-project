#include "Help.h"

Help::Help(std::string name, Audio& audio) :
	Screen(name),
	m_back(sf::Vector2f(640, 676), "Back", "Back Icon"),
	m_btnIndex(0),
	m_previousScreen(name)
{
	m_controllerSprite.setTexture(resourceManager.getTextureHolder()["Controller"]);
	m_controllerSprite.setPosition(-150, -200);
	//adding our buttons to our buttons map
	m_buttons[m_back.getName()] = &m_back;
	m_btnList.push_back(&m_back);
}

void Help::update()
{
	//loop through our buttons map and update each one
	for (auto key : m_buttons)
		key.second->update();
}

void Help::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
	window.draw(m_controllerSprite);
	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->render(window);

}

void Help::start()
{
	m_active = true;

	//Selects the first button in our list as the currently selected button
	selectButton(0);
}

void Help::end()
{
	m_active = false;
}

std::string Help::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "mainMenu"

	if (controller1.isButtonPressed("A"))
		currentScreen = m_previousScreen;

	return currentScreen;
}

void Help::selectButton(int newIndex)
{	
		m_btnList.at(m_btnIndex)->deSelect(); //deselect the current button

		m_btnIndex = newIndex; //reassign the button index to the new index

		m_btnList.at(m_btnIndex)->select(); //select the button at the new index

}

std::string Help::getName()
{
	return m_name;
}

void Help::setPreviousScreen(std::string screen)
{
	m_previousScreen = screen;
}
