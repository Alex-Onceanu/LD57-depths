#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>
#include <array>

#include "wfc.hpp"
#include "tile.hpp"

Wfc::Wfc(int _tile_shapes, int _materials)
    : tile_shapes(_tile_shapes), materials(_materials)
{
}

void Wfc::update_one_step(std::vector<std::vector<Tile>> world, int x, int y, int *verified, int x_prec, int y_prec, int dir)
{
    if (!has_wfc_ended(verified, n)){
        for(int i = 0; i < n; i++)
        {
            if(i % width == 0) std::cout << std::endl;
            std::cout << verified[i] << " ";
        }
        std::cout << std::endl;
        if (!verified[y * width + x] and 0 <=x and x<width and 0<=y and y<height)
        {
            std::cout << "not verified !" << std::endl;

            std::vector<Tile> newPossibilities;
            std::vector<int> idPossibilities;

            std::cout << "On veut acceder a l'element" << y_prec * width + x_prec << "d'un vector de taille " << world.size() << std::endl;

            for (auto &tl : world[y_prec * width + x_prec])
            {
                std::cout << "On veut acceder a l'element " << y * width + x << " d'un vecteur de taille " << world.size()<<std::endl;
                auto tv = tl.compatible_list(world[y * width + x], dir);
                std::cout << "J'AI DEJA PARTICIPE A CE JEU AVANT" << std::endl;

                for (auto &t2 : tv)
                {
                    if (std::find(idPossibilities.begin(), idPossibilities.end(), t2.getId()) == idPossibilities.end())
                    {
                        // std::cout << "On ajoute ";
                        // printTile(t2.sides);
                        // std::cout << "a new possibilities" << std::endl;
                        newPossibilities.push_back(t2);
                        idPossibilities.push_back(t2.getId());
                    }
                }
                std::cout << "le for est fini" << std::endl;
            }
            verified[y * width + x] = 1;
            // std::cout << "Taille finale de new possibilites : " << newPossibilities.size() << std::endl;

            for (int i = 0; i < 4; i++)
            {
                if (i != (dir + 2) % 4 and 0 <=x + (1 - i) * ((i + 1) % 2) and x + (1 - i) * ((i + 1) % 2)<width and 0<=y + (-i) * ((i) % 2) and y + (-i) * ((i) % 2)<height)
                {
                    update_one_step(world, x + (1 - i) * ((i + 1) % 2), y + (-i) * ((i) % 2), verified, x, y, i);
                }
            }
        }
    }
}

void Wfc::update(std::vector<std::vector<Tile>> world, int x, int y, int* verified)
{
    

    std::cout << "J'ai toujours pas planté après le malloc douteux" << std::endl;

    if (x == width - 1)
    {
        update_one_step(world, x - 1, y, verified, x, y, 2);
    }
    else
    {
        update_one_step(world, x + 1, y, verified, x, y, 0);
    }
}

bool Wfc::has_wfc_ended(int* verified, int n){
    bool chosen = true;
    for (int i = 0; i < n; i++)
    {
        chosen = chosen && (verified[i] == 1);
    }
    return chosen;
}

bool Wfc::has_wfc_ended(std::vector<std::vector<Tile>> world)
{
    bool chosen = true;
    for (int i = 0; i < world.size(); i++)
    {
        chosen = chosen && (world[i].size() == 1);
    }
    return chosen;
}

void Wfc::make_tile(std::vector<std::pair<int, int>> temp, int a, int b, int c, int d)
{
    std::pair<int, int> p1 = std::make_pair(b, a);
    std::pair<int, int> p2 = std::make_pair(c, b);
    std::pair<int, int> p3 = std::make_pair(d, c);
    std::pair<int, int> p4 = std::make_pair(a, d);
    temp.push_back(p1);
    temp.push_back(p2);
    temp.push_back(p3);
    temp.push_back(p4);
}

void Wfc::printTile(std::vector<std::pair<int, int>> temp)
{
    std::cout << "(" << temp[0].first << "," << temp[0].second << "), " << "(" << temp[1].first << "," << temp[1].second << "), " << "(" << temp[2].first << "," << temp[2].second << "), " << "(" << temp[3].first << "," << temp[3].second << ")" << std::endl;
}

std::vector<Tile> Wfc::wfc(int _width, int _height, int _n)
{
    width = _width;
    height = _height;
    n = _n;
    srand(time(NULL));
    std::vector<Tile> full;
    
    std::vector<std::vector<std::pair<int, int>>> tileTypes;


    for (int i = 0; i < materials + 1; i++)
    {
        for (int j = 0; j < materials + 1; j++)
        {
            for (int k = 0; k < materials + 1; k++)
            {
                for (int l = 0; l < materials + 1; l++)
                {
                    std::vector<std::pair<int, int>> temp;
                    std::cout << "coubeh" << std::endl;
                    make_tile(temp, i, j, k, l);
                    std::cout << "coubeh" << std::endl;
                    std::vector<std::pair<int, int>> arr;
                    std::cout << "coubeh" << std::endl;
                    for(int m = 0; m < 4; m++)
                    {
                        arr.push_back(std::make_pair(temp[m].first, temp[m].second));
                    }
                    std::cout << "coubeh" << std::endl;
                    tileTypes.push_back(arr);
                }
            }
        }
    }

    std::cout << "bouclesque, taille de la boucle " << materials * tile_shapes + 1 << std::endl;
    for (int i = 0; i < materials * tile_shapes + 1; i++)
    {
        Tile temp2(i, tileTypes[i]);
        std::cout << "tiletype[" << i << "] : ";
        printTile(tileTypes[i]);
        full.push_back(temp2);
    }

    std::cout << "taille de full : " << full.size()<<std::endl;
    std::cout << std::endl;

    std::vector<std::vector<Tile>> world;
    // cout << sizeof(Tile) << std::endl;
    for (int i = 0; i < n; i++)
    {
        std::vector<Tile> temp = full;
        world.push_back(temp);
        // copy(full.begin(),full.end(), back_inserter(world[i]));
        // printf("world est de longueur %d\n", world.size());
    }
    int *verified = new int[n];
    for (int i = 0; i < n; i++)
    {
        verified[i] = 0;
    }
    while (!has_wfc_ended(verified, n))
    {
        int k = rand() % n;

        int jsp = rand() % world.size();

        int chosen = rand() % static_cast<int>(world[jsp].size());

        Tile temp = world[jsp][chosen];
        std::cout << "Mon buffle, voici ton temp :";
        printTile(temp.sides);
        world[jsp].clear();
        std::cout << "Mon buffle, voici ton temp apres clear :";
        printTile(temp.sides);
        world[jsp].push_back(temp);

        std::cout << "Go update !" << std::endl;
        verified[jsp] = 1;
        update(world, jsp % width, jsp / width, verified);
    }
    std::cout << "Longueur de world : " << world.size() << std::endl;
    delete[] verified;

    return convert(world);
}

std::vector<Tile> Wfc::convert(std::vector<std::vector<Tile>> world)
{
    std::vector<Tile> res;
    for (auto &e : world)
    {
        res.push_back(e[0]);
    }
    return res;
}