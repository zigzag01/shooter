#pragma once
#include "SFML/Graphics.hpp"
#include <list>
#include "bullet.h"
#include "const.h"

class ManyBullets {
private:
	std::list<Bullet*> bullets;
public:
	ManyBullets(float x, float y) {
		
		Bullet* p1 = new Bullet(x, y - p1->getWidth());
		bullets.push_back(p1);
		float dx = (SHIP_WIDTH - p1->getWidth()) / 4.f;
		Bullet* p2 = new Bullet(x + 1 * dx, y - p1->getWidth());
		bullets.push_back(p2);
		Bullet* p3 = new Bullet(x + 2 * dx, y - p1->getWidth());
		bullets.push_back(p3);
		Bullet* p4 = new Bullet(x + 3 * dx, y - p1->getWidth());
		bullets.push_back(p4);
		Bullet* p5 = new Bullet(x + 4 * dx, y - p1->getWidth());
		bullets.push_back(p5);
	}
	void update() {
		for (auto it = bullets.begin(); it != bullets.end(); it++) {
			(*it)->update();
		}
	}
	void draw(sf::RenderWindow& window) {
		for (auto it = bullets.begin(); it != bullets.end(); it++) {
			(*it)->draw(window);
		}
	}
};