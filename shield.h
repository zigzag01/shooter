#pragma once 
#include "SFML/Graphics.hpp"
#include "const.h"

class Shield {
private:
	sf::Sprite sprite;
	sf::Texture texture;
	bool del = false;
	bool visible = false;
public:
	Shield();
	void draw(sf::RenderWindow& window);
	void update(sf::Vector2f position);
	size_t getWidth();
	size_t getHeight();
	sf::FloatRect getHitBox();
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();
	bool getDel();
	void setDel(bool x);
	bool getVisible();
	void setVisible(bool x);
	void setPosition(sf::Vector2f pos);
};
