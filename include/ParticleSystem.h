#pragma once
#include "Particle.h"

class ParticleSystem
{
public:
	ParticleSystem(float timeToLive, int maxParticles, sf::Vector2f originPos);

	void update();
	void draw(sf::RenderWindow& window);

private:
	float m_ttl; //The amount of time our particle sytem will live for
	int m_maxParticles; //The max amount of particles
	std::vector<Particle> m_particles;
};