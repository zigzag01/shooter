#include "game.h"

Game::Game() :
	window(sf::VideoMode(
		static_cast<size_t> (WINDOW_WIDTH),
		static_cast<size_t> (WINDOW_HEIGHT)
	),
		WINDOW_TITLE,
		sf::Style::Titlebar | sf::Style::Close
	),
	player(WINDOW_WIDTH / 2 - 112 / 2.f,
		WINDOW_HEIGHT - 75.f, "images/playerShip2_green.png"),
	hp_text(500, 5, 24, sf::Color::Green)
{
	window.setFramerateLimit(60);
	meteor_sprites.reserve(METEORS_QTY);
	for (size_t i = 0; i < METEORS_QTY; i++) {
		meteor_sprites.push_back(new Meteor());
	}

}
void Game::play() {
	while (window.isOpen()) {
		check_events();
		update();
		check_collisions();
		draw();
	}
}
void Game::check_events() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) window.close();
		else
			if (event.type == sf::Event::MouseButtonPressed &&
				event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Time elapsed = clock.getElapsedTime();
				if (elapsed.asMilliseconds() > 250) {
					laser_sprites.push_back(new Laser(player.getPosition().x +
						player.getWidth() / 2 - 5, player.getPosition().y));
					clock.restart();
				}
			}

	}
}
void Game::update() {
	switch (game_state) {
	case PLAY:
		player.update();
		for (size_t i = 0; i < METEORS_QTY; i++) {
			meteor_sprites[i]->update();
		}
		for (auto it = laser_sprites.begin(); it != laser_sprites.end(); it++) {
			(*it)->update();
		}
		for (auto it = bonus_sprites.begin(); it != bonus_sprites.end(); it++) {
			(*it)->update();
		}
		for (auto it = exp_sprites.begin(); it != exp_sprites.end(); it++) {
			(*it)->update();
		}
		for (auto it = bullet_sprites.begin(); it != bullet_sprites.end(); it++) {          ///
			(*it)->update();
		}
		check_collisions();
		hp_text.update(std::to_string(static_cast<int>(player.getHp())));
		shield.update(player.getPosition());
		break;
	}
}
void Game::draw() {

	window.clear();
	switch (game_state) {

	case PLAY:
		for (size_t i = 0; i < METEORS_QTY; i++) {
			meteor_sprites[i]->draw(window);
		}
		for (auto it = laser_sprites.begin(); it != laser_sprites.end(); it++) {
			(*it)->draw(window);
		}
		for (auto it = bonus_sprites.begin(); it != bonus_sprites.end(); it++) {
			(*it)->draw(window);
		}
		for (auto it = bullet_sprites.begin(); it != bullet_sprites.end(); it++) {          ///
			(*it)->draw(window);
		}
		//щит
		shield.draw(window);
		for (auto it = exp_sprites.begin(); it != exp_sprites.end(); it++) {
			(*it)->draw(window);
		}
		player.draw(window);
		hp_text.draw(window);
		break;
	case GAME_OVER:
		window.draw(game_over.getSprite());
	}
	window.display();
}
void Game::check_collisions() {
	sf::Time now = shield_clock.getElapsedTime();
	for (size_t i = 0; i < METEORS_QTY; i++) {
		//метеоров с игроком
		if (player.getHitBox().intersects(
			meteor_sprites[i]->getHitBox()))
		{
			player.reduceHp(meteor_sprites[i]->getWidth() / 3);
			meteor_sprites[i]->spawn();
		}
		//метеоров со щитом
		if (meteor_sprites[i]->getHitBox().intersects(shield.getHitBox())) {
			meteor_sprites[i]->spawn();
		}
	}
	for (auto it = bonus_sprites.begin(); it != bonus_sprites.end(); it++) {
		if (player.getHitBox().intersects((*it)->getHitBox())) {
			if ((*it)->getType() == Bonus::HP) {
				player.reduceHp(-20);
				(*it)->setDel(true);
			}
			if ((*it)->getType() == Bonus::SHIELD) {
				shield_clock.restart();
				shield.setPosition(player.getPosition());
				shield.setVisible(true);
				(*it)->setDel(true);
			}
			if ((*it)->getType() == Bonus::FIRE_RATE) {
				//bullet_sprites.setPosition(player.getPosition());
				bullet_sprites.push_back(new Bullet(player.getPosition().x +           ///
					player.getWidth() / 2 - 5, player.getPosition().y));
				(*it)->setDel(true);
			}
		}
	}
	if (now.asSeconds() > SHIELD_VIS_TIME) {
		shield.setVisible(false);
		shield.setPosition(sf::Vector2f(0.f, WINDOW_HEIGHT + shield.getHeight()));
	}

	if (player.isDead()) game_state = GAME_OVER;
	bonus_sprites.remove_if([](Bonus* bonus) {
		return bonus->getDel(); });
	laser_sprites.remove_if([](Laser* laser) {
		return laser->getPosition().y < 0; });
	bonus_sprites.remove_if([](Bonus* bonus) {
		return bonus->getPosition().y > WINDOW_HEIGHT; });
	bonus_sprites.remove_if([](Bonus* shield) {
		return shield->getPosition().y > WINDOW_HEIGHT; });
	bonus_sprites.remove_if([](Bonus* bullet) {                     ///
		return bullet->getPosition().y > WINDOW_HEIGHT; });
	exp_sprites.remove_if([](Explosion* exp) {return exp->getDel(); });

	for (auto it = laser_sprites.begin(); it != laser_sprites.end(); it++) {
		for (size_t i = 0; i < METEORS_QTY; i++) {
			if ((*it)->getHitBox().intersects(meteor_sprites[i]->getHitBox()))
			{
				Explosion* new_explosion = new Explosion(meteor_sprites[i]->getCenter());
				exp_sprites.push_back(new_explosion);
				meteor_sprites[i]->spawn();

				size_t chance = rand() % 10000;
				if (chance < 500) {
					Bonus* new_bonus = new Bonus(static_cast<Bonus::BonusType>(0),
						meteor_sprites[i]->getPosition());
					bonus_sprites.push_back(new_bonus);
				}
				else if (chance >= 500 && chance < 800) {
					Bonus* new_bonus = new Bonus(static_cast<Bonus::BonusType>(1),
						meteor_sprites[i]->getPosition());
					bonus_sprites.push_back(new_bonus);
				}
				else if (chance >= 800 && chance < 10000) {
					Bonus* new_bonus = new Bonus(static_cast<Bonus::BonusType>(2),
						meteor_sprites[i]->getPosition());
					bonus_sprites.push_back(new_bonus);
				}
			}
		}
	}
	for (auto it = bullet_sprites.begin(); it != bullet_sprites.end(); it++) {
		for (size_t i = 0; i < METEORS_QTY; i++) {
			if ((*it)->getHitBox().intersects(meteor_sprites[i]->getHitBox()))
			{
				Explosion* new_explosion = new Explosion(meteor_sprites[i]->getCenter());
				exp_sprites.push_back(new_explosion);
				meteor_sprites[i]->spawn();
			}
		}
	}
}