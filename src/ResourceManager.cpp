#include "ResourceManager.h"

/// <summary>
/// Authors: Daryl Keogh
/// Description: The resource manager is responsible for loading in any textures/fonts/sounds or videos we need.
/// Time Spent: 10 Minutes
/// </summary>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	//unload assets
}

//This method acquires our texture and then sets our texture smooth to true
void ResourceManager::acquireResource(std::string resourceId, thor::ResourceLoader<sf::Texture> textureLoader)
{
	m_textureHolder.acquire(resourceId, textureLoader);
	m_textureHolder[resourceId].setSmooth(true);
}

thor::ResourceHolder<sf::Texture, std::string>& ResourceManager::getTextureHolder()
{
	return m_textureHolder;
}