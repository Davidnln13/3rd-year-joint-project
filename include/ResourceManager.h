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
	thor::ResourceLoader<sf::Texture> m_buttonLayout = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Button Layout.png");

	//Our font(s)
	thor::ResourceLoader<sf::Font> m_oxinFont = thor::Resources::fromFile<sf::Font>("./resources/Fonts/OXIN.ttf");
};