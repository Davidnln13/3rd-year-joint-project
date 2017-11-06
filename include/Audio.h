#pragma once
#include "SFML\Audio.hpp"

enum GameState;
class JoystickController;

class Audio
{
public:
	Audio();
	sf::Music& getSong() { return m_music[musicIndexer]; };
	void update();
	sf::Music m_music[1];
	sf::SoundBuffer m_soundBuffer[1];
	sf::Sound m_soundArray[1];
	int musicIndexer = 0;
};
