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

void Tile::set(int topR, int botR, int botL, int topL)
{
    topRight = topR;
    botRight = botR;
    botLeft = botL;
    topLeft = topL;
}

sf::IntRect Tile::getRect()
{
    const int tileSize = 32;
    sf::IntRect ans(sf::Vector2i({ -1,-1 }),sf::Vector2i({ tileSize,tileSize }));

    int val = 0;
    int nbDiff = 0;
    if(topRight != val and topRight != 0)
    { 
        val = topRight;
        nbDiff++; 
    }
    if(botRight != val and botRight != 0)
    { 
        val = botRight;
        nbDiff++; 
    }
    if(botLeft != val and botLeft != 0)
    { 
        val = botLeft;
        nbDiff++; 
    }
    if(topLeft != val and topLeft != 0)
    { 
        val = topLeft;
        nbDiff++; 
    }

    if(nbDiff <= 1)
    {
        if(topRight)
        {
            if(botRight)
            {
                if(botLeft)
                {
                    if(topLeft)
                    {
                        ans.position = sf::Vector2i(15 * tileSize, (topRight - 1) * tileSize);
                    }
                    else
                    {
                        ans.position = sf::Vector2i(14 * tileSize, (topRight - 1) * tileSize);
                    }
                }
                else if(topLeft)
                {
                    ans.position = sf::Vector2i(13 * tileSize, (topRight - 1) * tileSize);
                }
                else
                {
                    ans.position = sf::Vector2i(12 * tileSize, (topRight - 1) * tileSize);
                }
            }
            else if(botLeft)
            {
                if(topLeft)
                {
                    ans.position = sf::Vector2i(11 * tileSize, (topRight - 1) * tileSize);
                }
                else
                {
                    ans.position = sf::Vector2i(10 * tileSize, (topRight - 1) * tileSize);
                }
            }
            else if(topLeft)
            {
                ans.position = sf::Vector2i(9 * tileSize, (topRight - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(8 * tileSize, (topRight - 1) * tileSize);
            }
        }
        else if(botRight)
        {
            if(botLeft)
            {
                if(topLeft)
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
                if(topLeft)
                {
                    ans.position = sf::Vector2i(5 * tileSize, (botRight - 1) * tileSize);
                }
                else
                {
                    ans.position = sf::Vector2i(4 * tileSize, (botRight - 1) * tileSize);
                }
            }
        }
        else if(botLeft)
        {
            if(topLeft)
            {
                ans.position = sf::Vector2i(3 * tileSize, (botLeft - 1) * tileSize);
            }
            else
            {
                ans.position = sf::Vector2i(2 * tileSize, (botLeft - 1) * tileSize);
            }
        }
        else if(topLeft)
        {
            ans.position = sf::Vector2i(1 * tileSize, (topLeft - 1) * tileSize);
        }
        else
        {
            ans.position = sf::Vector2i(0, 0);
        }
    }
    else
    {
        if(topLeft == 3 and topRight == 2)
        {
            ans.position = sf::Vector2i(1 * tileSize, 2 * tileSize);
        }
        else if(botLeft == 3 and botRight == 2)
        {
            ans.position = sf::Vector2i(2 * tileSize, 2 * tileSize);
        }
        else if(topRight == 2 and botRight == 2 and botLeft == 1)
        {
            ans.position = sf::Vector2i(3 * tileSize, 2 * tileSize);
        }
        else if(botLeft == 2 and botRight == 3)
        {
            ans.position = sf::Vector2i(4 * tileSize, 2 * tileSize);
        }
        else if(botLeft == 1 and topLeft == 2)
        {
            ans.position = sf::Vector2i(5 * tileSize, 2 * tileSize);
        }
        else if(botLeft == 3 and botRight == 3)
        {
            ans.position = sf::Vector2i(6 * tileSize, 2 * tileSize);
        }
        else if(topRight== 1 and botRight == 2)
        {
            ans.position = sf::Vector2i(7 * tileSize, 2 * tileSize);
        }
        else if(topRight == 3 and topLeft == 2)
        {
            ans.position = sf::Vector2i(8 * tileSize, 2 * tileSize);
        }
        else if(topRight == 3 and topLeft == 3)
        {
            ans.position = sf::Vector2i(9 * tileSize, 2 * tileSize);
        }
        else if(topLeft== 1 and botLeft == 2)
        {
            ans.position = sf::Vector2i(10 * tileSize, 2 * tileSize);
        }
        else if(topRight == 1 and botRight == 1 and botLeft == 2)
        {
            ans.position = sf::Vector2i(11 * tileSize, 2 * tileSize);
        }
        else if(botRight == 2 and botLeft == 1)
        {
            ans.position = sf::Vector2i(12 * tileSize, 2 * tileSize);
        }
        else if(botRight == 1 and botLeft == 2)
        {
            ans.position = sf::Vector2i(13 * tileSize, 2 * tileSize);
        }
        else if(topRight== 2 and botRight == 1)
        {
            ans.position = sf::Vector2i(14 * tileSize, 2 * tileSize);
        }
        else if(topRight == 1 and topLeft == 2)
        {
            ans.position = sf::Vector2i(15 * tileSize, 2 * tileSize);
        }
        else
        {
            std::cout << "Tile inconnue : " << topRight << " " << botRight << " " << botLeft << " " << topLeft << std::endl;
            ans.position = sf::Vector2i(0 * tileSize, 2 * tileSize);
            // throw std::runtime_error("Tile inconnue");
        }
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