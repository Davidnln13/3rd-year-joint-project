#pragma once
#include <SFML\Graphics.hpp>
#include "ResourceManager.h"

extern ResourceManager resourceManager;

class Particle
{
public:
	Particle(sf::Vector2f position, sf::Vector2f velocity, float ttl);

	void update();
	void draw(sf::RenderWindow& window);

private:
	float m_alpha;
	float m_ttl; //The time to live for our particle
	float m_timeAlive; //The time the particle has been alive for
	sf::Vector2f m_velocity; //The velocity of our particle
	sf::Vector2f m_position; //The position of our particle
	sf::Sprite m_sprite; //Our particle sprite
	sf::Clock m_clock;
};