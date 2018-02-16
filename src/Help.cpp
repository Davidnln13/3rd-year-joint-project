#include "Help.h"

Help::Help(std::string name, Audio& audio) :
	Screen(name),
	m_back(sf::Vector2f(640, 676), "Back", "Arrow Icon"),
	m_btnIndex(0),
	m_previousScreen(name),
	m_ltLabel("Prime sword to throw", sf::Vector2f(640, 200), resourceManager.getFontHolder()["oxinFont"]),
	m_lAnalogLabel("Move or Change Sword Height", sf::Vector2f(640, 250), resourceManager.getFontHolder()["oxinFont"]),
	m_dpadLabel("Change Sword Height", sf::Vector2f(640, 300), resourceManager.getFontHolder()["oxinFont"]),
	m_aLabel("Jump", sf::Vector2f(640, 350), resourceManager.getFontHolder()["oxinFont"]),
	m_yLabel("Pickup Sword", sf::Vector2f(640, 400), resourceManager.getFontHolder()["oxinFont"]),
	m_xLabel("Attack", sf::Vector2f(640, 450), resourceManager.getFontHolder()["oxinFont"])
{
	//adding our buttons to our buttons map
	m_buttons[m_back.getName()] = &m_back;
	m_btnList.push_back(&m_back);
	setIconSprite(m_ltIcon, resourceManager.getTextureHolder()["LT Icon"], sf::Vector2f(600, 200));

	//sf::Sprite m_ltIcon;
	//sf::Sprite m_lAnalogIcon;
	//sf::Sprite m_dpadIcon;
	//sf::Sprite m_yIcon;
	//sf::Sprite m_xIcon;
	//sf::Sprite m_aIcon;
}

void Help::update()
{
	//loop through our buttons map and update each one
	for (auto key : m_buttons)
		key.second->update();
}

void Help::setIconSprite(sf::Sprite & sprite, sf::Texture& texture, sf::Vector2f position)
{
	sprite.setTexture(texture);
	sprite.setOrigin(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f);
	sprite.setPosition(position);
}
void Help::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->render(window);
	window.draw(m_ltIcon);
	m_ltLabel.draw(window);
	m_lAnalogLabel.draw(window);
	m_dpadLabel.draw(window);
	m_aLabel.draw(window);
	m_yLabel.draw(window);
	m_xLabel.draw(window);

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
