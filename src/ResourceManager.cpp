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
		acquireResource("Controller", m_controllerPic);
		acquireResource("Options Icon", m_optionsIcon);
		acquireResource("Exit Icon", m_exitIcon);
		acquireResource("Main Menu Icon", m_mainMenuIcon);
		acquireResource("Help Icon", m_helpIcon);
		acquireResource("Crossed Sword Icon", m_crossedSwordIcon);
		acquireResource("Arrow Icon", m_arrowIcon);

		//Load level textures
		acquireResource("castleBG", m_castleBg);
		acquireResource("stoneTile", m_stoneTile);
		acquireResource("torch", m_torch);
		acquireResource("torchLight", m_torchLight);

		//Load player textures
		acquireResource("playerIdle", m_idlePlayer, false); //setting our animations to not smooth eliminates artefacts
		acquireResource("playerAttack", m_playerAttack, false);
		acquireResource("playerRun", m_playerRun, false);
		acquireResource("playerJump", m_playerJump, false);
		acquireResource("playerPickup", m_playerPickup, false);
		acquireResource("playerLight", m_playerLight);
		acquireResource("swordLight", m_swordLight);
		acquireResource("sword", m_sword);

		//Load our shaders
		acquireShader("recolourShader", m_recolourShaderPath);

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
void ResourceManager::acquireResource(std::string resourceId, thor::ResourceLoader<sf::Texture> textureLoader, bool isSmooth)
{
	m_textureHolder.acquire(resourceId, textureLoader);
	m_textureHolder[resourceId].setSmooth(isSmooth);
}

//This method acquires our fonts
void ResourceManager::acquireResource(std::string resourceId, thor::ResourceLoader<sf::Font> fontLoader)
{
	m_fontHolder.acquire(resourceId, fontLoader);
}

//loads in our shaders
void ResourceManager::acquireShader(std::string resourceId, std::string shaderPath)
{
	//adds a string with the key 'resourceId' with the path to the shader
	m_shaderHolder[resourceId] = shaderPath;
}

thor::ResourceHolder<sf::Texture, std::string>& ResourceManager::getTextureHolder()
{
	return m_textureHolder;
}

thor::ResourceHolder<sf::Font, std::string>& ResourceManager::getFontHolder()
{
	return m_fontHolder;
}

std::map<std::string, std::string>& ResourceManager::getShaderHolder()
{
	return m_shaderHolder;
}
