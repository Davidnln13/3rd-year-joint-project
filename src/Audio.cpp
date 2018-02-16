#include "Audio.h"
#include "ScreenManager.h"
#include "JoystickController.h"

Audio::Audio()
{
	//music
	m_musicMap["TitleMusic"].openFromFile("./resources/Audio/TitleMusic.ogg");
	m_musicMap["GameMusic"].openFromFile("./resources/Audio/GameMusic.ogg");
	//loading sounds into sound buffer
	if (!m_soundBufferMap["CTFSound"].loadFromFile("./resources/Audio/CTFSound.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["Death"].loadFromFile("./resources/Audio/Death.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["MoveMenu"].loadFromFile("./resources/Audio/MoveMenu.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["SelectMenuItem"].loadFromFile("./resources/Audio/SelectMenuItem.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["SwordCollide"].loadFromFile("./resources/Audio/SwordCollide.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["SwordSwing"].loadFromFile("./resources/Audio/SwordSwing.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	if (!m_soundBufferMap["ThrowSword"].loadFromFile("./resources/Audio/ThrowSword.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}

	//load into soundmap
	m_soundMap["SwordSwing"].setBuffer(m_soundBufferMap["SwordSwing"]);
	m_soundMap["CTFSound"].setBuffer(m_soundBufferMap["CTFSound"]);
	m_soundMap["Death"].setBuffer(m_soundBufferMap["Death"]);
	m_soundMap["MoveMenu"].setBuffer(m_soundBufferMap["MoveMenu"]);
	m_soundMap["SelectMenuItem"].setBuffer(m_soundBufferMap["SelectMenuItem"]);
	m_soundMap["SwordCollide"].setBuffer(m_soundBufferMap["SwordCollide"]);
	m_soundMap["ThrowSword"].setBuffer(m_soundBufferMap["ThrowSword"]);
	
	for (auto& key : m_soundMap)
	{
		m_soundMap[key.first].setVolume(100);
	}
	for (auto& key : m_musicMap)
	{
		m_musicMap[key.first].setVolume(100);
	}
}
void Audio::updateMusic(std::string name)
{
	if (startMusic == false)
	{
		startMusic = true;
		m_musicMap["GameMusic"].stop();
		m_musicMap["TitleMusic"].setLoop(true);
		m_musicMap["TitleMusic"].play();
		currentName = "Main";
	}
	if (name == "Main" && currentName != "Main")
	{
		m_musicMap["GameMusic"].stop();
		m_musicMap["TitleMusic"].setLoop(true);
		m_musicMap["TitleMusic"].play();
		currentName = "Main";
	}
	else if (name == "Game" && currentName != "Game")
	{
		m_musicMap["TitleMusic"].stop();
		m_musicMap["GameMusic"].setLoop(true);
		m_musicMap["GameMusic"].play();
		currentName = "Game";
	}
}