#include "Label.h"

Label::Label(std::string textString, sf::Vector2f& position):
	m_position(position.x + 27.5f, position.y)
{
	m_text.setFont(resourceManager.getFontHolder()["oxinFont"]);
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

sf::Text & Label:: getText()
{
	return m_text;
}
