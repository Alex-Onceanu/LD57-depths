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
    void printTile(std::vector<std::pair<int, int>> temp);
private :
    int tile_shapes;
    int materials;
    int width;
    int height;
    int n;
    void make_tile(std::vector<std::pair<int,int>>* temp,int a, int b, int c, int d);
    bool has_wfc_ended(std::vector<std::vector<Tile>> world);
    bool has_wfc_step_ended(int* verified, int n);
    void update(std::vector<std::vector<Tile>> world, int x, int y, int* verified);
    void update2(std::vector<std::vector<Tile>>* world, int x, int y, int* verified, int n);
    void update_one_step(std::vector<std::vector<Tile>> world, int x, int y, int* verified, int x_prec, int y_prec, int dir);
    std::vector<Tile> convert(std::vector<std::vector<Tile>> world);
    bool isValid(int a, int b, int c, int d);
    int pickMin(std::vector<std::vector<Tile>> world, std::vector<int>* depart_choisi);
    bool in(std::vector<int> vec, int a);
    std::vector<int> intersect(std::vector<int> va, std::vector<int> vb);
};