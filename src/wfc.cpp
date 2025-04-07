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

int Wave::dist(Wave& other)
{
    return abs(myX - other.getX()) + abs(myY - other.getY());
}



// _______________________________________________________________________________
// ___________________________________WFC algorithm_______________________________
// _______________________________________________________________________________



Wfc::Wfc(int __nbMateriaux)
    : nbMateriaux(__nbMateriaux)
{
    std::srand(std::time(nullptr));
}

Wave::~Wave()
{

}

void Wfc::init(int w, int h, std::vector<Tile> initial)
{
    assert(initial.size() == w * h);
    width = w;
    height = h;

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            waves.push_back(Wave(x, y, nbMateriaux));
        }
    }

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            Tile t = initial[y * w + x];
            if(t.getTopRight() != -1)
            {
                waves[get(x, y)].collapseToOne(t);
                propagate(waves[get(x, y)],w,h);
            }
        }
    }
}

int Wfc::get(int x, int y)
{
    return y * width + x;
}

// Choisit la Wave d'entropie minimale, en cas d'égalité choisit la distance minimale avec center
Wave Wfc::getMinimalEntropy(Wave& center)
{
    assert(waves.size() > 0);

    int minEntropy = 80;              // valeur arbitrairement grande
    int minDist = width * height + 1; // ici aussi
    Wave argMin = waves[0];           // ici valeur osef;

    for(auto& w : waves)
    {
        int e = w.getEntropy();
        if((e == minEntropy and w.dist(center) < minDist) or (e < minEntropy and e > 1))
        {
            minEntropy = e;
            argMin = w;
            minDist = w.dist(center);
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


void Wfc::propagate(Wave& start, int w, int h)
{
    std::vector<bool> visited;
    for(auto& w : waves)
    {
        visited.push_back(false);
    }

    Wave curr = start;
    visited[get(start.getX(), start.getY())] = true;
    waves[get(start.getX(), start.getY())] = start;
    for(int j = 0; j < w; j++)
    {
        visited[j] = true;
    }
    for (int i = 1; i < h; i++)
    {
        visited[i*w] = true;
        visited[(i+1)*w-1] = true;
    }
    std::queue<Wave> todo;
    todo.push(start);

    auto spread = [&](int lin){ 
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
    };

    while(not todo.empty())
    {
        curr = todo.front();
        todo.pop();
        int currX = curr.getX();
        int currY = curr.getY();

        if(currX > 0)           spread(get(currX - 1, currY));
        if(currX < width - 1)   spread(get(currX + 1, currY));
        if(currY > 0)           spread(get(currX, currY - 1));
        if(currY < height - 1)  spread(get(currX, currY + 1));
    }
}

std::vector<Tile> Wfc::collapse(int w, int h, std::vector<Tile> initial)
{
    init(w, h, initial);

    std::rand();
    int seed = get(std::rand() % w, std::rand() % h);

    Wave origin = getMinimalEntropy(waves[seed]);
    int currentEntropy = origin.getEntropy();
    while(currentEntropy > 1)
    {
        origin.collapseToOne();
        propagate(origin,w,h);
        origin = getMinimalEntropy(origin);
        currentEntropy = origin.getEntropy();
    }

    std::vector<Tile> res;
    for(auto& w : waves)
    {
        res.push_back(w.getFirstTile());
    }
    waves.clear();

    return res;
}

std::vector<Tile> Wfc::emptyTileset(int w, int h)
{
    std::vector<Tile> ans;

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            ans.push_back(Tile());
        }
    }
    return ans;
}