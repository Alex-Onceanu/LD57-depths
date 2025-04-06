#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

class Tile{
    private : 
        int id;
        //string sprite; nom du .png
    public : 
        std::vector<std::pair<int,int>> sides;
        void printTile(std::vector<std::pair<int, int>> temp);
        Tile();
        Tile(int _id, std::vector<std::pair<int,int>> sides); //string sprite
        Tile(int _id, std::vector<std::pair<int,int>>* sides);
        Tile copy();
        bool compatible(Tile other, int dir); //dir = 0 si other est à droite, 1 si other est en dessous, 2 si other est à gauche, ou 3 si other est au dessus de this 
        std::vector<Tile> compatible_list(std::vector<Tile> t_list, int dir);
        sf::IntRect getRect();
        int getId();
};