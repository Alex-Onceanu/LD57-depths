#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

#include "wfc.hpp"
#include "tile.hpp"

Wfc::Wfc(int _tile_shapes, int _materials)
    : tile_shapes(_tile_shapes), materials(_materials)
{
    
}

void Wfc::update_one_step(std::vector<std::vector<Tile>> world, int x, int y, int* verified, int x_prec, int y_prec, int dir){ 
    if (!verified[y*width+x]){
        world[y*width+x] = world[y_prec*width+x_prec][0].compatible_list(world[y*width+x], dir);
        verified[y*width+x] = 1;
        for (int i = 0; i<4; i++){
            if(i !=(dir+2)%4){
                update_one_step(world, x+(1-i)*(i+1)%2, y+(-i)*(i)%2, verified, x, y, i);
            }
        }
    }
}

void Wfc::update(std::vector<std::vector<Tile>> world, int x, int y){
    int* verified = (int*)malloc(sizeof(int)*n);
    for (int i = 0; i<n; i++){
        verified[i] = 0;
    }
    verified[y*width+x] = 1;
    if (x == width-1){
        update_one_step(world, x-1, y, verified, x, y, 2);
    } else {
        update_one_step(world, x+1, y, verified, x, y, 0);
    }
}

bool Wfc::has_wfc_ended(std::vector<std::vector<Tile>> world){
    bool chosen = true;
    for (int i = 0; i < world.size(); i++){
        chosen = chosen && (world[i].size() == 1);
    }
    return chosen;
}

void Wfc::make_tile(std::pair<int,int> temp[4],int a, int b, int c, int d){
    std::pair<int,int> p1 = std::make_pair(b,a);
    std::pair<int,int> p2 = std::make_pair(c,b);
    std::pair<int,int> p3 = std::make_pair(d,c);
    std::pair<int,int> p4 = std::make_pair(a,d);
    temp[0] = p1;
    temp[1] = p2;
    temp[2] = p3;
    temp[3] = p4;
}

void Wfc::printTile(std::pair<int,int> temp[4]){
    std::cout << "("<<temp[0].first<<","<<temp[0].second<<"), "<<"("<<temp[1].first<<","<<temp[1].second<<"), "<<"("<<temp[2].first<<","<<temp[2].second<<"), "<<"("<<temp[3].first<<","<<temp[3].second<<")"<<std::endl;
}

std::vector<Tile> Wfc::wfc(int _width, int _height, int _n){
    width = _width;
    height = _height;
    n = _n;
    //srand(time(NULL)); à mettre dans le main
    std::vector<Tile> full;
    // for (int i = 0; i < materials*tile_shapes+1; i++){
    //     int temp_array[4] = {i, i, i, i};

    //     Tile temp(i, temp_array);
    //     full.push_back(temp);
    // }
    std::pair<int,int>* tileTypes[materials * tile_shapes + 1];
    std::pair<int,int> temp[4];
    for (int i = 0; i< materials + 1; i++){
        for (int j = 0; j< materials + 1; j++){
            for (int k = 0; k< materials + 1; k++){
                for (int l = 0; l< materials + 1; l++){
                    make_tile(temp,i,j,k,l);
                    tileTypes[i*(materials+1)*(materials+1)*(materials+1)+j*(materials+1)*(materials+1)+k*(materials+1)+l] = temp;
                }
            }
        }
    }

    for (int i = 0; i < materials * tile_shapes + 1; i++){
        Tile temp(i, tileTypes[i]);
        full.push_back(temp);
    }

    std::vector<std::vector<Tile>> world;
    //cout << sizeof(Tile) << std::endl;
    for (int i = 0; i < n; i++) {
        std::vector<Tile> temp = full;
        world.push_back(temp);
        //copy(full.begin(),full.end(), back_inserter(world[i]));
        //printf("world est de longueur %d\n", world.size());
    }
    while(!has_wfc_ended(world)){
        int k = rand() % n;
        
        int jsp = rand() % world.size();
        
        int chosen = rand() % static_cast<int>(world[jsp].size());
        
        Tile temp = world[jsp][chosen];
        
        world[jsp].clear();
        
        world[jsp].push_back(temp);
    }
    return convert(world);
}

std::vector<Tile> Wfc::convert(std::vector<std::vector<Tile>> world){
    std::vector<Tile> res;
    for (auto &e : world){
        res.push_back(e[0]);
    }
    return res;
}