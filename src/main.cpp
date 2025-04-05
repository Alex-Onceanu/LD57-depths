#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#pragma once

#include "tile.hpp"


constexpr int width = 5;
constexpr int height = 5;

int main()
{
    int i = 0;
    srand(time(NULL));
    printf("%d\n",i);//0
    i++;
    constexpr int size = width * height;
    printf("%d\n",i);
    i++;
    std::vector<Tile> full;
    printf("%d\n",i);
    i++;
    std::vector<std::vector<Tile>> world;
    printf("%d\n",i);
    i++;
    //cout << sizeof(Tile) << std::endl;
    for (int i = 0; i < size; i++) {
        world.push_back({});
        world[i].assign(full.begin(),full.end());
        //copy(full.begin(),full.end(), back_inserter(world[i]));
        //printf("world est de longueur %d\n", world.size());
    }
    printf("%d\n",i);
    i++;
    int k = rand() % size;
    printf("%d\n",i);
    i++;
    int jsp = rand() % world.size();
    printf("jsp vaut %d\n", jsp);
    printf("%d\n",i);//6
    i++;
    long unsigned int sept2 = 7;
    int chosen = rand() % static_cast<int>(world[jsp].size());
    std::cout << 7 << std::endl;
    i++;
    Tile temp = world[jsp][chosen];
    printf("%d\n",i);
    i++;
    world[jsp].clear();
    printf("%d\n",i);
    i++;
    //world[jsp].push_back(temp);
    printf("%d\n",i);
    i++;
    printf("taille de world[jsp] : %ld\n", world[jsp].size());
    return 0;
}
