#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#pragma once

class Tile{
    private : 
        std::pair<int,int> sides[4];
        int id;
        //string sprite; nom du .png
    public : 
        Tile();
        Tile(int id, std::pair<int,int> sides[4]); //string sprite
        bool compatible(Tile other, int dir); //dir = 0 si other est à droite, 1 si other est en dessous, 2 si other est à gauche, ou 3 si other est au dessus de this 
        std::vector<Tile> compatible_list(std::vector<Tile> t_list, int dir);
        sf::IntRect getRect();
};