#include "PlayScreen.h"

PlayScreen::PlayScreen(std::string name, Audio& audio) :
	Screen(name),
	m_continueBtn(sf::Vector2f(640, 144), "continue", "Arrow Icon"),
	m_optionsBtn(sf::Vector2f(640, 288), "options", "Options Icon"),
	m_helpBtn(sf::Vector2f(640, 432), "help", "Help Icon"),
	m_mainMenuBtn(sf::Vector2f(640, 576), "main menu", "Main Menu Icon"),
	m_audioPlayScreen(audio),
	m_level(audio, 1),
	m_paused(false),
	m_btnIndex(0)
{
	//adding our buttons to our buttons map
	m_buttons[m_continueBtn.getName()] = &m_continueBtn;
	m_buttons[m_optionsBtn.getName()] = &m_optionsBtn;
	m_buttons[m_helpBtn.getName()] = &m_helpBtn;
	m_buttons[m_mainMenuBtn.getName()] = &m_mainMenuBtn;

	m_btnList.push_back(&m_continueBtn);
	m_btnList.push_back(&m_optionsBtn);
	m_btnList.push_back(&m_helpBtn);
	m_btnList.push_back(&m_mainMenuBtn);

	m_overlay.setSize(sf::Vector2f(1280, 720));
	m_overlay.setFillColor(sf::Color(255, 255, 255, 100));

	selectButton(0);//Select the continue button
}

void PlayScreen::update()
{
	//If the game isnt paused
	if (m_paused == false)
	{
		//update our level
		m_level.update();
	}
}

void PlayScreen::render(sf::RenderWindow& window)
{
	window.clear(sf::Color::White);

	m_level.render(window);

	if (m_paused)
	{
		window.draw(m_overlay);

		//loop through our buttons map and render each one
		for (auto key : m_buttons)
			key.second->render(window);
	}
}

void PlayScreen::start()
{
	if (m_paused == false)
	{
		m_paused = false;
		m_audioPlayScreen.updateMusic("Game");
		m_active = true;
	}
}

void PlayScreen::end()
{
	//Clears our level
	if (m_paused == false)
	{
		m_level.clearLevel();
		m_active = false;
	}
}

std::string PlayScreen::handleInput(JoystickController& controller1, JoystickController& controller2)
{
	auto currentScreen = m_name;
	if (m_paused == false)
	{
		//Handle input in the level
		currentScreen = m_level.handleInput(controller1, controller2);
	}
	//If the game is paused
	else
	{
		//assing the new index the same value as our current index
		auto newIndex = m_btnIndex;
		bool navigated = false;

		if (controller1.isButtonPressed("A"))
		{
			m_audioPlayScreen.m_soundMap["SelectMenuItem"].play();
			if (m_btnList.at(m_btnIndex)->getName() == "continue")
				m_paused = false;
			else if (m_btnList.at(m_btnIndex)->getName() == "main menu")
			{
				m_paused = false;
				currentScreen = m_btnList.at(m_btnIndex)->getName(); //assign the current screen the name of our button
			}
			else
				currentScreen = m_btnList.at(m_btnIndex)->getName(); //assign the current screen the name of our button
		}

		if (controller1.isButtonPressed("LeftThumbStickUp") || controller1.isButtonPressed("DpadUp"))
		{
			m_audioPlayScreen.m_soundMap["MoveMenu"].play();
			navigated = true;
			newIndex--;
		}
		if (controller1.isButtonPressed("LeftThumbStickDown") || controller1.isButtonPressed("DpadDown"))
		{
			m_audioPlayScreen.m_soundMap["MoveMenu"].play();
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


	//if either player has pressed tsart then pause the game
	if (controller1.isButtonPressed("Start") || controller2.isButtonPressed("Start"))
	{
		m_paused = !m_paused; //Pause/unpause the screen
	}

	return currentScreen;
}

void PlayScreen::selectButton(int newIndex)
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
	}
}

std::string PlayScreen::getName()
{
	return m_name;
}

bool PlayScreen::paused()
{
	return m_paused;
}

bool PlayScreen::gameOver()
{
	return m_level.gameOver();
}

float PlayScreen::gameOverTime()
{
	return m_level.gameOverTime();
}

void PlayScreen::setLevel(int maxKills, int maxTime, int levelNumber, bool ctf, std::map<int, std::string>& levelNames)
{
	m_level.setLevelParameters(maxKills, maxTime, levelNumber, ctf, levelNames);
}
