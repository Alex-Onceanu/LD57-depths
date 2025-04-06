#pragma once

#include <vector>

#include "tile.hpp"

class Wave
{
public:
    Wave(int x, int y, int nbMaterials);

    void collapseToOne();
    void collapseToOne(Tile choice);
    void collapseFromWave(Wave& target);
    int getEntropy();
    int getX();
    int getY();
    Tile getFirstTile();
    std::vector<Tile> getTiles();

private:
    std::vector<Tile> possibilities;
    int myX, myY;
};

class Wfc
{
public:
    Wfc(int __width, int __height, int nbMaterials);

    std::vector<Tile> collapse();
    int getWidth();
    int getHeight();
    std::vector<Wave> getWaves();
    int get(int x, int y);

private:
    Wave getMinimalEntropy();
    bool propagate();

private:
    std::vector<Wave> waves;
    int width, height;
};
