#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#include "tile.hpp"

class Wfc {
public :
    Wfc(int _tile_shapes, int _materials);
    std::vector<Tile> wfc(int _width, int _height, int _n);
private :
    int tile_shapes;
    int materials;
    int width;
    int height;
    int n;
    void printTile(std::pair<int,int> temp[4]);
    void make_tile(std::pair<int,int> temp[4],int a, int b, int c, int d);
    bool has_wfc_ended(std::vector<std::vector<Tile>> world);
    void update(std::vector<std::vector<Tile>> world, int x, int y);
    void update_one_step(std::vector<std::vector<Tile>> world, int x, int y, int* verified, int x_prec, int y_prec, int dir);
    std::vector<Tile> convert(std::vector<std::vector<Tile>> world);

};