#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#include "tile.hpp"

Tile::Tile(int id, int sides[4]){
    id = id;
    for (int i = 0; i < 4; i++){
        sides[i] = sides[i];
    }
}

bool Tile::compatible(Tile other, int dir){
    return sides[dir]==other.sides[(dir+2)%4];
}

//  filtre t_list pour ne garder que les éléments compatibles avec this
std::vector<Tile> Tile::compatible_list(std::vector<Tile> t_list, int dir) {
    std::vector<Tile> comp;
    for (int i = 0; i < t_list.size(); i++) {
        if (compatible(t_list[i], dir)) {
            comp.push_back(t_list[i]);
        }
    }
    return comp;
}