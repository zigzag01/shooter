#include "bullet.h"
#include "const.h"

Bullet::Bullet(float x, float y) {
	texture.loadFromFile(IMAGES_FOLDER + LASER_RATE_FILE_NAME);
	sprite.setTexture(texture);
	sprite.setPosition(x, y);
}
void Bullet::update() {
	sprite.move(0.f, -LASER_SPEEDY);
}
size_t Bullet::getWidth() { return sprite.getLocalBounds().width; }
size_t Bullet::getHeight() { return sprite.getLocalBounds().height; }
sf::FloatRect Bullet::getHitBox() { return sprite.getGlobalBounds(); }
void Bullet::draw(sf::RenderWindow& window) { window.draw(sprite); }
sf::Vector2f Bullet::getPosition() { return sprite.getPosition(); }