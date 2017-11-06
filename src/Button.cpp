#include "Button.h"

Button::Button(sf::Vector2f position, std::string name):
	m_position(position),
	m_name(name)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()["buttonLayout"]);
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.0f, m_sprite.getLocalBounds().height / 2.0f); //settign the origin of our button to the center of our texture
	m_sprite.setPosition(m_position);
}

void Button::render(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

std::string& Button::getName()
{
	return m_name;
}
