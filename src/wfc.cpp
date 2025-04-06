#include <algorithm>
#include <queue>
#include <cassert>
#include <iostream>

#include "wfc.hpp"

std::ostream& operator<<(std::ostream& out, Wfc w)
{
    out << "WFC : " << std::endl;
    for(int y = 0; y < w.getHeight(); y++)
    {
        for(int x = 0; x < w.getWidth(); x++)
        {
            Wave ww = w.getWaves()[w.get(x, y)];
            out << "[";
            for(Tile &e : ww.getTiles())
            {
                out << e.getTopRight() << e.getBotRight() << e.getBotLeft() << e.getTopLeft() << ",";
            }
            out << "] ";
        }
        out << std::endl;
    }
    return out;
}

Wave::Wave(int x, int y, int nbMaterials)
    : myX(x),
    myY(y)
{
    for(int mat = 1; mat < 1 + nbMaterials; mat++)
    {
        possibilities.push_back(Tile(0, 0, 0, 0));
        possibilities.push_back(Tile(0, 0, 0, mat));
        possibilities.push_back(Tile(0, 0, mat, 0));
        possibilities.push_back(Tile(0, 0, mat, mat));
        possibilities.push_back(Tile(0, mat, 0, 0));
        // possibilities.push_back(Tile(0, mat, 0, mat));
        possibilities.push_back(Tile(0, mat, mat, 0));
        possibilities.push_back(Tile(0, mat, mat, mat));
        possibilities.push_back(Tile(mat, 0, 0, 0));
        possibilities.push_back(Tile(mat, 0, 0, mat));
        // possibilities.push_back(Tile(mat, 0, mat, 0));
        possibilities.push_back(Tile(mat, 0, mat, mat));
        possibilities.push_back(Tile(mat, mat, 0, 0));
        possibilities.push_back(Tile(mat, mat, 0, mat));
        possibilities.push_back(Tile(mat, mat, mat, 0));
        possibilities.push_back(Tile(mat, mat, mat, mat));
    }
}

void Wave::collapseToOne()
{
    // choix arbitraire ici, changer les probas plus tard
    assert(possibilities.size() > 0);

    Tile choice = possibilities[rand() % possibilities.size()];
    possibilities.clear();

    // assert(copy != nullptr); ???

    possibilities.push_back(choice);
}

void Wave::collapseToOne(Tile choice)
{
    possibilities.clear();
    possibilities.push_back(choice);
}

std::vector<Tile> Wave::getTiles()
{
    return possibilities;
}

void Wave::collapseFromWave(Wave& target)
{
    std::vector<Tile> nextPossibilities;
    std::copy_if (possibilities.begin(), 
                possibilities.end(), 
                std::back_inserter(nextPossibilities), 
                [&](Tile t){ 
                    for(auto& t2 : target.getTiles())
                    {
                        if(t.compatible(myX, myY, t2, target.getX(), target.getY()))
                        {
                            return true;
                        }
                    }
                    return false;
                } );
    
    possibilities = nextPossibilities;
}

int Wave::getEntropy()
{
    return possibilities.size();
}

int Wave::getX()
{
    return myX;
}

int Wave::getY()
{
    return myY;
}

Tile Wave::getFirstTile()
{
    return possibilities[0];
}

Wfc::Wfc(int __width, int __height, int nbMaterials)
    : width(__width),
    height(__height)
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            waves.push_back(Wave(x, y, nbMaterials));
        }
    }

    assert(waves.size() == width * height);
}

int Wfc::get(int x, int y)
{
    return y * width + x;
}

Wave Wfc::getMinimalEntropy()
{
    assert(waves.size() > 0);

    int minEntropy = 80;
    Wave argMin = waves[0];    

    for(auto& w : waves)
    {
        int e = w.getEntropy();
        if((e == minEntropy and rand() % 2 == 1) or (e < minEntropy and e > 1))
        {
            minEntropy = e;
            argMin = w;
        }
    }

    return argMin;
}

std::vector<Wave> Wfc::getWaves()
{
    return waves;
}

int Wfc::getWidth()
{
    return width;
}

int Wfc::getHeight()
{
    return height;
}


bool Wfc::propagate()
{
    std::vector<bool> visited;
    for(auto& w : waves)
    {
        visited.push_back(false);
    }

    std::queue<Wave> todo;

    Wave start = getMinimalEntropy();
    if(start.getEntropy() == 1)
    {
        return true;
    }

    assert(start.getEntropy() > 1);

    visited[get(start.getX(), start.getY())] = true;

    start.collapseToOne();
    waves[get(start.getX(), start.getY())] = start;

    todo.push(start);

    while(not todo.empty())
    {
        Wave curr = todo.front();
        int currX = curr.getX();
        int currY = curr.getY();
        int lin = -1;

        if(currX > 0)
        {
            lin = get(currX - 1, currY);
            Wave other = waves[lin];

            if(not visited[lin])
            {
                int oldEntropy = other.getEntropy();
                other.collapseFromWave(curr);
                waves[lin] = other;
                if(other.getEntropy() != oldEntropy)
                {
                    todo.push(other);
                    visited[lin] = true;
                }
            }
        }
        if(currX < width - 1)
        {
            lin = get(currX + 1, currY);
            Wave other = waves[lin];

            if(not visited[lin])
            {
                int oldEntropy = other.getEntropy();
                other.collapseFromWave(curr);
                waves[lin] = other;
                if(other.getEntropy() != oldEntropy)
                {
                    todo.push(other);
                    visited[lin] = true;
                }
            }
        }
        if(currY > 0)
        {
            lin = get(currX, currY - 1);
            Wave other = waves[lin];

            if(not visited[lin])
            {
                int oldEntropy = other.getEntropy();
                other.collapseFromWave(curr);
                waves[lin] = other;
                if(other.getEntropy() != oldEntropy)
                {
                    todo.push(other);
                    visited[lin] = true;
                }
            }
        }
        if(currY < height - 1)
        {
            lin = get(currX, currY + 1);
            Wave other = waves[lin];

            if(not visited[lin])
            {
                int oldEntropy = other.getEntropy();
                other.collapseFromWave(curr);
                waves[lin] = other;
                if(other.getEntropy() != oldEntropy)
                {
                    todo.push(other);
                    visited[lin] = true;
                }
            }
        }

        todo.pop();
    }
    return false;
}

std::vector<Tile> Wfc::collapse()
{
    while(not propagate());

    std::vector<Tile> res;
    for(auto& w : waves)
    {
        res.push_back(w.getFirstTile());
    }
    return res;
}