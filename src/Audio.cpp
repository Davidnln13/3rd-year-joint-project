#include "Audio.h"
#include "ScreenManager.h"
#include "JoystickController.h"

Audio::Audio()
{
	//music
	m_musicMap["TitleMusic"].openFromFile("./resources/Audio/TitleMusic.ogg");
	m_musicMap["GameMusic"].openFromFile("./resources/Audio/GameMusic.ogg");
	//loading sounds
	setSounds("CTFSound", "./resources/Audio/CTFSound.wav");
	setSounds("Jump", "./resources/Audio/Jump.wav");
	setSounds("Death", "./resources/Audio/Death.wav");
	setSounds("MoveMenu", "./resources/Audio/MoveMenu.wav");
	setSounds("SelectMenuItem", "./resources/Audio/SelectMenuItem.wav");
	setSounds("SwordCollide", "./resources/Audio/SwordCollide.wav");
	setSounds("SwordSwing", "./resources/Audio/SwordSwing.wav");
	setSounds("ThrowSword", "./resources/Audio/ThrowSword.wav");
 
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

void Audio::setSounds(std::string name, std::string source)
{
	//load into soundbuffer
	if (!m_soundBufferMap[name].loadFromFile(source))
	{
		std::cout << "Cant find file" << std::endl;
	}
	m_soundMap[name].setBuffer(m_soundBufferMap[name]);
}
