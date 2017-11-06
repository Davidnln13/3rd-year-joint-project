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

	void acquireResource(std::string resourceId, thor::ResourceLoader<sf::Texture> textureLoader);

	thor::ResourceHolder<sf::Texture, std::string>& getTextureHolder();

private:
	//holds all of our textures
	thor::ResourceHolder<sf::Texture, std::string> m_textureHolder;

	//Our GUI Resources
	thor::ResourceLoader<sf::Texture> m_buttonLayout = thor::Resources::fromFile<sf::Texture>("./resources/GUI/Button Layout.png");
};