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
	thor::ResourceLoader<sf::Texture> m_buttonSelected = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Selected Button Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_optionsIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/settings icon.png");	
	thor::ResourceLoader<sf::Texture> m_helpIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/help icon.png");
	thor::ResourceLoader<sf::Texture> m_exitIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/exit icon.png");
	thor::ResourceLoader<sf::Texture> m_timerIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Timer Icon.png");
	thor::ResourceLoader<sf::Texture> m_killIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Kill Icon.png");
	thor::ResourceLoader<sf::Texture> m_mapIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Map Icon.png");
	thor::ResourceLoader<sf::Texture> m_mainMenuIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/main menu icon.png");
	thor::ResourceLoader<sf::Texture> m_crossedSwordIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Crossed Sword Icon.png");
	thor::ResourceLoader<sf::Texture> m_arrowIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Arrow Icon.png");
	thor::ResourceLoader<sf::Texture> m_playerIcons = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Player Icons.png");
	thor::ResourceLoader<sf::Texture> m_plusScoreIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Add score Icon.png");
	thor::ResourceLoader<sf::Texture> m_minusScoreIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Minus Score Icon.png");
	thor::ResourceLoader<sf::Texture> m_particle = thor::Resources::fromFile<sf::Texture>("./resources/Textures/particle.png");
	//controller icons 
	thor::ResourceLoader<sf::Texture> m_startIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Controller Icons/Start Icon.png");
	thor::ResourceLoader<sf::Texture> m_bIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/B Icon.png");
	thor::ResourceLoader<sf::Texture> m_aIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/A Icon.png");
	thor::ResourceLoader<sf::Texture> m_yIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/Y Icon.png");
	thor::ResourceLoader<sf::Texture> m_xIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/X Icon.png");
	thor::ResourceLoader<sf::Texture> m_ltIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/LT Icon.png");
	thor::ResourceLoader<sf::Texture> m_dpadIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/Dpad Icon.png");
	thor::ResourceLoader<sf::Texture> m_lanalogIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI//Controller Icons/Lanalog Icon.png");
	thor::ResourceLoader<sf::Texture> m_winIndicator = thor::Resources::fromFile<sf::Texture>("./resources/GUI/win indicator.png");
	thor::ResourceLoader<sf::Texture> m_loseIndicator = thor::Resources::fromFile<sf::Texture>("./resources/GUI/lose indicator.png");
	thor::ResourceLoader<sf::Texture> m_drawIndicator = thor::Resources::fromFile<sf::Texture>("./resources/GUI/draw indicator.png");

	//Our Level tiles
	thor::ResourceLoader<sf::Texture> m_castleBg = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Castle BG.png");
	thor::ResourceLoader<sf::Texture> m_stoneTile = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Stone Tile.png");
	thor::ResourceLoader<sf::Texture> m_torch = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Torch Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_torchLight = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Torch Light Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_windowTile = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Window.png");

	//Our player related sprite sheets and textures
	thor::ResourceLoader<sf::Texture> m_idlePlayer = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Idle Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerAttack = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Attack Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerRun = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Run Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerJump = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Jump Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_playerPickup = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Pickup Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_swordLight = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Sword Light.png");
	thor::ResourceLoader<sf::Texture> m_playerLight = thor::Resources::fromFile<sf::Texture>("./resources/Textures/Player Light.png");
	thor::ResourceLoader<sf::Texture> m_sword= thor::Resources::fromFile<sf::Texture>("./resources/Textures/sword.png");

	//Our shader paths
	std::string m_recolourShaderPath = "./resources/Shaders/Recolour Shader.frag";

	//Our font(s)
	thor::ResourceLoader<sf::Font> m_oxinFont = thor::Resources::fromFile<sf::Font>("./resources/Fonts/OXIN.ttf");
	thor::ResourceLoader<sf::Font> m_arialFont = thor::Resources::fromFile<sf::Font>("./resources/Fonts/ARIAL.ttf");
};