#pragma once
/*
 * Name: Daryl Keogh
 * Date: 20/01/18
 * Description: Helper class for Sf::Vector2, this will help extend the functionality of the class
*/

//Including sf::vector2 so we can perform vector2 operations
#include "SFML\System\Vector2.hpp"

/**
* @description lerp interpolates from one vector to another
* @param a vector
* @param b vector
* @param t dt
* returns -> (1-t)*a + t*b
*/
static sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
	return sf::Vector2f((1-t)*a.x + t*b.x, (1 - t)*a.y + t*b.y);
}

/**
* @description distance simply calculates the distance between two vectors
* we use template arguments here so we can use this method with any vector type (int, float)
* @param a vector
* @param b vector
* returns -> a -b
*/
template<typename Vec2>
static Vec2 distance(const Vec2& a, const Vec2& b)
{
	return a - b;
}