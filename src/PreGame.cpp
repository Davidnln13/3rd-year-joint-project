#include "PreGame.h"

bool GameMode::isCtf = false;
int GameMode::killLimit = 5;
int GameMode::timeLimit = 1;
int GameMode::levelNum = 0;
int GameMode::gameModeNum = 0;
int GameMode::maxGameModes = 2;
int GameMode::maxLevels = 1;
std::map<int, std::string> GameMode::levelNames;


PreGameScreen::PreGameScreen(std::string name, Audio & audio) :
	Screen(name),
	gameMode(),
	m_gameModeBtn(sf::Vector2f(640, 144), "game mode", resourceManager.getFontHolder()["oxinFont"], "Options Icon"),
	m_mapBtn(sf::Vector2f(640, 288), "level name", resourceManager.getFontHolder()["oxinFont"], "Map Icon"),
	m_killLimitBtn(sf::Vector2f(640, 432), "kill limit", resourceManager.getFontHolder()["arialFont"], "Kill Icon"),
	m_timeLimitBtn(sf::Vector2f(640, 576), "time limit", resourceManager.getFontHolder()["arialFont"], "Timer Icon"),
	m_buttonPressed(false),
	m_currentOption(&gameMode.gameModeNum),
	m_startLabel("start", sf::Vector2f(960, 683), resourceManager.getFontHolder()["oxinFont"]),
	m_backLabel("back", sf::Vector2f(1180, 683), resourceManager.getFontHolder()["oxinFont"]),
	m_transitionCol(255, 255, 255, 255),
	m_transitionRect(sf::Vector2f(1280, 720))
{
	//Set the position of the rectangle
	m_transitionRect.setPosition(0, 0);

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
	m_optionMapper[&m_gameModeBtn] = &GameMode::gameModeNum;
	m_optionMapper[&m_mapBtn] = &GameMode::levelNum;
	m_optionMapper[&m_killLimitBtn] = &GameMode::killLimit;
	m_optionMapper[&m_timeLimitBtn] = &GameMode::timeLimit;

	//Change our text on our buttons
	m_killLimitBtn.setText(std::to_string(GameMode::killLimit), resourceManager.getFontHolder()["arialFont"]);
	m_timeLimitBtn.setText(std::to_string(GameMode::timeLimit) + ":00", resourceManager.getFontHolder()["arialFont"]);
	m_mapBtn.setText(gameMode.levelNames[GameMode::levelNum], resourceManager.getFontHolder()["oxinFont"]);
	m_gameModeBtn.setText(gameMode.gameModes[GameMode::levelNum], resourceManager.getFontHolder()["oxinFont"]);

	setIconSprite(m_startIcon, resourceManager.getTextureHolder()["Start Icon"], sf::Vector2f(914, 683));
	setIconSprite(m_bIcon, resourceManager.getTextureHolder()["B Icon"], sf::Vector2f(1141, 683));
}

void PreGameScreen::update()
{
	//loop through our buttons map and update each one
	for (auto btn : m_buttons)
		btn.second->update();

	if (m_alpha != 0)
	{
		m_alpha = lerpValue(m_alpha, 0, 1.0f);
		m_transitionCol.a = m_alpha;

		m_transitionRect.setFillColor(m_transitionCol);
	}
}

void PreGameScreen::render(sf::RenderWindow & window)
{
	window.clear(sf::Color::White);

	//loop through our buttons map and update each one
	for (auto btn : m_buttons)
		btn.second->render(window);

	window.draw(m_startIcon);
	window.draw(m_bIcon);
	m_backLabel.draw(window);
	m_startLabel.draw(window);

	window.draw(m_transitionRect); //Draw our transition rectangle
}

void PreGameScreen::start()
{
	m_active = true;

	//Resetting our transition variables
	m_alpha = 255;
	m_transitionCol.a = m_alpha;
	m_transitionRect.setFillColor(m_transitionCol);

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
	{
		currentScreen = "play game";
	}

	if (controller1.isButtonPressed("A"))
	{
		if (m_buttonPressed)
		{
			m_buttonPressed = false;
			m_btnList.at(m_btnIndex)->pressed(false);
		}
		else
		{
			m_buttonPressed = true;
			m_btnList.at(m_btnIndex)->pressed(true);
		}
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

			//Checking our options bounds
			if (currentOptionValue < 0)
			{
				//If the button we have slected is the level button then set our current option to max level num
				if (m_currentButton->getName() == "level name")
					currentOptionValue = GameMode::maxLevels;
				else if (m_currentButton->getName() == "game mode")
				{
					GameMode::isCtf = false;
					currentOptionValue = GameMode::maxGameModes;
					//if the gamemode choesn is CTF then ste our boolean
					if (currentOptionValue == 2)
						GameMode::isCtf = true;
				}

				else //else set our value to 0
					currentOptionValue = 0;
			}
			else
			{
				if (m_currentButton->getName() == "level name" && currentOptionValue > 1)
					currentOptionValue = 0;
				else if (m_currentButton->getName() == "game mode" && currentOptionValue > 2)
					currentOptionValue = 0;
			}

			if (m_currentButton->getName() == "game mode")
			{
				//If our game mode is Capture the flag, set the parameters of our game mode
				if (currentOptionValue == 2)
				{
					GameMode::isCtf = true;
					GameMode::killLimit = 0; //set kill limit to infinite
					GameMode::timeLimit = 3; //set time limit to 3 minutes
				}
				//else If our game mode is Sudden death, set the parameters of our game mode
				else if (currentOptionValue == 1)
				{
					GameMode::isCtf = false;
					GameMode::killLimit = 1; //set kill limit to 1
					GameMode::timeLimit = 0; //set time limit to infinite
				}
				//else if our current game mode is death match, set our parameters for a deathmatch
				else if (currentOptionValue == 0)
				{
					GameMode::isCtf = false;
					GameMode::killLimit = 5; //set kill limit to 5
					GameMode::timeLimit = 1; //set time limit to 1 minute
				}
			}


			//Set our current option to our new option value
			*m_currentOption = currentOptionValue;

			//Set the text for all of our butttons
			setButtonText(m_killLimitBtn, GameMode::killLimit, "infinite", std::to_string(GameMode::killLimit), resourceManager.getFontHolder()["arialFont"], resourceManager.getFontHolder()["oxinFont"]);
			setButtonText(m_timeLimitBtn, GameMode::timeLimit, "infinite", std::to_string(GameMode::timeLimit) + ":00", resourceManager.getFontHolder()["arialFont"], resourceManager.getFontHolder()["oxinFont"]);
			setButtonText(m_mapBtn, GameMode::levelNum, gameMode.levelNames[GameMode::levelNum], gameMode.levelNames[GameMode::levelNum], resourceManager.getFontHolder()["oxinFont"], resourceManager.getFontHolder()["oxinFont"]);
			setButtonText(m_gameModeBtn, GameMode::gameModeNum, gameMode.gameModes[GameMode::gameModeNum], gameMode.gameModes[GameMode::gameModeNum], resourceManager.getFontHolder()["oxinFont"], resourceManager.getFontHolder()["oxinFont"]);
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

void PreGameScreen::setButtonText(OptionButton& btn, int value, std::string conditionText, std::string text, sf::Font & font, sf::Font& conditionFont)
{
	if (value == 0)
	{
		btn.setText(conditionText, conditionFont);
	}
	else
	{
		btn.setText(text, font);
	}
}

std::string PreGameScreen::getName()
{
	return m_name;
}

//Our gameMode constructor
//Here we set the default parameters for our gamemode
GameMode::GameMode() :
	name("Deathmatch")
{
	//Initialising our maps
	GameMode::gameModes[0] = "Deathmatch";
	GameMode::gameModes[1] = "Sudden Death";
	GameMode::gameModes[2] = "Capture the Flag";
	GameMode::levelNames[0] = "Castle";
	GameMode::levelNames[1] = "Castle CTF";
}
