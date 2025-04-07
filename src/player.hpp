#pragma once
#include "entity.hpp"
#include "tile.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>


class Player : public Entity
{
public:
	Player(std::vector<Tile> *__map, int __mapWidth, sf::Vector2f* __mapOffset, float* _fogHeight, std::vector<sf::Sprite>* __mapSpritesPtr);
	~Player();

	void input(std::vector<std::optional<sf::Event>> events, float time);
	void process(float dt);
	void draw(sf::RenderWindow &window);
	sf::Vector2f* getPosPtr();

  	void setSpriteCyclePtr(int* which);

private:
	std::vector<Tile>* map;
	std::vector<sf::Sprite>* mapSpritesPtr;
	int mapWidth;

	sf::Vector2f* mapOffset;
  bool isDead = false;
	// actions
	// Jump
	bool anythingPressed = false;
  int buffer=0;
	bool jmping = false;
  bool hasJumped = false;
  bool jmpPressed = false;
	bool canJump = false;
	float jump_time= 0.;
  float jump_buffer = 0.0;
	    //Bomb
    bool bombing = false;
    bool bombSprite = false;
    int bombBuffer = 0;
    float boom_start = 0.5;
    float boom_time = 10.;
    float float_time = .3;
	bool boomPressed = false;
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
	sf::Vector2f pos{553.0, 300.0};
	const float SPEED_VAL = 224.0;
	sf::Vector2f speed = {0.0, 0.0};
	sf::Vector2f grav = {0.0, static_cast<float>(4 * (16.0 / 1.7) * 10)};
	float stepPerDirection[2] = {static_cast<float>(1.0), static_cast<float>(-1.0)};
	sf::Keyboard::Key keyPerDirection[2] = {
		sf::Keyboard::Key::Right,
		sf::Keyboard::Key::Left};
	sf::Texture texture;
	sf::Sprite *sprite;
  	sf::Vector2f spawnPoint{96,96};
	float speedXForStepBuf;

	// time
	sf::Clock myClock;
	float t0 = myClock.restart().asSeconds();
	float time_boom;
	float time_slide;
	int* spriteCyclePtr = nullptr;

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

	void mineDown();
	void mineRight();
	void mineLeft();

	void mineBotLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr);
	void mineBotRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr);
	void mineTopLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr);
	void mineTopRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr);

	void mineBotLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr , int otherX, int otherY);
	void mineBotRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY);
	void mineTopLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr , int otherX, int otherY);
	void mineTopRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY);

	void updateMapAfterMining(std::vector<sf::Vector2i> imposed);

	//fog
  float* fogHeight;
  bool fogCollision();
};
