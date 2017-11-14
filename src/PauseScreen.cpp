#include "PauseScreen.h"

PauseScreen::PauseScreen(std::string name, Audio & audio) :
	Screen(name),
	m_continueBtn(sf::Vector2f(640, 144), "continue", "Continue Icon"),
	m_optionsBtn(sf::Vector2f(640, 288), "options", "Options Icon"),
	m_helpBtn(sf::Vector2f(640, 432), "help", "Help Icon"),
	m_mainMenuBtn(sf::Vector2f(640, 576), "main menu", "Main Menu Icon"),
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
}

void PauseScreen::update()
{
	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->update();
}

void PauseScreen::render(sf::RenderWindow & window)
{
	window.clear(sf::Color::White);
	//loop through our buttons map and render each one
	for (auto key : m_buttons)
		key.second->render(window);
}

void PauseScreen::start()
{
	m_active = true;
	selectButton(0);
}

void PauseScreen::end()
{
	m_active = false;
}

std::string PauseScreen::handleInput(JoystickController & controller1, JoystickController & controller2)
{
	auto currentScreen = m_name;
	//assing the new index the same value as our current index
	auto newIndex = m_btnIndex;
	bool navigated = false;

	if (controller1.isButtonPressed("A"))
	{
		if (m_btnList.at(m_btnIndex)->getName() == "continue")
			currentScreen = "play game";
		else
			currentScreen = m_btnList.at(m_btnIndex)->getName(); //assign the current screen the name of our button
	}

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
	return currentScreen;
}

void PauseScreen::selectButton(int newIndex)
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

std::string PauseScreen::getName()
{
	return m_name;
}
