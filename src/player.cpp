#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iterator>
#include <optional>
#include "player.hpp"
#include "tile.hpp"
#include <iostream>
#include <algorithm>
#include <ostream>

Player::Player(std::vector<Tile> *__map, int __mapWidth, sf::Vector2f __mapOffset, float *_fogHeight, std::vector<sf::Sprite>* __mapSpritesPtr)
	: map(__map),
	  mapOffset(__mapOffset),
	  mapWidth(__mapWidth),
	  mapSpritesPtr(__mapSpritesPtr)
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
	}
	// if (sliding && t_b >= slide_time)
	// {
	// 	sliding = false;
	// }
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
		pos.x = p;
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

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
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

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{

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

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{

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

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size())
	{
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
			return closestTile.getBotLeft() or closestTile.getBotRight();
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


void Player::mineBotLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getBotLeft() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), old.getBotRight(), 0, old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	imposedPtr->push_back({ x - 1,y });
	imposedPtr->push_back({ x,y + 1 });
	imposedPtr->push_back({ x - 1,y + 1 });
}

void Player::mineBotLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getBotLeft() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), old.getBotRight(), 0, old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	if(x - 1 != otherX and y != otherY) imposedPtr->push_back({ x - 1,y });
	if(x != otherX and y + 1 != otherY) imposedPtr->push_back({ x,y + 1 });
	if(x - 1 != otherX and y + 1 != otherY) imposedPtr->push_back({ x - 1,y + 1 });
}

void Player::mineBotRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getBotRight() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), 0, old.getBotLeft(), old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	imposedPtr->push_back({ x + 1,y });
	imposedPtr->push_back({ x,y + 1 });
	imposedPtr->push_back({ x + 1,y + 1 });
}

void Player::mineBotRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getBotRight() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), 0, old.getBotLeft(), old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	if(x + 1 != otherX and y != otherY) imposedPtr->push_back({ x + 1,y });
	if(x != otherX and y + 1 != otherY) imposedPtr->push_back({ x,y + 1 });
	if(x + 1 != otherX and y + 1 != otherY) imposedPtr->push_back({ x + 1,y + 1 });
}

void Player::mineTopLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getTopLeft() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), old.getBotRight(), old.getBotLeft(), 0);
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	imposedPtr->push_back({ x - 1,y });
	imposedPtr->push_back({ x,y - 1 });
	imposedPtr->push_back({ x - 1,y - 1 });
}

void Player::mineTopLeft(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getTopLeft() == 2) return;
	(*map)[x + y * mapWidth] = Tile(old.getTopRight(), old.getBotRight(), old.getBotLeft(), 0);
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;

	if(x - 1 != otherX and y != otherY) imposedPtr->push_back({ x - 1,y });
	if(x != otherX and y - 1 != otherY) imposedPtr->push_back({ x,y - 1 });
	if(x - 1 != otherX and y - 1 != otherY) imposedPtr->push_back({ x - 1,y - 1 });
}

void Player::mineTopRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getTopRight() == 2) return;
	(*map)[x + y * mapWidth] = Tile(0, old.getBotRight(), old.getBotLeft(), old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;
	
	imposedPtr->push_back({ x - 1,y });
	imposedPtr->push_back({ x,y - 1 });
	imposedPtr->push_back({ x - 1,y - 1 });
}

void Player::mineTopRight(int x, int y, std::vector<sf::Vector2i>* imposedPtr, int otherX, int otherY)
{
	Tile old = (*map)[x + y * mapWidth];
	if(old.getTopRight() == 2) return;
	(*map)[x + y * mapWidth] = Tile(0, old.getBotRight(), old.getBotLeft(), old.getTopLeft());
	
	sf::Sprite spr = (*mapSpritesPtr)[y * mapWidth + x];
	spr.setTextureRect((*map)[y * mapWidth + x].getRect());
	(*mapSpritesPtr)[y * mapWidth + x] = spr;
	
	if(x + 1 != otherX and y != otherY) imposedPtr->push_back({ x + 1,y });
	if(x != otherX and y - 1 != otherY) imposedPtr->push_back({ x,y - 1 });
	if(x + 1 != otherX and y - 1 != otherY) imposedPtr->push_back({ x + 1,y - 1 });
}

void Player::updateMapAfterMining(std::vector<sf::Vector2i> imposed)
{
	if(imposed.size() <= 0) return;

	for(sf::Vector2i& v : imposed)
	{
		(*map)[v.y * mapWidth + v.x] = Tile();
	}

	for(sf::Vector2i& v : imposed)
	{
		int a = (*map)[v.y * mapWidth + v.x + 1].getTopLeft();
		int b = (*map)[v.y * mapWidth + v.x + 1].getBotLeft();
		int c = (*map)[(v.y + 1) * mapWidth + v.x].getTopRight();
		int d = (*map)[(v.y + 1) * mapWidth + v.x].getTopLeft();
		int e = (*map)[v.y * mapWidth + v.x - 1].getBotRight();
		int f = (*map)[v.y * mapWidth + v.x - 1].getTopRight();
		int g = (*map)[(v.y - 1) * mapWidth + v.x].getBotLeft();
		int h = (*map)[(v.y - 1) * mapWidth + v.x].getBotRight();

		int topRight = h != -1 ? h : a;
		int botRight = c != -1 ? c : b;
		int botLeft  = d != -1 ? d : e;
		int topLeft  = g != -1 ? g : f;

		assert(topRight != -1 and botRight != -1 and botLeft != -1 and topLeft != -1);

		(*map)[v.y * mapWidth + v.x].set(topRight, botRight, botLeft, topLeft);

		sf::Sprite spr = (*mapSpritesPtr)[v.y * mapWidth + v.x];
		spr.setTextureRect((*map)[v.y * mapWidth + v.x].getRect());
		(*mapSpritesPtr)[v.y * mapWidth + v.x] = spr;
	}
}

void Player::mineDown() 
{
	std::vector<sf::Vector2i> imposed;

	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x - mapOffset.x) / tileSize;
	int closestCenteri = 1 + static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size()) return;

	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];
	// if(closestTile.getTopRight() == 0 and closestTile.getBotRight() == 0 and closestTile.getTopLeft() == 0 and closestTile.getBotLeft() == 0) return;

	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.x > closestCenterX)
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX + tileSize / 2.0) - pos.x))
		{
			mineBotLeft(closestCenterj, closestCenteri, &imposed);
			mineBotRight(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			if ((closestCenterj + 1) >= mapWidth)
			{
				mineBotRight(closestCenterj, closestCenteri, &imposed);
			}
			else
			{
				mineBotRight(closestCenterj, closestCenteri, &imposed, closestCenterj + 1, closestCenteri);
				mineBotLeft(closestCenterj + 1, closestCenteri, &imposed, closestCenterj, closestCenteri);
			}
		}
	}
	else
	{
		if (std::abs(closestCenterX - pos.x) < std::abs((closestCenterX - tileSize / 2.0) - pos.x))
		{
			mineBotLeft(closestCenterj, closestCenteri, &imposed);
			mineBotRight(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			if ((closestCenterj - 1) < 0)
			{
				mineBotLeft(closestCenterj, closestCenteri, &imposed);
			}
			else
			{
				mineBotLeft(closestCenterj, closestCenteri, &imposed, closestCenterj - 1, closestCenteri);
				mineBotRight(closestCenterj - 1, closestCenteri, &imposed, closestCenterj, closestCenteri);
			}
		}
	}

	updateMapAfterMining(imposed);
}

void Player::mineRight()
{
	std::vector<sf::Vector2i> imposed;

	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x + 25.0 - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size()) return;

	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];
	if(closestTile.getTopRight() == 0 and closestTile.getBotRight() == 0 and closestTile.getTopLeft() == 0 and closestTile.getBotLeft() == 0) return;

	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.y < closestCenterY)
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY - tileSize / 2.0) - pos.y))
		{
			mineTopRight(closestCenterj, closestCenteri, &imposed);
			mineBotRight(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			if ((closestCenteri - 1) < 0)
			{
				mineTopRight(closestCenterj, closestCenteri, &imposed);
			}
			else
			{
				mineTopRight(closestCenterj, closestCenteri, &imposed, closestCenterj, closestCenteri - 1);
				mineBotRight(closestCenterj, closestCenteri - 1, &imposed, closestCenterj, closestCenteri);
			}
		}
	}
	else
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY + tileSize / 2.0) - pos.y))
		{
			mineTopRight(closestCenterj, closestCenteri, &imposed);
			mineBotRight(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			mineBotRight(closestCenterj, closestCenteri, &imposed, closestCenterj, closestCenteri + 1);
			mineTopRight(closestCenterj, closestCenteri + 1, &imposed, closestCenterj, closestCenteri);
		}
	}

	updateMapAfterMining(imposed);
}

void Player::mineLeft()
{
	std::vector<sf::Vector2i> imposed;

	const int tileSize = 32;
	int closestCenterj = static_cast<int>(pos.x + 20.0 - mapOffset.x) / tileSize;
	int closestCenteri = static_cast<int>(pos.y + tileSize / 2.0 - mapOffset.y) / tileSize;

	if (closestCenterj < 0 or closestCenteri < 0 or closestCenterj >= mapWidth or closestCenterj + (closestCenteri + 1) * mapWidth >= map->size()) return;
	
	Tile closestTile = (*map)[closestCenterj + closestCenteri * mapWidth];
	if(closestTile.getTopRight() == 0 and closestTile.getBotRight() == 0 and closestTile.getTopLeft() == 0 and closestTile.getBotLeft() == 0) return;

	float closestCenterX = closestCenterj * tileSize + mapOffset.x;
	float closestCenterY = closestCenteri * tileSize + mapOffset.y;

	if (pos.y < closestCenterY)
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY - tileSize / 2.0) - pos.y))
		{
			mineTopLeft(closestCenterj, closestCenteri, &imposed);
			mineBotLeft(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			if ((closestCenteri - 1) < 0)
			{
				mineTopLeft(closestCenterj, closestCenteri, &imposed);
			}
			mineTopLeft(closestCenterj, closestCenteri, &imposed, closestCenterj, closestCenteri - 1);
			mineBotLeft(closestCenterj, closestCenteri - 1, &imposed, closestCenterj, closestCenteri);
		}
	}
	else
	{
		if (std::abs(closestCenterY - pos.y) < std::abs((closestCenterY + tileSize / 2.0) - pos.y))
		{
			mineTopLeft(closestCenterj, closestCenteri, &imposed);
			mineBotLeft(closestCenterj, closestCenteri, &imposed);
		}
		else
		{
			mineBotLeft(closestCenterj, closestCenteri, &imposed, closestCenterj, closestCenteri + 1);
			mineTopLeft(closestCenterj, closestCenteri + 1, &imposed, closestCenterj, closestCenteri);
		}
	}

	updateMapAfterMining(imposed);
}


void Player::input(std::vector<std::optional<sf::Event>> events, float time)
{
	speedXForStepBuf = 0.0;
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

				b_slide = a_slide + sens * slide_dist;
				anythingPressed = true;
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::C)
			{
				if(currentDirection == 0) mineRight();
				else mineLeft();
			}
			if (keyPressed->scancode == sf::Keyboard::Scancode::X)
			{
				mineDown();
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
			speedXForStepBuf += 180 * stepPerDirection[i];
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
	bool cright = collisionRight();
	bool cleft = collisionLeft();
	bool cup = collisionUp();

	if (not(speedXForStepBuf > 0.0 and cright) and not(speedXForStepBuf < 0.0 and cleft))
		pos.x += speedXForStepBuf * dt;
	if (fogCollision())
	{
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
		if (jmping || frames_since_jmp <= 20)
		{
			decal = 5;
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
