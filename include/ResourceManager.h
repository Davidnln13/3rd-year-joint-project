#pragma once
#include "Thor\Resources.hpp" //including resource library
#include "SFML\Graphics.hpp"
#include <iostream>
#include <mutex>

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
	void acquireResource(std::string resourceId, thor::ResourceLoader<sf::Texture> textureLoader);
	void acquireResource(std::string resourceId, thor::ResourceLoader<sf::Font> fontLoader);

	//getters
	thor::ResourceHolder<sf::Texture, std::string>& getTextureHolder();
	thor::ResourceHolder<sf::Font, std::string>& getFontHolder();

private:
	//holds all of our textures
	thor::ResourceHolder<sf::Texture, std::string> m_textureHolder;
	//Holds fonts
	thor::ResourceHolder<sf::Font, std::string> m_fontHolder;

	//Our GUI Resources
	thor::ResourceLoader<sf::Texture> m_buttonLayout = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Button Spritesheet.png");
	thor::ResourceLoader<sf::Texture> m_swordIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/sword icon.png");
	thor::ResourceLoader<sf::Texture> m_optionsIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/settings icon.png");	
	thor::ResourceLoader<sf::Texture> m_helpIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/help icon.png");
	thor::ResourceLoader<sf::Texture> m_exitIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/exit icon.png");
	thor::ResourceLoader<sf::Texture> m_continueIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/continue icon.png");
	thor::ResourceLoader<sf::Texture> m_backIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/back icon.png");
	thor::ResourceLoader<sf::Texture> m_mainMenuIcon = thor::Resources::fromFile<sf::Texture>("./resources/GUI/main menu icon.png");
	thor::ResourceLoader<sf::Texture> m_controllerPic = thor::Resources::fromFile<sf::Texture>("./resources/GUI/controller.png");

	//Our font(s)
	thor::ResourceLoader<sf::Font> m_oxinFont = thor::Resources::fromFile<sf::Font>("./resources/Fonts/OXIN.ttf");
};