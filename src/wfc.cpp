#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdio>
#include <array>
#include <queue>

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

            std::cout << "On veut acceder a l'element" << y_prec*width+x_prec << "d'un vector de taille " << world.size() << std::endl;

            for (auto &tl : world[y_prec*width+x_prec])
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
    if (!has_wfc_ended(verified, n)){
        if (x == width - 1)
        {
            update_one_step(world, x - 1, y, verified, x, y, 2);
        }
        else
        {
            update_one_step(world, x + 1, y, verified, x, y, 0);
        }
    }
}

void Wfc::update2(std::vector<std::vector<Tile>>* world, int x, int y, int* verified)
{
    

    //std::cout << "J'ai toujours pas planté après le malloc douteux" << std::endl;
    std::queue<int> visité;
    visité.push(x+y*width);
    int dir;
    if (x==width-1) dir = 2; else dir = 0;
    int prec_id = visité.front();
    if (prec_id-1>=0 and !verified[prec_id-1]){
        visité.push(prec_id-1);
        verified[prec_id-1] = 1;
    } if((prec_id%width)+1<width and !verified[prec_id+1]){
        visité.push(prec_id+1);
        verified[prec_id+1] = 1;
    } if(prec_id-width>=0 and !verified[prec_id-width]){
        visité.push(prec_id-width);
        verified[prec_id-width] = 1;
    } if(prec_id+width<width*height and !verified[prec_id+width]){
        visité.push(prec_id+width);
        verified[prec_id+width] = 1;
    }
    visité.pop();
    while(!visité.empty()){
        int temp_int = visité.front();
        if (temp_int-1>=0 and !verified[temp_int-1]){
            visité.push(temp_int-1);
            verified[temp_int-1] = 1;
        } if((temp_int%width)+1<width and !verified[temp_int+1]){
            visité.push(temp_int+1);
            verified[temp_int+1] = 1;
        } if(temp_int-width>=0 and !verified[temp_int-width]){
            visité.push(temp_int-width);
            verified[temp_int-width] = 1;
        } if(temp_int+width<width*height and !verified[temp_int+width]){
            visité.push(temp_int+width);
            verified[temp_int+width] = 1;
        }
        std::vector<Tile> newPossibilities;
        std::vector<int> idPossibilities;

        //std::cout << "On veut acceder a l'element" << temp_int << "d'un vector de taille " << world.size() << std::endl;

        for (auto &tl : (*world)[temp_int])
        {
            auto tv = tl.compatible_list((*world)[prec_id], dir);

            for (auto &t2 : tv)
            {
                bool idt2_in_idPoss = false;
                for (int i = 0; i < idPossibilities.size(); i++){
                    if (idPossibilities[i] == t2.getId()){
                        idt2_in_idPoss = true;
                    }
                }
                if (!idt2_in_idPoss)
                {
                    // std::cout << "On ajoute ";
                    // printTile(t2.sides);
                    // std::cout << "a new possibilities" << std::endl;
                    newPossibilities.push_back(t2);
                    idPossibilities.push_back(t2.getId());
                }
            }
        }
        std::cout<<"idPossibilities est de longueur "<<idPossibilities.size()<<std::endl;
        std::cout<<"newPossibilities est de longueur "<<newPossibilities.size()<<std::endl;
        (*world)[temp_int] = newPossibilities;
        prec_id = temp_int;
        visité.pop();
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

void Wfc::make_tile(std::vector<std::pair<int, int>>* temp, int a, int b, int c, int d)
{
    std::pair<int, int> p1 = std::make_pair(b, a);
    std::pair<int, int> p2 = std::make_pair(c, b);
    std::pair<int, int> p3 = std::make_pair(d, c);
    std::pair<int, int> p4 = std::make_pair(a, d);
    temp->push_back(p1);
    temp->push_back(p2);
    temp->push_back(p3);
    temp->push_back(p4);
}

void Wfc::printTile(std::vector<std::pair<int, int>> temp)
{
    std::cout << "(" << temp[0].first << "," << temp[0].second << "), " << "(" << temp[1].first << "," << temp[1].second << "), " << "(" << temp[2].first << "," << temp[2].second << "), " << "(" << temp[3].first << "," << temp[3].second << ")" << std::endl;
}

bool Wfc::isValid(int a, int b, int c, int d){
    if (a+b == 3 or a+c == 3 or a+d == 3 or b+c == 3 or b+d == 3 or c+d == 3){
        return false;
    }
    return true;
}

int Wfc::pickMin(std::vector<std::vector<Tile>> world, int* verified, int n){
    if (!has_wfc_ended(verified, n)){
        int minLen = materials * tile_shapes + 2;
        int acc = 0;
        for (int i = 0; i < world.size(); i++){
            if (world[i].size() < minLen && !verified[i]){
                minLen = world[i].size();
                acc = i;
            }
        }
        return acc;
    } return -1;
}

std::vector<Tile> Wfc::wfc(int _width, int _height, int _n)
{
    width = _width;
    height = _height;
    n = _n;
    srand(time(NULL));
    std::vector<Tile> full;
    std::cout<<"bon"<<std::endl;
    std::vector<std::vector<std::pair<int, int>>> tileTypes;
    int acc = 0;
    for (int i = 0; i < materials + 1; i++)
    {
        for (int j = 0; j < materials + 1; j++)
        {
            for (int k = 0; k < materials + 1; k++)
            {
                for (int l = 0; l < materials + 1; l++)
                {
                    if (isValid(i,j,k,l)){
                        acc++;
                        std::vector<std::pair<int, int>> temp;
                        make_tile(&temp, i, j, k, l);
                        std::vector<std::pair<int, int>> arr;
                        for(int m = 0; m < 4; m++)
                        {
                            std::pair<int,int> encore_temp = std::make_pair(temp[m].first, temp[m].second);
                            arr.push_back(encore_temp);
                        }
                        tileTypes.push_back(arr);
                    }
                }
            }
        }
    }
    std::cout<<"ok c'est un début"<<std::endl;
    for (int i = 0; i < materials * tile_shapes + 1; i++)
    {
        Tile temp2(i, &tileTypes[i]);
        //printTile(tileTypes[i]);
        std::cout << "tiletype[" << i << "] : ";
        printTile(tileTypes[i]);
        std::cout<<"id de tileType["<<i<<"] : "<<temp2.getId()<<std::endl;
        printTile(temp2.sides);
        full.push_back(temp2);
    }

    std::cout << "taille de full : " << full.size()<<std::endl;
    std::cout << std::endl;

    for (int i = 0; i < materials * tile_shapes + 1; i++){
        std::cout << "id de full["<<i<<"] : "<<full[i].getId()<<std::endl;
    }

    std::vector<std::vector<Tile>> world;
    // cout << sizeof(Tile) << std::endl;
    for (int i = 0; i < n; i++){
        //std::cout<<"n vaut "<<n<<std::endl;
        //world.push_back(std::vector<Tile>(full));
        std::vector<Tile> arr;
        //std::cout<<"tour "<<i<<std::endl;
        for(int m = 0; m < full.size(); m++)
        {
            Tile toujours_temp = full[m].copy();
            std::cout<<"sous tour "<<m<<std::endl;
            arr.push_back(toujours_temp);
        }
        std::cout<<"fin du tour "<<i<<std::endl;
        world.push_back(arr);
    }
    std::cout<<"enfin"<<std::endl;
    for (int i = 0; i < n; i++)
    {
        //std::vector<Tile> temp3 = full;
        //std::copy(full.begin(), full.end(), world[i].begin());
    }
    std::cout << "test"<<std::endl;
    int *verified = new int[n];
    for (int i = 0; i < n; i++)
    {
        verified[i] = 0;
    }
    bool premier_tour = true;
    int indice;
    while (!has_wfc_ended(verified, n))
    {
        if (premier_tour){
            indice = rand()%world.size();
            premier_tour = false;
        } else {
            indice = pickMin(world, verified, n);
        }

        int chosen = rand() % static_cast<int>(world[indice].size());
        std::cout << "test2"<<std::endl;
        std::cout<<"taille de world : "<<world.size()<<std::endl;
        std::cout<<"taille de world[indice] : "<<world[indice].size()<<std::endl;
        std::cout<<"indice vaut "<<indice<<std::endl;
        std::cout<<"chosen : "<<chosen<<std::endl;

        Tile temp = world[indice][chosen];
        std::cout << "Mon buffle, voici ton temp :";
        printTile(temp.sides);
        world[indice].clear();
        std::cout << "Mon buffle, voici ton temp apres clear :";
        printTile(temp.sides);
        world[indice].push_back(temp);

        std::cout << "Go update !" << std::endl;
        verified[indice] = 1;
        update2(&world, indice % width, indice / width, verified);
    }
    std::cout << "Longueur de world : " << world.size() << std::endl;
    delete[] verified;
    std::cout<<"ici world[0] est de longueur "<<world[0].size()<<std::endl;
    return convert(world);
}

std::vector<Tile> Wfc::convert(std::vector<std::vector<Tile>> world)
{
    std::vector<Tile> res;
    for (auto &e : world)
    {
        std::cout<<"e est de longueur "<<e.size()<<std::endl;
        res.push_back(e[0]);
    }
    return res;
}