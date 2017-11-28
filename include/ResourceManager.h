#pragma once
#include "Thor\Resources.hpp" //including resource library
#include "SFML\Graphics.hpp"
#include <iostream>
#include <mutex>
#include <map>

/// <summary>
/// Authors: Daryl Keogh
/// Description: The resource manager is responsible for loading in any textures/fonts/sounds or videos we need.
/// Time Spent: 10 Minutes
/// </summary>
class ResourceManager
{
public:
	 ResourceManager();
	~ResourceManager();

	//Method to load in files on request (this cna be overloaded to work with many different file types such as fonts and audio)
	void acquireResource(std::string resourceId, thor::ResourceLoader<sf::Texture> textureLoader, bool isSmooth = true);
	void acquireResource(std::string resourceId, thor::ResourceLoader<sf::Font> fontLoader);
	void acquireShader(std::string resourceId, std::string shaderPath);

	//getters
	thor::ResourceHolder<sf::Texture, std::string>& getTextureHolder();
	thor::ResourceHolder<sf::Font, std::string>& getFontHolder();
	std::map<std::string, std::string>& getShaderHolder();
private:
	//holds all of our textures
	thor::ResourceHolder<sf::Texture, std::string> m_textureHolder;
	//Holds fonts
	thor::ResourceHolder<sf::Font, std::string> m_fontHolder;
	//Holds shaders
	std::map<std::string, std::string> m_shaderHolder;

	//Our GUI Resources
	thor::ResourceLoader<sf::Texture> m_buttonLayout = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Button Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_swordIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/sword icon.png");
	thor::ResourceLoader<sf::Texture> m_optionsIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/settings icon.png");
	thor::ResourceLoader<sf::Texture> m_exitIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/exit icon.png");
	thor::ResourceLoader<sf::Texture> m_helpIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/help icon.png");
	thor::ResourceLoader<sf::Texture> m_crossedSwordIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Crossed Sword Icon.png");

	//Our Level tiles
	thor::ResourceLoader<sf::Texture> m_castleBg = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Castle BG.png");
	thor::ResourceLoader<sf::Texture> m_stoneTile = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Stone Tile.png");

	//Our player related sprite sheets and textures
	thor::ResourceLoader<sf::Texture> m_idlePlayer = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Idle Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerAttack = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Attack Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerRun = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Run Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_swordLight = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Sword Light.png");
	thor::ResourceLoader<sf::Texture> m_playerLight = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Player Light.png");
	thor::ResourceLoader<sf::Texture> m_sword= thor::Resources::fromFile<sf::Texture>("./resources/Textures/sword.png");

	//Our shader paths
	std::string m_recolourShaderPath = "./resources/Shaders/Recolour Shader.frag";

	//Our font(s)
	thor::ResourceLoader<sf::Font> m_oxinFont = thor::Resources::fromFile<sf::Font>("./resources/Fonts/OXIN.ttf");
};