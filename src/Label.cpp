#include "Label.h"

Label::Label(std::string textString, sf::Vector2f& position):
	m_position(position.x + 55, position.y)
{
	m_text.setFont(resourceManager.getFontHolder()["oxinFont"]);
	m_text.setCharacterSize(30);
	m_text.setString(textString);
	m_text.setOrigin(m_text.getLocalBounds().width / 2.f, m_text.getLocalBounds().height);
	m_text.setFillColor(sf::Color::Black);
	m_text.setPosition(position);
}

void Label::draw(sf::RenderWindow & window)
{
	window.draw(m_text);
}
