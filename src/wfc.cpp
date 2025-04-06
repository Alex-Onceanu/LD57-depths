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
    if (!has_wfc_step_ended(verified, n)){
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
    if (!has_wfc_step_ended(verified, n)){
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

bool Wfc::in(std::vector<int> vec, int a){
    bool a_in_vec = false;
    for (int i = 0; i< vec.size(); i++){
        a_in_vec = a_in_vec or (vec[i] == a);
    }
    return a_in_vec;
}

std::vector<int> Wfc::intersect(std::vector<int> va, std::vector<int> vb){
    std::vector<int> res;
    for (int i = 0; i < va.size(); i++){
        if (in(vb,va[i])){
            res.push_back(va[i]);
        }
    }
    return res;
}

void Wfc::update2(std::vector<std::vector<Tile>>* world, int x, int y, int* verified, int n)
{
    

    //std::cout << "J'ai toujours pas planté après le malloc douteux" << std::endl;
    std::queue<int> visité;
    //std::queue<std::vector<int>> visité;
    visité.push(x+y*width);
    int dir;
    /*visité.push(std::vector<int>(x+width*y,dir));
    std::vector<int> prec_id = visité.front();
    if (prec_id[0]-1>=0 and !verified[prec_id[0]-1]){
        visité.push(std::vector<int>(prec_id[0]-1, 0));
        verified[prec_id[0]-1] = 1;
    } if((prec_id[0]%width)+1<width and !verified[prec_id[0]+1]){
        visité.push(std::vector<int>(prec_id[0]+1, 2));
        verified[prec_id[0]+1] = 1;
    } if(prec_id[0]-width>=0 and !verified[prec_id[0]-width]){
        visité.push(std::vector<int>(prec_id[0]-width, 1));
        verified[prec_id[0]-width] = 1;
    } if(prec_id[0]+width<width*height and !verified[prec_id[0]+width]){
        visité.push(std::vector<int>(prec_id[0]+width, 3));
        verified[prec_id[0]+width] = 1;
    }*/
    int prec_id = visité.front();
    if ((prec_id%width)-1>=0 and !verified[prec_id-1]){
        dir = 2;
        visité.push(prec_id-1);
        //verified[prec_id-1] = 1;
    } if((prec_id%width)+1<width and !verified[prec_id+1]){
        dir = 0;
        visité.push(prec_id+1);
        //verified[prec_id+1] = 1;
    } if(prec_id-width>=0 and !verified[prec_id-width]){
        visité.push(prec_id-width);
        //verified[prec_id-width] = 1;
    } if(prec_id+width<width*height and !verified[prec_id+width]){
        visité.push(prec_id+width);
        //verified[prec_id+width] = 1;
    }
    //visité.pop();

    while(!visité.empty()){
        int temp_int = visité.front();
        if (temp_int-1>=0 and !verified[temp_int-1]){
            visité.push(temp_int-1);
            //std::cout<<"on ajoute "<<temp_int-1<<std::endl;
            verified[temp_int-1] = 1;
        } if((temp_int%width)+1<width and !verified[temp_int+1]){
            visité.push(temp_int+1);
            //std::cout<<"on ajoute "<<temp_int+1<<std::endl;
            verified[temp_int+1] = 1;
        } if(temp_int-width>=0 and !verified[temp_int-width]){
            visité.push(temp_int-width);
            //std::cout<<"on ajoute "<<temp_int-width<<std::endl;
            verified[temp_int-width] = 1;
        } if(temp_int+width<width*height and !verified[temp_int+width]){
            visité.push(temp_int+width);
            //std::cout<<"on ajoute "<<temp_int+width<<std::endl;
            verified[temp_int+width] = 1;
        }
        std::vector<Tile> newPossibilities0;
        std::vector<int> idPossibilities0;
        std::vector<Tile> newPossibilities1;
        std::vector<int> idPossibilities1;
        std::vector<Tile> newPossibilities2;
        std::vector<int> idPossibilities2;
        std::vector<Tile> newPossibilities3;
        std::vector<int> idPossibilities3;

        //std::cout << "On veut acceder a l'element" << temp_int << "d'un vector de taille " << world.size() << std::endl;
        //std::cout<<"début du traitement de la case"<<std::endl;
        //std::cout<<"on note que (*world)[temp_int].size() = "<<(*world)[temp_int].size()<<std::endl;
        if ((temp_int)%width-1 >= 0){
            for (auto &tl : (*world)[temp_int])
            {
                auto tv1 = tl.compatible_list((*world)[temp_int-1], 2);

                for (auto &t2 : tv1)
                {
                    bool idt2_in_idPoss0 = false;
                    for (int i = 0; i < idPossibilities0.size(); i++){
                        if (idPossibilities0[i] == t2.getId()){
                            idt2_in_idPoss0 = true;
                        }
                    }
                    if (!idt2_in_idPoss0)
                    {
                        // std::cout << "On ajoute ";
                        // printTile(t2.sides);
                        // std::cout << "a new possibilities" << std::endl;
                        newPossibilities0.push_back(t2);
                        idPossibilities0.push_back(t2.getId());
                    }
                }
            }
            (*world)[temp_int-1] = newPossibilities0;
        } if ((temp_int)%width+1 < width){
            for (auto &tl : (*world)[temp_int])
            {
                auto tv2 = tl.compatible_list((*world)[temp_int+1], 0);

                for (auto &t2 : tv2)
                {
                    bool idt2_in_idPoss2 = false;
                    for (int i = 0; i < idPossibilities2.size(); i++){
                        if (idPossibilities2[i] == t2.getId()){
                            idt2_in_idPoss2 = true;
                        }
                    }
                    if (!idt2_in_idPoss2)
                    {
                        // std::cout << "On ajoute ";
                        // printTile(t2.sides);
                        // std::cout << "a new possibilities" << std::endl;
                        newPossibilities2.push_back(t2);
                        idPossibilities2.push_back(t2.getId());
                    }
                }
            }
            (*world)[temp_int+1] = newPossibilities2;
        } if ((temp_int)-width >= 0){
            int acc = 0;
            int count = 0;
            for (auto &tl : (*world)[temp_int])
            {
                count++;
                auto tv3 = tl.compatible_list((*world)[temp_int-width], 3);
                //std::cout<<"taille de tv3 : "<<tv3.size()<<std::endl;
                for (auto &t2 : tv3)
                {
                    bool idt2_in_idPoss1 = false;
                    for (int i = 0; i < idPossibilities1.size(); i++){
                        if (idPossibilities1[i] == t2.getId()){
                            idt2_in_idPoss1 = true;
                        }
                    }
                    if (!idt2_in_idPoss1)
                    {
                        // std::cout << "On ajoute ";
                        // printTile(t2.sides);
                        acc++;
                        //std::cout<<acc<<std::endl;
                        newPossibilities1.push_back(t2);
                        idPossibilities1.push_back(t2.getId());
                    }
                }
                //std::cout << "taille de newpossibilities1 : " << newPossibilities1.size() << std::endl;
            }
            (*world)[temp_int-width] = newPossibilities1;
            //std::cout<<"count : "<<count<<std::endl;
        } if ((temp_int)+width < width*height){
            for (auto &tl : (*world)[temp_int])
            {
                auto tv4 = tl.compatible_list((*world)[temp_int+width], 1);

                for (auto &t2 : tv4)
                {
                    bool idt2_in_idPoss3 = false;
                    for (int i = 0; i < idPossibilities3.size(); i++){
                        if (idPossibilities3[i] == t2.getId()){
                            idt2_in_idPoss3 = true;
                        }
                    }
                    if (!idt2_in_idPoss3)
                    {
                        // std::cout << "On ajoute ";
                        // printTile(t2.sides);
                        // std::cout << "a new possibilities" << std::endl;
                        newPossibilities3.push_back(t2);
                        idPossibilities3.push_back(t2.getId());
                    }
                }
            }
            (*world)[temp_int+width] = newPossibilities3;
        }
        //std::cout<<"fin du traitement de la case"<<std::endl;
        std::vector<Tile> newPossibilities;
        std::vector<int> idPossibilities;
        //std::cout << "idPos ont des tailles de "<<idPossibilities0.size()<<", "<<idPossibilities1.size()<<", "<<idPossibilities2.size()<<", "<<idPossibilities3.size()<<std::endl;
        for (int i = 0; i < idPossibilities0.size(); i++){
            int a = idPossibilities0[i];
            if (in(idPossibilities1,a) and in(idPossibilities2,a) and in(idPossibilities3,a)){
                idPossibilities.push_back(a);
                Tile last_temp = newPossibilities0[i].copy();
                newPossibilities.push_back(last_temp);
            }
        }
        //std::cout<<"idPossibilities a une longueur de "<<idPossibilities.size()<<std::endl;
        /*for (auto &tl : (*world)[prec_id])
        {
            auto tv = tl.compatible_list((*world)[temp_int], dir);

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
        }*/
        //std::cout<<"idPossibilities est de longueur "<<idPossibilities.size()<<std::endl;
        //std::cout<<"newPossibilities est de longueur "<<newPossibilities.size()<<std::endl;
        verified[prec_id] = 1;
        prec_id = temp_int;
        verified[temp_int] = 1;
        for (int i = 0; i<n; i++){
            if (i%width == 0){
                //std::cout<<std::endl;
            }
            //std::cout<<verified[i]<<" ";
            //std::cout<<(*world)[i].size()<<" ";
        }
        //std::cout<<std::endl<<std::endl<<std::endl;
        visité.pop();
    }
    /*while(!visité.empty()){
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

        for (auto &tl : (*world)[prec_id])
        {
            auto tv = tl.compatible_list((*world)[temp_int], dir);

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
    }*/
}

bool Wfc::has_wfc_step_ended(int* verified, int n){
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
        chosen = chosen && (world[i].size() <= 1);
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

int Wfc::pickMin(std::vector<std::vector<Tile>> world, std::vector<int>* depart_choisi){
    int minLen = materials * tile_shapes + 2;
    int acc = 0;
    for (int i = 0; i < world.size(); i++){
        if (world[i].size() < minLen and !(*depart_choisi)[i] and world[i].size()>0){
            minLen = world[i].size();
            acc = i;
        }
    }
    //std::cout<<"entropie minimale trouvée vaut "<<minLen<<std::endl;
    (*depart_choisi)[acc] = 1;
    return acc;
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

    /*for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            for (int k = 0; k < 2; k++)
            {
                for (int l = 0; l < 2; l++)
                {
                    std::vector<std::pair<int, int>> temp;
                    make_tile(&temp, i, j, k, l);
                    std::vector<std::vector<std::pair<int, int>>> arr;
                    for (int p = 0; p < materials ; p++){
                        arr.push_back(std::vector<std::pair<int,int>>());
                        //if (!(i==j==k==l==p==0)){
                            for(int m = 0; m < 4; m++)
                            {
                                std::pair<int,int> encore_temp = std::make_pair(temp[m].first*(p+1), temp[m].second*(p+1));
                                arr[p].push_back(encore_temp);
                            }
                            tileTypes.push_back(arr[p]);
                        //}
                    }
                }
            }
        }
    }
    std::cout<<"longueur de tileTypes : "<<tileTypes.size()<<std::endl;
    std::cout<<"ok ? "<<materials * tile_shapes + 1<<std::endl;
    Tile temp2(0, &tileTypes[0]);
    full.push_back(temp2);
    printTile(tileTypes[0]);*/
    for (int i = 0; i < materials * tile_shapes + 1; i++)
    {
        int x_temp = (i*materials)%(tile_shapes+1);
        int y_temp = i/(tile_shapes+1);
        Tile temp2(i, &tileTypes[i]);
        printTile(tileTypes[i]);
        //std::cout << "tiletype[" << i << "] : ";
        //printTile(tileTypes[i]);
        //std::cout<<"id de tileType["<<i<<"] : "<<temp2.getId()<<std::endl;
        //printTile(temp2.sides);
        full.push_back(temp2);
    }
    std::cout<<"taille de full : "<<full.size()<<std::endl;

   // std::cout << "taille de full : " << full.size()<<std::endl;
    //std::cout << std::endl;

    for (int i = 0; i < materials * tile_shapes + 1; i++){
       // std::cout << "id de full["<<i<<"] : "<<full[i].getId()<<std::endl;
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
            //std::cout<<"sous tour "<<m<<std::endl;
            arr.push_back(toujours_temp);
        }
        //std::cout<<"fin du tour "<<i<<std::endl;
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
    // int test = 14;
    // full[14].compatible_list(full, 0);
    // full[14].compatible_list(full, 1);
    // full[14].compatible_list(full, 2);
    // full[14].compatible_list(full, 3);
    std::vector<int> depart_choisi;
    for (int i = 0; i < world.size(); i++){
        depart_choisi.push_back(0);
    }
    while (!has_wfc_ended(world))
    {
        if (premier_tour){
            indice = rand()%world.size();
            premier_tour = false;
        } else {
            indice = pickMin(world, &depart_choisi);
            //indice = rand()%world.size();
        }

        int chosen = rand() % static_cast<int>(world[indice].size());
        // std::cout << "test2"<<std::endl;
        // std::cout<<"taille de world : "<<world.size()<<std::endl;
        // std::cout<<"taille de world[indice] : "<<world[indice].size()<<std::endl;
        // std::cout<<"indice vaut "<<indice<<std::endl;
        // std::cout<<"chosen : "<<chosen<<std::endl;

        Tile temp = world[indice][chosen];
        //std::cout << "Mon buffle, voici ton temp :";
        //printTile(temp.sides);
        world[indice].clear();
        //std::cout << "Mon buffle, voici ton temp apres clear :";
        //printTile(temp.sides);
        world[indice].push_back(temp);
        //std::cout<<"world[indice].size() = "<<world[indice].size()<<std::endl;
        //std::cout << "Go update !" << std::endl;
        verified[indice] = 1;
        update2(&world, indice % width, indice / width, verified, n);
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
        //std::cout<<"e est de longueur "<<e.size()<<std::endl;
        if(!e.size()){
            std::vector<std::pair<int, int>> solution_de_secours;
            for(int m = 0; m < 4; m++)
            {
                std::pair<int,int> encore_plus_schlag = std::make_pair(0, 0);
                solution_de_secours.push_back(encore_plus_schlag);
            }
            e.push_back(Tile(0, &solution_de_secours));
        }
        res.push_back(e[0]);
    }
    return res;
}