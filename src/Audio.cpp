#include "Audio.h"
#include "ScreenManager.h"
#include "JoystickController.h"

Audio::Audio()
{
	//music
	m_music[0].openFromFile("./resources/Audio/Music.ogg");

	if (!m_soundBuffer[0].loadFromFile("./resources/Audio/SwordCollide.wav"))
	{
		std::cout << "Cant Find File" << std::endl;
	}
	m_soundArray[0].setBuffer(m_soundBuffer[0]);
}
void Audio::update()
{
	if (m_music[0].getStatus() == sf::Music::Status::Stopped)
	{
		m_music[0].play();
	}
}