#pragma once
#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>

//forward references so we can reference the box2d world and Pixel Per Metre variable
extern b2World world; //defining the box2d world object, saves us from passing it over to every objetc that needs it
extern float PPM;

class Boundary
{
public:
	Boundary(sf::Vector2f size, sf::Vector2f position);
	void update();
	void render(sf::RenderWindow& window);

private:
	b2Body* m_body;
	sf::RectangleShape m_rect;
};