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
	m_xLabel("Attack or Throw primed sword", sf::Vector2f(640, 450), resourceManager.getFontHolder()["oxinFont"]),
	m_audioRef(audio)
{
	//adding our buttons to our buttons map
	m_buttons[m_back.getName()] = &m_back;
	m_btnList.push_back(&m_back);

	setIconSprite(m_ltIcon, resourceManager.getTextureHolder()["LT Icon"], sf::Vector2f(450, 70));
	setIconSprite(m_lAnalogIcon, resourceManager.getTextureHolder()["Lanalog Icon"], sf::Vector2f(450, 170));
	setIconSprite(m_dpadIcon, resourceManager.getTextureHolder()["Dpad Icon"], sf::Vector2f(450, 270));
	setIconSprite(m_yIcon, resourceManager.getTextureHolder()["Y Icon"], sf::Vector2f(450, 470));
	setIconSprite(m_aIcon, resourceManager.getTextureHolder()["A Icon"], sf::Vector2f(450, 370));
	setIconSprite(m_xIcon, resourceManager.getTextureHolder()["X Icon"], sf::Vector2f(450, 570));
	
	m_ltLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 50));
	m_lAnalogLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 150));
	m_dpadLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 250));
	m_aLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 350));
	m_yLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 450));
	m_xLabel.setOriginAndPosition(sf::Vector2f(0, 0), sf::Vector2f(500, 550));
	
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
	window.draw(m_lAnalogIcon);
	window.draw(m_dpadIcon);
	window.draw(m_aIcon);
	window.draw(m_yIcon);
	window.draw(m_xIcon);

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

	if (controller1.isButtonPressed("A") || controller1.isButtonPressed("B"))
	{
		currentScreen = m_previousScreen;
		m_audioRef.m_soundMap["SelectMenuItem"].play();
	}

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
