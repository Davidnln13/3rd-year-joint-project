#include "Options.h"

Options::Options(std::string name, Audio& audio) :
	Screen(name),
	m_previousScreen(name),
	m_soundVolumeLabel("Sound", sf::Vector2f(640, 200), resourceManager.getFontHolder()["oxinFont"]),
	m_musicVolumeLabel("Music", sf::Vector2f(640, 400), resourceManager.getFontHolder()["oxinFont"]),
	m_audioRef(audio),
	m_soundSlider("soundSlider", sf::Vector2f(440,240), sf::IntRect(0, 0, 357, 50)),
	m_musicSlider("musicSlider", sf::Vector2f(440,440), sf::IntRect(0, 0, 357, 50)),
	m_backLabel("back", sf::Vector2f(1180, 683), resourceManager.getFontHolder()["oxinFont"])
{
	setIconSprite(m_bIcon, resourceManager.getTextureHolder()["B Icon"], sf::Vector2f(1141, 683));
}

void Options::update()
{
	for (auto& key : m_audioRef.m_soundMap)
	{
		m_audioRef.m_soundMap[key.first].setVolume(m_soundSlider.getSliderLevel());
	}
	for (auto& key : m_audioRef.m_musicMap)
	{
		m_audioRef.m_musicMap[key.first].setVolume(m_musicSlider.getSliderLevel());
	}
	m_soundSlider.update();
    m_musicSlider.update();
}
void Options::setIconSprite(sf::Sprite & sprite, sf::Texture& texture, sf::Vector2f position)
{
	sprite.setTexture(texture);
	sprite.setOrigin(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f);
	sprite.setPosition(position);
}
void Options::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);
	m_soundVolumeLabel.draw(window);
	m_musicVolumeLabel.draw(window);
	m_soundSlider.draw(window);
	m_musicSlider.draw(window);
	window.draw(m_bIcon);
	m_backLabel.draw(window);
}

void Options::start()
{
	m_active = true;
}

void Options::end()
{
	m_active = false;
}

std::string Options::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "mainMenu"

	if (controller1.isButtonPressed("B"))
	{
		currentScreen = m_previousScreen;
		m_audioRef.m_soundMap["SelectMenuItem"].play();
	}
	if (controller1.isButtonPressed("B"))
	{
		m_musicSlider.moveUp();
		//currentScreen = m_previousScreen;
		m_audioRef.m_soundMap["SelectMenuItem"].play();
	}
	if (controller1.isButtonPressed("Y"))
	{
		m_musicSlider.moveDown();
		m_audioRef.m_soundMap["SelectMenuItem"].play();
	}

	return currentScreen;
}

std::string Options::getName()
{
	return m_name;
}

void Options::setPreviousScreen(std::string screen)
{
	m_previousScreen = screen;
}
