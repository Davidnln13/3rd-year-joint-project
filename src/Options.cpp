#include "Options.h"

Options::Options(std::string name, Audio& audio) :
	Screen(name),
	m_previousScreen(name),
	m_soundVolumeLabel("Sound", sf::Vector2f(640, 200), resourceManager.getFontHolder()["oxinFont"]),
	m_musicVolumeLabel("Music", sf::Vector2f(640, 400), resourceManager.getFontHolder()["oxinFont"]),
	m_audioRef(audio),
	m_soundSlider("soundSlider", sf::Vector2f(440,240), sf::IntRect(0, 0, 357, 50)),
	m_musicSlider("musicSlider", sf::Vector2f(440,440), sf::IntRect(0, 0, 357, 50)),
	m_backLabel("back", sf::Vector2f(1180, 683), resourceManager.getFontHolder()["oxinFont"]),
	m_sliderIndex(0),
	m_navigationLabel("navigation and volume change", sf::Vector2f(300,683), resourceManager.getFontHolder()["oxinFont"])
{
	setIconSprite(m_navigationIcon, resourceManager.getTextureHolder()["Lanalog Icon"], sf::Vector2f(70, 683));

	m_highlight.setTextureRect(sf::IntRect(4152, 0, 410, 60));
	m_highlight.setTexture(resourceManager.getTextureHolder()["Button Selected"]);
	m_highlight.setPosition(435, 240);
	m_highlight.setScale(sf::Vector2f(0.98, 1));

	setIconSprite(m_bIcon, resourceManager.getTextureHolder()["B Icon"], sf::Vector2f(1141, 683));
	setIconSprite(m_soundIcon, resourceManager.getTextureHolder()["Sound Icon"], sf::Vector2f(808,267.5));
	setIconSprite(m_musicIcon, resourceManager.getTextureHolder()["Music Icon"], sf::Vector2f(808, 467.5));

	m_sliders[m_soundSlider.getName()] = &m_soundSlider;
	m_sliders[m_musicSlider.getName()] = &m_musicSlider;

	m_sliderList.push_back(&m_soundSlider);
	m_sliderList.push_back(&m_musicSlider);
}

void Options::update()
{
	for (auto key : m_sliders)
		key.second->update();
	for (auto& key : m_audioRef.m_soundMap)
	{
		m_audioRef.m_soundMap[key.first].setVolume(m_soundSlider.getSliderLevel());
	}
	for (auto& key : m_audioRef.m_musicMap)
	{
		m_audioRef.m_musicMap[key.first].setVolume(m_musicSlider.getSliderLevel());
	}
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
	m_navigationLabel.draw(window);
	m_musicVolumeLabel.draw(window);
	for (auto key : m_sliders)
		key.second->draw(window);
	window.draw(m_bIcon);
	window.draw(m_soundIcon);
	window.draw(m_musicIcon);
	window.draw(m_navigationIcon);
	window.draw(m_highlight);
	m_backLabel.draw(window);
}

void Options::start()
{
	m_highlight.setPosition(sf::Vector2f(435, 240));
	selectSlider(0);
	m_active = true;
}

void Options::end()
{
	m_active = false;
}

std::string Options::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	bool navigated = false;
	//assing the new index the same value as our current index
	auto newIndex = m_sliderIndex;

	auto currentScreen = m_name; //the current screen we are on is m_name ie. "mainMenu"

	if (controller1.isButtonPressed("B"))
	{
		m_audioRef.m_soundMap["SelectMenuItem"].play();
		currentScreen = m_previousScreen;
	}
	if (controller1.isButtonPressed("LeftThumbStickLeft") || controller1.isButtonPressed("DpadLeft"))
	{
		m_audioRef.m_soundMap["SelectMenuItem"].play();
		m_sliderList.at(m_sliderIndex)->moveDown();
	}
	if (controller1.isButtonPressed("LeftThumbStickRight") || controller1.isButtonPressed("DpadRight"))
	{
		m_audioRef.m_soundMap["SelectMenuItem"].play();
		m_sliderList.at(m_sliderIndex)->moveUp();
	}
	if (controller1.isButtonPressed("LeftThumbStickUp") || controller1.isButtonPressed("DpadUp"))
	{
		moveHighlight();
		m_audioRef.m_soundMap["MoveMenu"].play();
		navigated = true;
		newIndex--;
	}
	if (controller1.isButtonPressed("LeftThumbStickDown") || controller1.isButtonPressed("DpadDown"))
	{
		moveHighlight();
		m_audioRef.m_soundMap["MoveMenu"].play();
		navigated = true;
		newIndex++;
	}

	//if we have navigated through the menu then changed buttons
	if (navigated)
	{
		//checking if our newIndex has gone out of bounds
		if (newIndex > 1)
			newIndex = 0;
		else if (newIndex < 0)
			newIndex = 1;

		selectSlider(newIndex); //focus our button at index: newIndex
	}


	return currentScreen;
}

void Options::moveHighlight()
{
	if (m_highlight.getPosition() == sf::Vector2f(435, 240))
	{
		m_highlight.setPosition(sf::Vector2f(435, 440));
	}
	else
	{
		m_highlight.setPosition(sf::Vector2f(435, 240));
	}
}

void Options::selectSlider(int newIndex)
{
	if (newIndex < 0 || newIndex > 1)
	{
		std::cout << newIndex + " is not within range of the vector: m_sliderList" << std::endl;
	}
	else
	{
		m_sliderIndex= newIndex; //reassign the button index to the new index
	}
}

std::string Options::getName()
{
	return m_name;
}

void Options::setPreviousScreen(std::string screen)
{
	m_previousScreen = screen;
}
