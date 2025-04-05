#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#include "tile.hpp"

Tile::Tile()
{
    id = 0;
    for (int i = 0; i < 4; i++){
        sides[i].first = 0;
        sides[i].second = 0;
    }
}

Tile::Tile(int id, std::pair<int,int> sides[4]){
    id = id;
    for (int i = 0; i < 4; i++){
        sides[i].first = sides[i].first;
        sides[i].second = sides[i].second;
    }
}

bool Tile::compatible(Tile other, int dir){
    return  sides[dir].first == other.sides[(dir+2)%4].second 
        and sides[dir].second == other.sides[(dir+2)%4].first;
}

//  filtre t_list pour ne garder que les éléments compatibles avec this
std::vector<Tile> Tile::compatible_list(std::vector<Tile> t_list, int dir) {
    std::vector<Tile> comp;
    std::cout << "compatible list go !" << std::endl;
    for (int i = 0; i < t_list.size(); i++) {
        if (compatible(t_list[i], dir)) {
            comp.push_back(t_list[i]);
        }
    }
    return comp;
}

sf::IntRect Tile::getRect()
{
    return sf::IntRect( { 32 * ((id % 16) - 1) , 32 * id / 16 },{ 32,32 } );
}

int Tile::getId()
{
    return id;
}