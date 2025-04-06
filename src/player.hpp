#pragma once
#include "entity.hpp"
#include "tile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>

class Player : public Entity
{
public:
	Player(std::vector<Tile> *__map, int __mapWidth, sf::Vector2f __mapOffset);
	~Player();
	void input(std::vector<std::optional<sf::Event>> events, float time);
	void process(float dt);
	void draw(sf::RenderWindow &window);

private:
	std::vector<Tile>* map;
	int mapWidth;
	sf::Vector2f mapOffset;
	// actions
	// Jump
	bool anythingPressed = false;
	bool jmping = false;
	bool canJump = false;
	int frames_since_jmp = 100;
	// Bomb
	bool bombing = false;
	float boom_time = 0.5;
	float float_time = 0.11;
	// slide
	bool sliding = false;
	float slide_time = 1.;

	// Sprite
	int spriteW = 32;
	const int NB_FRAMES = 6;
	int currentFrame = 0;
	int currentDirection = 0;
	const float FRAME_DURATION = 0.1;
	// physics
	const float HANG_TIME = 36 * FRAME_DURATION;
	const float VERTICAL = 500.;
	int sol = 300;
	bool gravity = true;
	float g;
	float v0;
	float timeSinceLastAnim = 0.0;
	sf::Vector2f pos{553.0, 100.0};
	const float SPEED_VAL = 224.0;
	sf::Vector2f speed = {0.0, 0.0};
	sf::Vector2f grav = {0.0, static_cast<float>(5.0 * (16.0 / 1.7) * 10)};
	float stepPerDirection[2] = {static_cast<float>(1.0), static_cast<float>(-1.0)};
	sf::Keyboard::Key keyPerDirection[2] = {
		sf::Keyboard::Key::Right,
		sf::Keyboard::Key::Left};
	sf::Texture texture;
	sf::Sprite *sprite;
	// time
	sf::Clock myClock;
	float t0 = myClock.restart().asSeconds();
	float time_boom;
	float time_slide;
	// fonctions
	bool blocked(sf::Vector2f pos);
	float jumpAction(float time);
	void bombAction(float tb, float time);
	void slideAction(float ts, float time);
	
	bool collision();
	bool collisionDown();
	bool collisionRight();
	bool collisionUp();
	bool collisionLeft();
};
