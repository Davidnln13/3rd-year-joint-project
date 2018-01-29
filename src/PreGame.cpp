#include "PreGame.h"

PreGameScreen::PreGameScreen(std::string name, Audio & audio) :
	Screen(name),
	m_gameModeBtn(sf::Vector2f(640, 144), "deathmatch", resourceManager.getFontHolder()["oxinFont"], "Options Icon"),
	m_mapBtn(sf::Vector2f(640, 288), "castle", resourceManager.getFontHolder()["oxinFont"], "Map Icon"),
	m_killLimitBtn(sf::Vector2f(640, 432), "kill limit", resourceManager.getFontHolder()["arialFont"], "Kill Icon"),
	m_timeLimitBtn(sf::Vector2f(640, 576), "time limit", resourceManager.getFontHolder()["arialFont"], "Timer Icon"),
	m_buttonPressed(false),
	m_killLimit(5),
	m_timeLimit(1),
	m_gameMode(0),
	m_map(0),
	m_currentOption(&m_gameMode),
	m_startLabel("start", sf::Vector2f(960, 683), resourceManager.getFontHolder()["oxinFont"]),
	m_backLabel("back", sf::Vector2f(1180, 683), resourceManager.getFontHolder()["oxinFont"])
{
	//adding our buttons to our buttons map
	m_buttons[m_gameModeBtn.getName()] = &m_gameModeBtn;
	m_buttons[m_mapBtn.getName()] = &m_mapBtn;
	m_buttons[m_killLimitBtn.getName()] = &m_killLimitBtn;
	m_buttons[m_timeLimitBtn.getName()] = &m_timeLimitBtn;

	//Add our buttons to our list
	m_btnList.push_back(&m_gameModeBtn);
	m_btnList.push_back(&m_mapBtn);
	m_btnList.push_back(&m_killLimitBtn);
	m_btnList.push_back(&m_timeLimitBtn);

	//Mapping our buttons to our options variables
	m_optionMapper[&m_gameModeBtn] = &m_gameMode;
	m_optionMapper[&m_mapBtn] = &m_map;
	m_optionMapper[&m_killLimitBtn] = &m_killLimit;
	m_optionMapper[&m_timeLimitBtn] = &m_timeLimit;

	//Change our text on our kill and time buttons
	m_killLimitBtn.setText(std::to_string(m_killLimit), resourceManager.getFontHolder()["arialFont"]);
	m_timeLimitBtn.setText(std::to_string(m_timeLimit) + ":00", resourceManager.getFontHolder()["arialFont"]);

	setIconSprite(m_startIcon, resourceManager.getTextureHolder()["Start Icon"], sf::Vector2f(914, 683));
	setIconSprite(m_backIcon, resourceManager.getTextureHolder()["Back Icon"], sf::Vector2f(1141, 683));
}

void PreGameScreen::update()
{
	//loop through our buttons map and update each one
	for (auto btn : m_buttons)
		btn.second->update();
}

void PreGameScreen::render(sf::RenderWindow & window)
{
	window.clear(sf::Color::White);

	//loop through our buttons map and update each one
	for (auto btn : m_buttons)
		btn.second->render(window);

	window.draw(m_startIcon);
	window.draw(m_backIcon);
	m_backLabel.draw(window);
	m_startLabel.draw(window);
}

void PreGameScreen::start()
{
	m_active = true;

	bool btnAlreadySelected = false;
	for (auto& btn : m_btnList)
	{
		if (btn->getSelected())
			btnAlreadySelected = true;
	}

	if (false == btnAlreadySelected)
	{
		//Set our pointer field to our game mode button
		m_currentOption = m_optionMapper[&m_gameModeBtn];

		//Selects the first button in our list as the currently selected button if no othe rbutton was previously selected
		selectButton(0);
	}
}

void PreGameScreen::end()
{
	m_active = false;
}

std::string PreGameScreen::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	auto currentScreen = m_name; //the current screen we are on is m_name ie. "PreGame"

	int currentOptionValue = *m_currentOption;

	if (controller1.isButtonPressed("B"))
	{
		if (m_buttonPressed)
		{
			m_buttonPressed = false;
			m_btnList.at(m_btnIndex)->pressed(false);
		}

		else
			currentScreen = "main menu";
	}

	if (controller1.isButtonPressed("Start"))
		currentScreen = "play game";

	if (controller1.isButtonPressed("A"))
	{
		m_buttonPressed = true;
		m_btnList.at(m_btnIndex)->pressed(true);
	}

	//If a button is not pressed then check for navigation
	if (false == m_buttonPressed)
	{
		bool navigated = false;
		//assing the new index the same value as our current index
		auto newIndex = m_btnIndex;


		if (controller1.isButtonPressed("LeftThumbStickUp") || controller1.isButtonPressed("DpadUp"))
		{
			navigated = true;
			newIndex--;
		}
		if (controller1.isButtonPressed("LeftThumbStickDown") || controller1.isButtonPressed("DpadDown"))
		{
			navigated = true;
			newIndex++;
		}

		//if we have navigated through the menu then changed buttons
		if (navigated)
		{
			//checking if our newIndex has gone out of bounds
			if (newIndex > 3)
				newIndex = 0;
			else if (newIndex < 0)
				newIndex = 3;

			selectButton(newIndex); //focus our button at index: newIndex
		}
	}
	//Else if it is pressed change the options of our Button
	else
	{
		bool navigated = false;

		if (controller1.isButtonPressed("LeftThumbStickUp") || controller1.isButtonPressed("DpadUp"))
		{
			navigated = true;
			currentOptionValue++;
		}
		if (controller1.isButtonPressed("LeftThumbStickDown") || controller1.isButtonPressed("DpadDown"))
		{
			navigated = true;
			currentOptionValue--;
		}

		//if we have navigated through the menu then changed buttons
		if (navigated)
		{
			//Checking our options
			if (currentOptionValue < 0)
				currentOptionValue = 0;

			*m_currentOption = currentOptionValue;

			//Change our kill limit button
			if (m_currentButton->getName() == "kill limit")
			{
				if (*m_currentOption == 0)
				{
					m_currentButton->setText("infinite", resourceManager.getFontHolder()["oxinFont"]);
				}
				else
				{
					m_currentButton->setText(std::to_string(*m_currentOption),resourceManager.getFontHolder()["arialFont"]);
				}
			}
			//Change our time limit button text
			else if (m_currentButton->getName() == "time limit")
			{
				if (*m_currentOption == 0)
				{
					m_currentButton->setText("infinite", resourceManager.getFontHolder()["oxinFont"]);
				}
				else
				{
					m_currentButton->setText(std::to_string(*m_currentOption) + ":00", resourceManager.getFontHolder()["arialFont"]);
				}
			}
		}

	}

	return currentScreen;
}

void PreGameScreen::selectButton(int newIndex)
{
	//if our new index is out of range then output an error message
	if (newIndex < 0 || newIndex > 3)
	{
		std::cout << newIndex + " is not within range of the vector: m_btnList" << std::endl;
	}
	else
	{
		m_btnList.at(m_btnIndex)->deSelect(); //deselect the current button

		m_btnIndex = newIndex; //reassign the button index to the new index

		m_btnList.at(m_btnIndex)->select(); //select the button at the new index

		//Set our pointer field to our game mode button
		m_currentOption = m_optionMapper[m_btnList.at(m_btnIndex)];

		m_currentButton = m_btnList.at(m_btnIndex);
	}
}

void PreGameScreen::setButtonText(OptionButton * btn, std::string text)
{
	btn->setText(text);
}

void PreGameScreen::setIconSprite(sf::Sprite & sprite, sf::Texture& texture, sf::Vector2f position)
{
	sprite.setTexture(texture);
	sprite.setOrigin(sprite.getGlobalBounds().left + sprite.getGlobalBounds().width / 2.0f, sprite.getGlobalBounds().top + sprite.getGlobalBounds().height / 2.0f);
	sprite.setPosition(position);
}

std::string PreGameScreen::getName()
{
	return m_name;
}
