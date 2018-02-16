#pragma once
#include "SFML\Audio.hpp"
#include <map>

enum GameState;
class JoystickController;

class Audio
{
public:
	Audio();
	//audio maps
	std::map<std::string, sf::Music> m_musicMap;
	std::map<std::string, sf::SoundBuffer> m_soundBufferMap;
	std::map<std::string, sf::Sound> m_soundMap;
	std::string currentName = "Main";
	bool startMusic = false;
	void updateMusic(std::string name);
	void setSounds(std::string name, std::string source);
};
