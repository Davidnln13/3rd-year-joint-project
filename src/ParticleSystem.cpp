#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(float timeToLive, int maxParticles, sf::Vector2f originPos):
	m_ttl(timeToLive),
	m_maxParticles(maxParticles)
{
	srand(time(NULL)); //Setting the random seed

	for (int i = 0; i < m_maxParticles; i++)
	{
		//Get a random number between for y and x velocity
		float xVel = (-.5f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.5f - (-.5f))));
		float yVel = (-1.0f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (.25f - (-1.0f))));
		float ttlScalar = (.25f) + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - (.25f))));

		m_particles.push_back(Particle(originPos, sf::Vector2f(xVel, yVel), 1.0f * ttlScalar));
	}

}

void ParticleSystem::update()
{
	//Update every particle
	for (auto& particle : m_particles)
		particle.update();

}

void ParticleSystem::draw(sf::RenderWindow & window)
{
	//Draw every particle
	for (auto& particle : m_particles)
		particle.draw(window);
}
