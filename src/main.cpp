#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>

constexpr int width = 5;
constexpr int height = 5;
constexpr int depth = 5;

class tile{
private : 
    int sides[4];
    int id;
    //string sprite; nom du .png
public : 
    tile(int id, int sides[4]); //string sprite
    bool compatible(tile other, int dir); //dir = 0 si other est à droite, 1 si other est en dessous, 2 si other est à gauche, ou 3 si other est au dessus de this 
    std::vector<tile> compatible_list(std::vector<tile> t_list, int dir);
};

tile::tile(int id, int sides[4]){
    id = id;
    for (int i = 0; i < 4; i++){
        sides[i] = sides[i];
    }
}

bool tile::compatible(tile other, int dir){
    return sides[dir]==other.sides[(dir+2)%4];
}

//  filtre t_list pour ne garder que les éléments compatibles avec this
std::vector<tile> tile::compatible_list(std::vector<tile> t_list, int dir) {
    std::vector<tile> comp;
    for (int i = 0; i < t_list.size(); i++) {
        if (compatible(t_list[i], dir)) {
            comp.push_back(t_list[i]);
        }
    }
    return comp;
}

int main()
{
    int i = 0;
    srand(time(NULL));
    printf("%d\n",i);
    i++;
    constexpr int size = width * height * depth;
    printf("%d\n",i);
    i++;
    std::vector<tile> full;
    printf("%d\n",i);
    i++;
    std::vector<std::vector<tile>> world;
    printf("%d\n",i);
    i++;
    //cout << sizeof(tile) << std::endl;
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
    printf("%d\n",i);
    i++;
    int chosen = rand() % world[jsp].size();
    tile temp = world[jsp][chosen];
    world[jsp].clear();
    world[jsp].push_back(temp);
    printf("taille de world[jsp] : %d\n", world[jsp].size());
    return 0;
}
