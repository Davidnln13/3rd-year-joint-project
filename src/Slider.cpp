#include "Slider.h"

Slider::Slider(std::string name, std::string iconName, sf::Vector2f pos, sf::IntRect rect):
	m_currentVal(100),
	m_name(name),
	m_position(pos),
	m_rect(rect)
{
	m_sprite.setTexture(resourceManager.getTextureHolder()["Button Spritesheet Reversed"]);
	m_sprite.setTextureRect(rect);
	m_sprite.setPosition(m_position);

	m_iconSprite.setTexture(resourceManager.getTextureHolder()[iconName]);
	m_iconSprite.setOrigin(m_iconSprite.getLocalBounds().width / 2.0f, m_iconSprite.getLocalBounds().height / 2.0f);
	m_iconSprite.setPosition(m_position.x + m_sprite.getLocalBounds().width - m_iconSprite.getLocalBounds().width / 2.0f + 5
	, m_position.y + m_iconSprite.getLocalBounds().height / 2.0f + 2.5f);
}

void Slider::update()
{

}

void Slider::draw(sf::RenderWindow & window)
{
	window.draw(m_sprite);
	window.draw(m_iconSprite);
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
		setRect(sf::IntRect(m_rect.left - 400, m_rect.top, m_rect.width, m_rect.height));
		m_iconSprite.setPosition(m_iconSprite.getPosition().x - 35.818f, m_iconSprite.getPosition().y);
	}
}

void Slider::moveUp()
{
	if (m_currentVal < 100)
	{
		m_currentVal += 10;
		setRect(sf::IntRect(m_rect.left + 400, m_rect.top, m_rect.width, m_rect.height));
		m_iconSprite.setPosition(m_iconSprite.getPosition().x + 35.818f, m_iconSprite.getPosition().y);
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
