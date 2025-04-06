#include "tile.hpp"

#include <iostream>

Tile::Tile()
{
    topRight = -1;
    botRight = -1;
    botLeft = -1;
    topLeft = -1;
}

Tile::Tile(int __topRight, int __botRight, int __botLeft, int __topLeft)
{
    topRight = __topRight;
    botRight = __botRight;
    botLeft = __botLeft;
    topLeft = __topLeft;
}

int Tile::getTopRight()
{
    return topRight;
}

int Tile::getBotRight()
{
    return botRight;
}

int Tile::getBotLeft()
{
    return botLeft;
}

int Tile::getTopLeft()
{
    return topLeft;
}

sf::IntRect Tile::getRect()
{
    const int tileSize = 32;
    const int base = 3;
    sf::IntRect ans(sf::Vector2i({ -1,-1 }),sf::Vector2i({ tileSize,tileSize }));

    if(topRight != 0)
    {
        if(botRight != 0)
        {
            if(botLeft != 0)
            {
                if(topLeft != 0)
                {
                    ans.position = sf::Vector2i(15 * tileSize, (topRight - 1) * tileSize);
                }
                else
                {
                    ans.position = sf::Vector2i(14 * tileSize, (topRight - 1) * tileSize);
                }
            }
            else if(topLeft != 0)
            {
                ans.position = sf::Vector2i(13 * tileSize, (topRight - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(12 * tileSize, (topRight - 1) * tileSize);
            }
        }
        else if(botLeft != 0)
        {
            if(topLeft != 0)
            {
                ans.position = sf::Vector2i(11 * tileSize, (topRight - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(10 * tileSize, (topRight - 1) * tileSize);
            }
        }
        else if(topLeft != 0)
        {
            ans.position = sf::Vector2i(9 * tileSize, (topRight - 1) * tileSize);
        }
        else
        {
            ans.position = sf::Vector2i(8 * tileSize, (topRight - 1) * tileSize);
        }
    }
    else if(botRight != 0)
    {
        if(botLeft != 0)
        {
            if(topLeft != 0)
            {
                ans.position = sf::Vector2i(7 * tileSize, (botRight - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(6 * tileSize, (botRight - 1) * tileSize);
            }
        }
        else
        {
            if(topLeft != 0)
            {
                ans.position = sf::Vector2i(5 * tileSize, (botRight - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(4 * tileSize, (botRight - 1) * tileSize);
            }
        }
    }
    else if(botLeft != 0)
    {
        if(topLeft != 0)
        {
            ans.position = sf::Vector2i(3 * tileSize, (botLeft - 1) * tileSize);
        }
        else
        {
            ans.position = sf::Vector2i(2 * tileSize, (botLeft - 1) * tileSize);
        }
    }
    else if(topLeft != 0)
    {
        ans.position = sf::Vector2i(1 * tileSize, (topLeft - 1) * tileSize);
    }
    else
    {
        ans.position = sf::Vector2i(0, 0);
    }
    return ans;
}

bool Tile::compatible(int myX, int myY, Tile other, int otherX, int otherY)
{
    if(myX == otherX)
    {
        if(myY == otherY + 1)
        {
            return topRight == other.getBotRight() and topLeft == other.getBotLeft();
        }
        else if(myY == otherY - 1)
        {
            return botRight == other.getTopRight() and botLeft == other.getTopLeft();
        }
        return false;
    }
    else if(myY == otherY)
    {
        if(myX == otherX + 1)
        {
            return topLeft == other.getTopRight() and botLeft == other.getBotRight();
        }
        else if(myX == otherX - 1)
        {
            return topRight == other.getTopLeft() and botRight == other.getBotLeft();
        }
        return false;
    }
    return false;
}