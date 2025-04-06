#include <algorithm>
#include <queue>
#include <cassert>
#include <iostream>
#include <ctime>

#include "wfc.hpp"

std::ostream& operator<<(std::ostream& out, Wfc w)
{
    out << "WFC : " << std::endl;
    for(int y = 0; y < w.getHeight(); y++)
    {
        for(int x = 0; x < w.getWidth(); x++)
        {
            Wave ww = w.getWaves()[w.get(x, y)];
            out << "[";
            for(Tile &e : ww.getTiles())
            {
                out << e.getTopRight() << e.getBotRight() << e.getBotLeft() << e.getTopLeft() << ",";
            }
            out << "] ";
        }
        out << std::endl;
    }
    return out;
}

Wave::Wave(int x, int y, int nbMaterials)
    : myX(x),
    myY(y)
{
    for(int mat = 1; mat < 1 + nbMaterials; mat++)
    {
        possibilities.push_back(Tile(0, 0, 0, 0));
        possibilities.push_back(Tile(0, 0, 0, mat));
        possibilities.push_back(Tile(0, 0, mat, 0));
        possibilities.push_back(Tile(0, 0, mat, mat));
        possibilities.push_back(Tile(0, mat, 0, 0));
        // possibilities.push_back(Tile(0, mat, 0, mat));
        possibilities.push_back(Tile(0, mat, mat, 0));
        possibilities.push_back(Tile(0, mat, mat, mat));
        possibilities.push_back(Tile(mat, 0, 0, 0));
        possibilities.push_back(Tile(mat, 0, 0, mat));
        // possibilities.push_back(Tile(mat, 0, mat, 0));
        possibilities.push_back(Tile(mat, 0, mat, mat));
        possibilities.push_back(Tile(mat, mat, 0, 0));
        possibilities.push_back(Tile(mat, mat, 0, mat));
        possibilities.push_back(Tile(mat, mat, mat, 0));
        possibilities.push_back(Tile(mat, mat, mat, mat));
    }
}

void Wave::collapseToOne()
{
    // choix arbitraire ici, changer les probas plus tard
    assert(possibilities.size() > 0);

    Tile choice = possibilities[rand() % possibilities.size()];
    possibilities.clear();

    // assert(copy != nullptr); ???

    possibilities.push_back(choice);
}

void Wave::collapseToOne(Tile choice)
{
    possibilities.clear();
    possibilities.push_back(choice);
}

std::vector<Tile> Wave::getTiles()
{
    return possibilities;
}

void Wave::collapseFromWave(Wave& target)
{
    std::vector<Tile> nextPossibilities;
    std::copy_if (possibilities.begin(), 
                possibilities.end(), 
                std::back_inserter(nextPossibilities), 
                [&](Tile t){ 
                    for(auto& t2 : target.getTiles())
                    {
                        if(t.compatible(myX, myY, t2, target.getX(), target.getY()))
                        {
                            return true;
                        }
                    }
                    return false;
                } );
    
    possibilities = nextPossibilities;
}

int Wave::getEntropy()
{
    return possibilities.size();
}

int Wave::getX()
{
    return myX;
}

int Wave::getY()
{
    return myY;
}

Tile Wave::getFirstTile()
{
    return possibilities[0];
}

int Wave::dist(Wave& other)
{
    return abs(myX - other.getX()) + abs(myY - other.getY());
}



// _______________________________________________________________________________
// ___________________________________WFC algorithm_______________________________
// _______________________________________________________________________________



Wfc::Wfc(int __nbMateriaux)
    : nbMateriaux(__nbMateriaux)
{

}

void Wfc::init(int w, int h, std::vector<Tile> initial)
{
    assert(initial.size() == w * h);

    width = w;
    height = h;

    waves.clear();

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            waves.push_back(Wave(x, y, nbMateriaux));
        }
    }

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            Tile t = initial[y * w + x];
            if(t.getTopRight() != -1)
            {
                waves[get(x, y)].collapseToOne(t);
                //propagate(waves[get(x, y)]);
            }
        }
    }
}

int Wfc::get(int x, int y)
{
    return y * width + x;
}

// Choisit la Wave d'entropie minimale, en cas d'égalité choisit la distance minimale avec center
Wave Wfc::getMinimalEntropy(Wave& center)
{
    assert(waves.size() > 0);

    int minEntropy = 80;              // valeur arbitrairement grande
    int minDist = width * height + 1; // ici aussi
    Wave argMin = waves[0];           // ici valeur osef;

    for(auto& w : waves)
    {
        int e = w.getEntropy();
        if((e == minEntropy and w.dist(center) < minDist) or (e < minEntropy and e > 1))
        {
            minEntropy = e;
            argMin = w;
            minDist = w.dist(center);
        }
    }

    return argMin;
}

std::vector<Wave> Wfc::getWaves()
{
    return waves;
}

int Wfc::getWidth()
{
    return width;
}

int Wfc::getHeight()
{
    return height;
}


void Wfc::propagate(Wave& start)
{
    std::vector<bool> visited;
    for(auto& w : waves)
    {
        visited.push_back(false);
    }

    Wave curr = start;
    visited[get(start.getX(), start.getY())] = true;
    waves[get(start.getX(), start.getY())] = start;

    std::queue<Wave> todo;
    todo.push(start);

    auto spread = [&](int lin){ 
        Wave other = waves[lin];

        if(not visited[lin])
        {
            int oldEntropy = other.getEntropy();
            other.collapseFromWave(curr);
            waves[lin] = other;
            if(other.getEntropy() != oldEntropy)
            {
                todo.push(other);
                visited[lin] = true;
            }
        }
    };

    while(not todo.empty())
    {
        curr = todo.front();
        todo.pop();
        int currX = curr.getX();
        int currY = curr.getY();

        if(currX > 0)           spread(get(currX - 1, currY));
        if(currX < width - 1)   spread(get(currX + 1, currY));
        if(currY > 0)           spread(get(currX, currY - 1));
        if(currY < height - 1)  spread(get(currX, currY + 1));
    }
}


//ptet modifie en place waves et renvoie void
void Wfc::prétraitement(int w, int h, std::vector<Tile> initial){
    init(w, h, initial);
    std::srand(std::time({}));
    int i = rand() % (w-4); // on enlève les murs d'épaisseur 2 sur les côtés, ils sont déterministes
    int j = rand() % 14; //pour le premier tour seulement
    std::cout<<"on choisit la tile "<<j<<" de la wave "<<i+2<<std::endl;
    Wave temp(w,h,1);
    for (int l = 0; l < (w)*h ; l++){
        if (l%(w) == 0){
            std::cout<<std::endl;
        }
        std::cout<<waves[l].getTiles().size()<<" ";
    }
    std::cout<<std::endl;
    waves[i+w+2].collapseToOne(temp.getTiles()[j]);
    std::cout<<"ça devrait changer bordel"<<std::endl;
    for (int l = 0; l < (w)*h ; l++){
        if (l%(w) == 0){
            std::cout<<std::endl;
        }
        std::cout<<waves[l].getTiles().size()<<" ";
    }
    std::cout<<std::endl;
    propagate(waves[i+w+2]);
    for (int l = 0; l < (w)*h ; l++){
        if (l%(w) == 0){
            std::cout<<std::endl;
        }
        std::cout<<waves[l].getTiles().size()<<" ";
    }
    std::cout<<std::endl;
    waves[i+2] = getMinimalEntropy(waves[i]);
    std::cout<<"jusqu'ici tout va bien, on fait "<<(w-4)*h<<" tours"<<std::endl;
    for (int l = 0; l < (w)*h ; l++){
        if (l%(w) == 0){
            std::cout<<std::endl;
        }
        std::cout<<waves[l].getTiles().size()<<" ";
    }
    std::cout<<std::endl;
    int depth = 1; // on continbue jusqu'à depth = h(-1 ?)
    while (depth < h){
        std::cout<<"un tour de plus, depth = "<<depth<<std::endl;
        int k = rand()%3; //direction, 0 = droite, 1 = bas, 2 = gauche
        if ((k == 2 and i == 0) or (k == 0 and i == w-5)){ //on est tout à gauche donc on peut pas aller plus à gauche, où tout à droite et mm problème
            k = 1;
        } 
        if (k==1){
            depth++;
            std::cout<<"on génère un entier entre 0 et "<<waves[i+(w-4)*depth].getTiles().size()<<", indice "<<i+(w-4)*depth<<std::endl;
            waves[i+w*depth].collapseToOne(waves[i+w*depth].getTiles()[rand() % waves[i+w*depth].getTiles().size()]);
            propagate(waves[i+w*depth]);
            waves[i+w*depth] = getMinimalEntropy(waves[i+w*depth]);
        } else if(k==0){
            i++;
            waves[i+w*depth].collapseToOne(waves[i+w*depth].getTiles()[rand() % waves[i+w*depth].getTiles().size()]);
            propagate(waves[i+w*depth]);
            waves[i+w*depth] = getMinimalEntropy(waves[i+w*depth]);
        } else {
            i--;
            waves[i+w*depth].collapseToOne(waves[i+w*depth].getTiles()[rand() % waves[i+w*depth].getTiles().size()]);
            propagate(waves[i+w*depth]);
            waves[i+w*depth] = getMinimalEntropy(waves[i+w*depth]);
        }
    }

}

std::vector<Tile> Wfc::collapse(int w, int h, std::vector<Tile> initial)
{
    init(w, h, initial);
    for (int l = 0; l < w*h ; l++){
        if (l%w == 0){
            std::cout<<std::endl;
        }
        std::cout<<waves[l].getTiles().size()<<" ";
    }
    std::cout<<std::endl;
    prétraitement(w,h,initial);
    Wave origin = getMinimalEntropy(waves[get(w / 2, h / 2)]);
    int currentEntropy = origin.getEntropy();
    while(currentEntropy > 1)
    {
        origin.collapseToOne();
        propagate(origin);
        origin = getMinimalEntropy(origin);
        currentEntropy = origin.getEntropy();
    }

    std::vector<Tile> res;
    for(auto& w : waves)
    {
        res.push_back(w.getFirstTile());
    }
    return res;
}

std::vector<Tile> Wfc::emptyTileset(int w, int h)
{
    std::vector<Tile> ans;

    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            ans.push_back(Tile());
        }
    }
    return ans;
}