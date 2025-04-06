#pragma once

#include <SFML/Graphics.hpp>

class Tile
{
public:
    Tile(int __topRight, int __botRight, int __botLeft, int __topLeft);

    int getTopRight();
    int getBotRight();
    int getBotLeft();
    int getTopLeft();

    sf::IntRect getRect();

    bool compatible(int myX, int myY, Tile other, int otherX, int otherY);
private:
    int topRight, botRight, botLeft, topLeft;
};
