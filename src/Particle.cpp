#include "Particle.h"

Particle::Particle(sf::Vector2f position, sf::Vector2f velocity, float ttl):
	m_velocity(velocity),
	m_ttl(ttl),
	m_position(position),
	m_alpha(255),
	m_timeAlive(0)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()["Particle"]);
	m_sprite.setOrigin(m_sprite.getGlobalBounds().left + m_sprite.getGlobalBounds().width / 2.0f, m_sprite.getGlobalBounds().top + m_sprite.getGlobalBounds().height / 2.0f);
	m_sprite.setPosition(m_position);
	m_clock.restart();
}

void Particle::update()
{
	m_timeAlive += m_clock.restart().asSeconds(); //Add the time gone since last frame

	if (m_timeAlive >= m_ttl)
		m_timeAlive = m_ttl;

	m_velocity.y += 0.0196f; //Gravity

	m_alpha = 255 - (255 * m_timeAlive / m_ttl);

	m_position += m_velocity;

	m_sprite.setPosition(m_position);
	m_sprite.setColor(sf::Color(m_sprite.getColor().r, m_sprite.getColor().g, m_sprite.getColor().b, m_alpha));
	m_clock.restart();
}

void Particle::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}
