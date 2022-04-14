#pragma once
#include "SFML/Graphics.hpp"
#include <list>


class Bullet {
private:
	sf::Sprite sprite;
	sf::Texture texture;
	float speedx;
	float speedy;
public:
	Bullet(float, float);
	void draw(sf::RenderWindow& window);
	void update();
	size_t getWidth();
	size_t getHeight();
	sf::FloatRect getHitBox();
	sf::Vector2f getPosition();
};