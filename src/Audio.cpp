#include "Audio.h"
#include "ScreenManager.h"
#include "JoystickController.h"

Audio::Audio()
{
	//music
	m_musicMap["TitleMusic"].openFromFile("./resources/Audio/TitleMusic.ogg");
	m_musicMap["GameMusic"].openFromFile("./resources/Audio/GameMusic.ogg");

	//sounds 
	if (!m_soundBufferMap["SwordSwing"].loadFromFile("./resources/Audio/SwordSwing.wav"))
	{
		std::cout << "Cant find file" << std::endl;
	}
	m_soundMap["SwordSwing"].setBuffer(m_soundBufferMap["SwordSwing"]);
	
	for (auto& key : m_soundMap)
	{
		m_soundMap[key.first].setVolume(50);
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