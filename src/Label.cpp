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
	m_text.setFillColor(sf::Color::White);
}

void Label::deSelect()
{
	m_text.setFillColor(sf::Color::Black);
}

void Label::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}
