#pragma once
#include "Thor\Resources.hpp" //including resource library
#include "SFML\Graphics.hpp"
#include <iostream>

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
};