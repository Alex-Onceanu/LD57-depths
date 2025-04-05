#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#include "tile.hpp"
#include "wfc.hpp"


constexpr int width = 5;
constexpr int height = 5;
constexpr int n = 25;
constexpr int tile_shapes = 15;
constexpr int materials = 1;



int main()
{
    Wfc test(tile_shapes, materials);
    test.wfc(width, height, n);
    return 0;
}
