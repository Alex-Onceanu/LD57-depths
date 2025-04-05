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

Tile Tile::copy(){
    std::vector<std::pair<int,int>> newSides;
    std::cout<<"bon"<<std::endl;
    for (int i = 0; i<sides.size(); i++){
        std::cout<<"HUH"<<std::endl;
        newSides.push_back(std::pair<int,int>(sides[i].first,sides[i].second));
    }
    std::cout<<"YES longueur "<<newSides.size()<<std::endl;

    Tile res(id, &newSides);
    std::cout<<"jusqu'ici ça va"<<std::endl;
    return res;
}

Tile::Tile(int _id, std::vector<std::pair<int,int>>* _sides){
    id = _id;
    std::cout<<"ceci ne peut valoir que 4 (cas 1) : "<<_sides->size()<<std::endl;
    //sides = _sides;
    for (int i = 0; i < 4; i++){
        std::cout<< "pour info ça vaut ";
        std::cout<<(*_sides)[i].first<<std::endl;
        std::cout<<"bruh"<<std::endl;
        sides.push_back(std::pair<int,int>((*_sides)[i].first, (*_sides)[i].second));
        //sides[i].first = (*_sides)[i].first;
        //sides[i].second = (*_sides)[i].second;
    }
}

Tile::Tile(int _id, std::vector<std::pair<int,int>> sides){
    id = _id;
    std::cout<<"ceci ne peut valoir que 4 (cas 2) : "<<sides.size()<<std::endl;
    //sides = _sides;
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
    // std::cout << "compatible list go !" << std::endl;
    for (int i = 0; i < t_list.size(); i++) {
        if (compatible(t_list[i], dir)) {
            comp.push_back(t_list[i]);
            // std::cout << "id : " << t_list[i].id << std::endl;
        }
    }
    //std::cout<<"len(comp) = "<<comp.size()<<std::endl;
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