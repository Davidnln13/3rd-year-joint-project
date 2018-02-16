	#pragma once
	#include <SFML\Graphics.hpp>
	#include "ResourceManager.h"

	extern ResourceManager resourceManager;

	class Label
	{
	public:
		Label(std::string textString, sf::Vector2f& position, sf::Font& font);

		void select();
		void deSelect();
		void draw(sf::RenderWindow& window);

		//Setters
		void setText(std::string text);
		void setText(std::string text, sf::Vector2f pos);
		void setText(std::string text, sf::Font& font, sf::Color color, sf::Vector2f origin);
		void setText(std::string text, sf::Font& font, sf::Color color = sf::Color::Black);
		void setOrigin(sf::Vector2f origin);
		void setPosition(sf::Vector2f pos);
		void setColor(sf::Color color);
		void setSize(int size);

		//Getters
		sf::Text& getText();
		sf::Vector2f getPos();

	private:
		sf::Text m_text;
		sf::Vector2f m_position;
	};