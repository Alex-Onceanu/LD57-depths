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
	sf::Vector2f* getPosPtr();

private:
	std::vector<Tile>* map;
	int mapWidth;
	sf::Vector2f mapOffset;
	// actions
	// Jump
	bool anythingPressed = false;
  int buffer=0;
	bool jmping = false;
  bool hasJumped = false;
  bool jmpPressed = false;
	bool canJump = false;
	float jump_time= 0.;
  float jump_buffer = 0.1;
	    //Bomb
    bool bombing = false;
    bool bombSprite = false;
    int bombBuffer = 0;
    float boom_start = 0.5;
    float boom_time = 10.;
    float float_time = .3;
    float a_bomb;
    float b_bomb;
    //slide 
    bool sliding = false;
    float slide_start = 1.;
    float slide_time = .4;
    float slide_dist = 130.;
    float a_slide;
    float b_slide;

	// Sprite
	int spriteW = 32;
	const int NB_FRAMES = 3;
	int currentFrame = 0;
	int currentDirection = 0;
	int sens = (currentDirection == 1) ? -1 : 1;
	const float FRAME_DURATION = 0.1;
	// physics
	const float HANG_TIME = 36 * FRAME_DURATION;
	const float VERTICAL = 500.;

	bool gravity = true;
	float g;
	float v0;
	float timeSinceLastAnim = 0.0;
	sf::Vector2f pos{553.0, 100.0};
	const float SPEED_VAL = 224.0;
	sf::Vector2f speed = {0.0, 0.0};
	sf::Vector2f grav = {0.0, static_cast<float>(4 * (16.0 / 1.7) * 10)};
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
	void jumpAction(float t_j,float time);
	void bombAction(float tb, float time);
	void slideAction(float ts, float time);
	float slideEasing(float t);
  float bombEasing(float t);
  void loadTexture();

	bool collision();
	bool collisionDown();
	bool collisionRight();
	bool collisionUp();
	bool collisionLeft();
	
};
