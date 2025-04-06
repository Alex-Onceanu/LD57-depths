#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iterator>
#include <optional>
#include "player.hpp"
#include "tile.hpp"
#include <iostream>
#include <ostream>

Player::Player(std::vector<Tile> *__map, int __mapWidth, sf::Vector2f __mapOffset, float* _fogHeight)
	: map(__map),
	  mapOffset(__mapOffset),
	  mapWidth(__mapWidth)
{
	texture = sf::Texture("assets/miner_grid.png");
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
float Player::jumpAction(float time)
{
	speed.y = v0;
	// std::cout << "speedy is" << speed.y << std::endl;
	jmping = true;
	gravity = true;
	frames_since_jmp = 0;
	return time;
}
float Player::bombEasing(float time)
{
	return time;
}
void Player::bombAction(float t_b, float time)
{
	if (t_b < boom_time && bombing)
	{
		float t = (time - boom_start) / boom_time;
		t = bombEasing(t);
		float p = a_slide * t + (1 - t) * b_slide;
		pos.y = p;
		/*std::cout << 'a' << a_slide << ' '<< b_slide << t << std::endl;*/
	}
	// if (sliding && t_b >= slide_time)
	// {
	// 	sliding = false;
	// }
}

sf::Vector2f* Player::getPosPtr()
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
		pos.x = p;
		// std::cout << 'a' << a_slide << ' '<< b_slide << t << std::endl;
	}
	if (sliding && t_s >= slide_time)
	{
		sliding = false;
	}
}

bool Player::collisionLeft()
{
	return false;
}
bool Player::collisionUp()
{

	return false;
}
bool Player::collisionRight()
{
	return false;
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
	// (*map)[closestCenterj + closestCenteri * mapWidth] = Tile(1, 1, 1, 1);
	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

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

bool Player::collision()
{
	return collisionDown();
}

bool Player::fogCollision(){
  return *fogHeight > pos.y;
}

void Player::input(std::vector<std::optional<sf::Event>> events, float time)
{

	int jmpPressed = false;
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
				b_bomb = 1.;
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::RShift)
			{
				slidePressed = true;
				a_slide = pos.x;
				std::cout << sens << ' ' << currentDirection << std::endl;
				b_slide = a_slide + sens * slide_dist;
				anythingPressed = true;
			}
		}
	}
	// JUMP
	if (jmpPressed && canJump)
		float time_jmp = jumpAction(time);
	// BOOM
	if (boomPressed)
	{
		bombing = true;
		// std::cout << "boom" << std::endl;
		boom_time = time;
	}
	if (slidePressed)
	{
		sliding = true;
		slide_start = time;
	}
	float t_b = myClock.getElapsedTime().asSeconds() - boom_start;
	// Bomb action
	bombAction(t_b, time);
	// Slide action
	float t_s = myClock.getElapsedTime().asSeconds() - slide_start;
	slideAction(t_s, time);
	// Left and Right
	for (int i = 0; i < 2; i++)
	{

		if (sf::Keyboard::isKeyPressed(keyPerDirection[i]) && (slide_time - t_s < 0.23)) // pour qu'on puisse pas bouger avant quasi la fin du slide.
		{
			currentDirection = i;
			pos.x += 3 * stepPerDirection[i];
			anythingPressed = true;
		}
	}
	frames_since_jmp++;
	if (!anythingPressed)
	{
		speed.x = 0;
	}
}

void Player::process(float dt)
{
	sf::Vector2f oldPos = sf::Vector2f({pos.x, pos.y});
  if (fogCollision()){
    std::cout<<"ptdr chuis mort"<<std::endl;
    isDead = true;
  }
  if (isDead){
    sprite->setPosition(spawnPoint);
  }
  else {
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
    if (jmping || frames_since_jmp <= 20)
    {
      decal = 5;
      // std::cout << "jmping" << std::endl;
    }
    sprite->setTextureRect(sf::IntRect({(currentFrame + decal) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));

    if (collisionDown())
    {
      pos.y = oldPos.y;
      speed.y = 0.0;
      canJump = true;
      jmping = false;
    }
    else
    {
      canJump = false;
    }

    sprite->setPosition(pos);
  }
}

void Player::draw(sf::RenderWindow &window)
{
	window.draw(*sprite);
}
