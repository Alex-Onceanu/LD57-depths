#pragma once

#include <vector>

#include "tile.hpp"

class Wave
{
public:
    Wave(int x, int y);
    ~Wave();

    void collapseToOne();
    void collapseToOne(Tile choice);
    void collapseFromWave(Wave& target);
    int getEntropy();
    int getX();
    int getY();
    Tile getFirstTile();
    std::vector<Tile> getTiles();
    int dist(Wave& other);

private:
    std::vector<Tile> possibilities;
    int myX, myY;
};

class Wfc
{
public:
    Wfc();

    std::vector<Tile> collapse(int w, int h, std::vector<Tile> initial);
    int getWidth();
    int getHeight();
    std::vector<Wave> getWaves();
    int get(int x, int y);
    
    static std::vector<Tile> emptyTileset(int w, int h);

private:
    Wave getMinimalEntropy(Wave& center);
    void propagate(Wave& start,int w,int h);
    void init(int w, int h, std::vector<Tile> initial);

private:
    int width = 0;
    int height = 0;
    std::vector<Wave> waves;
};
