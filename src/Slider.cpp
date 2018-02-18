#include "Slider.h"

Slider::Slider(std::string name, sf::Vector2f pos, sf::IntRect rect):
	m_currentVal(100),
	m_name(name),
	m_position(pos),
	m_rect(rect)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()["Button Spritesheet Reversed"]);
	m_sprite.setTextureRect(rect);
	m_sprite.setPosition(m_position);
}

void Slider::update()
{

}

void Slider::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
}

void Slider::setSliderLevel(int level)
{
	m_currentVal = level;
	if (m_currentVal > 100)
		m_currentVal = 100;
	if (m_currentVal < 0)
		m_currentVal = 0;
}

void Slider::setTexture(sf::Texture texture)
{
	m_sprite.setTexture(texture);
}

void Slider::setRect(sf::IntRect rect)
{
	m_sprite.setTextureRect(rect);
	m_rect = rect;
}

void Slider::moveDown()
{
	if (m_currentVal > 0)
	{
		m_currentVal -= 10;
		setRect(sf::IntRect(m_rect.left + 400, m_rect.top, m_rect.width, m_rect.height));
	}
}

void Slider::moveUp()
{
	if (m_currentVal < 100)
	{
		m_currentVal += 10;
		setRect(sf::IntRect(m_rect.left - 400, m_rect.top, m_rect.width, m_rect.height));
	}
}

int Slider::getSliderLevel()
{
	return m_currentVal;
}

std::string & Slider::getName()
{
	return m_name;
}
