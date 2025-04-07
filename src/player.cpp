#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iterator>
#include <tgmath.h>
#include <optional>
#include "player.hpp"
#include "tile.hpp"
#include <iostream>
#include <ostream>

Player::Player(std::vector<Tile> *__map, int __mapWidth, sf::Vector2f __mapOffset, float *_fogHeight)
	: map(__map),
	  mapOffset(__mapOffset),
	  mapWidth(__mapWidth)
{
	texture = sf::Texture("assets/combined_image.png");
	fogHeight = _fogHeight;
	sprite = new sf::Sprite(texture);
	sprite->setOrigin({16.0, 16.0});
	sprite->setTextureRect(sf::IntRect({0, 0}, {32, 32}));
	g = 2 * VERTICAL / (HANG_TIME * HANG_TIME);
	v0 = -2 * VERTICAL / HANG_TIME;
}

Player::~Player()
{
	delete sprite;
}
bool Player::blocked(sf::Vector2f pos)
{
	return false; // TODO: a modifier quand collisios etc
}
float Player::slideEasing(float t)
{
	return (1 - t) * (1 - t) * (1 - t) * (1 - t);
}
float Player::bombEasing(float t)
{
	return slideEasing(t) * slideEasing(t);
}
void Player::jumpAction(float t_j, float time)
{
	if (t_j < jump_buffer)
	{
		jmping = true;
	}
	if (t_j > jump_buffer && !hasJumped)
	{
		speed.y = v0;
		hasJumped = true;
		// std::cout << "speedy is" << speed.y << std::endl;
		buffer = 0;
		gravity = true;
	}
}

void Player::bombAction(float t_b, float time)
{
	if (t_b < float_time && bombing)
	{
		float t = (time - boom_start) / float_time;
		t = slideEasing(t);
		float p = a_bomb * t + (1 - t) * b_bomb;
		pos.y = p;
		gravity = false;
	}
	if (bombing && t_b >= float_time)
	{
		speed.y = -3 * v0;
		gravity = true;
	}
	if (bombing && t_b >= boom_time)
	{
		bombing = false;
		gravity = true;
	}
}

sf::Vector2f *Player::getPosPtr()
{
	return &pos;
}

void Player::slideAction(float t_s, float time)
{
	if (jmping)
	{
		sliding = false; // pr que le slide n'arrive pas quand on retombe du sol
	}
	if (t_s < slide_time && sliding && !jmping)
	{
		float t = (time - slide_start) / slide_time;
		t = slideEasing(t);
		float p = a_slide * t + (1 - t) * b_slide;
		if (!collisionLeft() && !collisionRight())
		{
			pos.x = p;
		}
		else
		{
			pos.x = p;
		}
		// std::cout << 'a' << a_slide << ' '<< b_slide << t << std::endl;
	}
	if (sliding && t_s >= slide_time)
	{
		sliding = false;
	}
}

bool Player::collisionLeft()
{
	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x + 20.0 - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;
	// std::cout << "i : " << closestCenteri << "j : " << closestCenterj << std::endl;
	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
		// std::cout << "pas de collision on est en dehors de la map" << std::endl;
		return false;
	}
	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];

	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.y < closestCenterY)
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY - tileSize / 2.0) - pos.y))
		{
			return closestTile.getTopLeft() or closestTile.getBotLeft();
		}
		else
		{
			if ((closestCenteri - 1) < 0)
			{
				return closestTile.getTopLeft();
			}
			Tile topTile = (*map)[closestCenterj + (closestCenteri - 1) * mapWidth];
			return closestTile.getTopLeft() or topTile.getBotLeft() or closestTile.getBotLeft();
		}
	}
	else
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY + tileSize / 2.0) - pos.y))
		{
			return closestTile.getTopLeft() or closestTile.getBotLeft();
		}
		else
		{
			Tile botTile = (*map)[closestCenterj + (closestCenteri + 1) * mapWidth];
			return closestTile.getBotLeft() or botTile.getTopLeft();
		}
	}
}
bool Player::collisionUp()
{
	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;
	// std::cout << "i : " << closestCenteri << "j : " << closestCenterj << std::endl;
	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
		// std::cout << "pas de collision on est en dehors de la map" << std::endl;
		return false;
	}
	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];
	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.x > closestCenterX)
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX + tileSize / 2.0) - pos.x))
		{
			return closestTile.getTopLeft() or closestTile.getTopRight();
		}
		else
		{
			if ((closestCenterj + 1) >= mapWidth)
			{
				return closestTile.getTopRight();
			}
			Tile rightTile = (*map)[(closestCenterj + 1) + closestCenteri * mapWidth];
			return closestTile.getTopRight() or rightTile.getTopLeft();
		}
	}
	else
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX - tileSize / 2.0) - pos.x))
		{
			return closestTile.getTopLeft() or closestTile.getTopRight();
		}
		else
		{
			if ((closestCenterj - 1) < 0)
			{
				return closestTile.getTopLeft();
			}
			Tile rightTile = (*map)[(closestCenterj - 1) + closestCenteri * mapWidth];
			return closestTile.getTopLeft() or rightTile.getTopRight();
		}
	}
}
bool Player::collisionRight()
{
	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x + 9.0 - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;
	// std::cout << "i : " << closestCenteri << "j : " << closestCenterj << std::endl;
	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
		// std::cout << "pas de collision on est en dehors de la map" << std::endl;
		return false;
	}
	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];

	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.y < closestCenterY)
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY - tileSize / 2.0) - pos.y))
		{
			return closestTile.getTopRight() or closestTile.getBotRight();
		}
		else
		{
			if ((closestCenteri - 1) < 0)
			{
				return closestTile.getTopRight();
			}
			Tile topTile = (*map)[closestCenterj + (closestCenteri - 1) * mapWidth];
			return closestTile.getTopRight() or topTile.getBotRight() or closestTile.getBotRight();
		}
	}
	else
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY + tileSize / 2.0) - pos.y))
		{
			return closestTile.getTopRight() or closestTile.getBotRight();
		}
		else
		{
			Tile botTile = (*map)[closestCenterj + (closestCenteri + 1) * mapWidth];
			return closestTile.getBotRight() or botTile.getTopRight();
		}
	}
}

bool Player::collisionDown()
{
	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;
	// std::cout << "i : " << closestCenteri << "j : " << closestCenterj << std::endl;
	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
		// std::cout << "pas de collision on est en dehors de la map" << std::endl;
		return false;
	}
	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];
	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (closestTile.getBotLeft() and closestTile.getBotRight())
		return true;

	if (pos.x > closestCenterX)
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX + tileSize / 2.0) - pos.x))
		{
			return closestTile.getBotLeft() or closestTile.getBotRight();
		}
		else
		{
			if ((closestCenterj + 1) >= mapWidth)
			{
				return closestTile.getBotRight();
			}
			Tile rightTile = (*map)[(closestCenterj + 1) + closestCenteri * mapWidth];
			return closestTile.getBotRight() or rightTile.getBotLeft();
		}
	}
	else
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX - tileSize / 2.0) - pos.x))
		{
			return closestTile.getBotLeft() or closestTile.getTopRight();
		}
		else
		{
			if ((closestCenterj - 1) < 0)
			{
				return closestTile.getBotLeft();
			}
			Tile rightTile = (*map)[(closestCenterj - 1) + closestCenteri * mapWidth];
			return closestTile.getBotLeft() or rightTile.getBotRight();
		}
	}
}

bool Player::fogCollision()
{
	return *fogHeight > pos.y;
}

void Player::input(std::vector<std::optional<sf::Event>> events, float time)
{
	speedXForStepBuf = 0.0;
	jmpPressed = false;
	int boomPressed = false;
	int slidePressed = false;
	anythingPressed = false;
	sens = (currentDirection == 1) ? -1 : 1;

	for (auto event : events)
	{
		if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
		{
			if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
			{
				jmpPressed = true;
				anythingPressed = true;
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::LShift)
			{
				boomPressed = true;
				anythingPressed = true;
				a_bomb = pos.y;
				b_bomb = a_bomb - 15;
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::RShift)
			{
				slidePressed = true;
				a_slide = pos.x;
				// std::cout << sens << ' ' << currentDirection << std::endl;
				b_slide = a_slide + sens * slide_dist;
				anythingPressed = true;
			}
		}
	}
	// JUMP
	if (jmpPressed && canJump)
	{
		jmping = true;
		hasJumped = false;
		jump_time = time;
	}
	// BOOM
	if (boomPressed)
	{
		bombing = true;
		// std::cout << "boom" << std::endl;
		boom_start = time;
	}
	if (slidePressed)
	{
		sliding = true;
		slide_start = time;
	}
	// Bomb action
	float t_b = myClock.getElapsedTime().asSeconds() - boom_start;
	bombAction(t_b, time);
	// Jump action
	float t_j = myClock.getElapsedTime().asSeconds() - jump_time;
	jumpAction(t_j, time);
	// Slide action
	float t_s = myClock.getElapsedTime().asSeconds() - slide_start;
	slideAction(t_s, time);
	// Left and Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
	{
		pos.x = 553.0;
		pos.y = 100.0;
	}
	for (int i = 0; i < 2; i++)
	{

		if (sf::Keyboard::isKeyPressed(keyPerDirection[i]) && (slide_time - t_s < 0.23)) // pour qu'on puisse pas bouger avant quasi la fin du slide.
		{
			currentDirection = i;
			speedXForStepBuf += 180 * stepPerDirection[i];
			anythingPressed = true;
		}
	}
	if (!anythingPressed)
	{
		speed.x = 0;
	}
}
void Player::loadTexture()
{
	int decal = 0;
	int bombFrames = 15;
	if (!anythingPressed)
	{
		/*float y = (sens == 1) ? (0.) :(32.);*/
		sprite->setTextureRect(sf::IntRect({0, currentDirection * spriteW}, {spriteW, spriteW}));
	}
	bool spritejmp = jmping and buffer < 10;
	bool spriteslide = sliding;
	bool spritebomb = bombSprite || bombBuffer < bombFrames;
	if (spritejmp || spriteslide || spritebomb)
	{
		spritejmp = jmping and buffer < 10;
		spriteslide = sliding;
		spritebomb = bombSprite || bombBuffer < bombFrames;
		if (spritejmp)
		{
			buffer++;
			decal = buffer / 2;
			std::cout << "decal =" << decal << std::endl;
			sprite->setTextureRect(sf::IntRect({(5 + decal) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
			std::cout << "j" << std::endl;
		}
		if (spriteslide)
		{
			sprite->setTextureRect(sf::IntRect({(15) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
		}
		if (bombSprite || bombBuffer < bombFrames)
		{
			std::cout << "bopmb" << std::endl;
			bombSprite = false;
			bombBuffer++;
			sprite->setTextureRect(sf::IntRect({(15) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
		}
	}
	else
	{
		sprite->setTextureRect(sf::IntRect({(currentFrame + decal) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
	}
}
void Player::process(float dt)
{
	sf::Vector2f oldPos = sf::Vector2f({pos.x, pos.y});
	bool cright = collisionRight();
	bool cleft = collisionLeft();
	bool cup = collisionUp();

	if (not(speedXForStepBuf > 0.0 and cright) and not(speedXForStepBuf < 0.0 and cleft))
		pos.x += speedXForStepBuf * dt;
	if (fogCollision())
	{
		std::cout << "ptdr chuis mort" << std::endl;
		isDead = true;
	}
	if (isDead)
	{
		sprite->setPosition(spawnPoint);
	}
	else
	{
		if (not anythingPressed)
		{
			timeSinceLastAnim = 0.0;
			currentFrame = 0;
		}
		else if (timeSinceLastAnim > FRAME_DURATION)
		{
			timeSinceLastAnim = 0.0;
			currentFrame = (1 + currentFrame) % NB_FRAMES;
		}
		if (gravity)
		{
			speed.y += 2 * grav.y * dt;
		}
		timeSinceLastAnim += dt;
		pos += speed * dt;
		int decal = 0;

		if (collisionDown())
		{
			if (bombing)
			{
				bombSprite = true;
				bombBuffer = 0;
			}
			pos.y = oldPos.y;
			speed.y = 0.0;
			canJump = true;
			jmping = false;
			buffer = 0;
			bombing = false;
		}
		else
		{
			canJump = false;
		}
		loadTexture();
		sprite->setPosition(pos);
	}
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(*sprite);
}
