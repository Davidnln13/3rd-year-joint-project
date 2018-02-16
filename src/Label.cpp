#include "Label.h"

Label::Label(std::string textString, sf::Vector2f& position, sf::Font& font):
	m_position(position.x + 27.5f, position.y)
{
	m_text.setFont(font);
	m_text.setCharacterSize(30);
	m_text.setString(textString);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	m_text.setFillColor(sf::Color::Black);
	m_text.setPosition(m_position);
}

void Label::select()
{
	auto newColor = m_text.getFillColor();
	newColor.r = 255;
	newColor.g = 255;
	newColor.b = 255;
	m_text.setFillColor(newColor);
}

void Label::deSelect()
{
	auto newColor = m_text.getFillColor();
	newColor.r = 0;
	newColor.g = 0;
	newColor.b = 0;
	m_text.setFillColor(newColor);
}

void Label::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}

void Label::setText(std::string text)
{
	m_text.setString(text);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	m_text.setPosition(m_position);
}

void Label::setText(std::string text, sf::Vector2f pos)
{
	m_text.setString(text);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	m_text.setPosition(pos);
}

void Label::setText(std::string text, sf::Font & font, sf::Color color, sf::Vector2f origin = sf::Vector2f(99, 99))
{
	m_text.setFont(font);
	m_text.setString(text);
	if (origin.x == 99 && origin.y == 99)
		m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	else
		m_text.setOrigin(origin);
	m_text.setPosition(m_position);
}

void Label::setText(std::string text, sf::Font & font, sf::Color color)
{
	m_text.setFont(font);
	m_text.setString(text);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
	m_text.setPosition(m_position);
}

void Label::setOrigin(sf::Vector2f origin)
{
	m_text.setOrigin(origin);
	m_text.setPosition(m_position);
}

void Label::setPosition(sf::Vector2f pos)
{
	m_text.setPosition(pos);
}

void Label::setOriginAndPosition(sf::Vector2f origin, sf::Vector2f pos)
{
	setOrigin(origin);
	setPosition(pos);
}

void Label::setColor(sf::Color color)
{
	m_text.setFillColor(color);
}

void Label::setSize(int size)
{
	m_text.setCharacterSize(size);
	m_text.setOrigin(m_text.getLocalBounds().left + m_text.getLocalBounds().width / 2.0f, m_text.getLocalBounds().top + m_text.getGlobalBounds().height / 2.0f);
}

sf::Text & Label:: getText()
{
	return m_text;
}

sf::Vector2f Label::getPos()
{
	return m_text.getPosition();
}
