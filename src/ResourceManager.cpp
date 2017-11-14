#include "ResourceManager.h"

/// <summary>
/// Authors: Daryl Keogh
/// Description: The resource manager is responsible for loading in any textures/fonts/sounds or videos we need.
/// Time Spent: 10 Minutes
/// </summary>

ResourceManager::ResourceManager()
{
	//load all of our resources
	try 
	{
		//load GUI
		acquireResource("Button Spritesheet", m_buttonLayout);
		acquireResource("Sword Icon", m_swordIcon);
		acquireResource("Options Icon", m_optionsIcon);
		acquireResource("Help Icon", m_helpIcon);
		acquireResource("Exit Icon", m_exitIcon);
		acquireResource("Continue Icon", m_continueIcon);
		acquireResource("Main Menu Icon", m_mainMenuIcon);

		//Load Fonts
		acquireResource("oxinFont", m_oxinFont);
	}
	catch (thor::ResourceLoadingException& e)
	{
		std::cout << e.what() << std::endl; //output error
	}
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

//This method acquires our fonts
void ResourceManager::acquireResource(std::string resourceId, thor::ResourceLoader<sf::Font> fontLoader)
{
	m_fontHolder.acquire(resourceId, fontLoader);
}

thor::ResourceHolder<sf::Texture, std::string>& ResourceManager::getTextureHolder()
{
	return m_textureHolder;
}

thor::ResourceHolder<sf::Font, std::string>& ResourceManager::getFontHolder()
{
	return m_fontHolder;
}
