#include <iostream>
#include <cassert>

#include "chunkgenerator.hpp"

ChunkGenerator::ChunkGenerator(sf::Vector2f* __playerPos, sf::Vector2f* __mapOffset, std::vector<Tile>* __map, std::vector<sf::Sprite>* __mapSprites, std::unique_ptr<Wfc> __wfcInstance, int __pathX)
    : playerPos(__playerPos),
    mapOffset(__mapOffset),
    map(__map),
    mapSprites(__mapSprites),
    pathX(__pathX)
{
    wfcInstance = std::move(__wfcInstance);
}

void ChunkGenerator::freeChunk()
{
    for(int i = 0; i < NB_TILES_X * DEPTH; i++)
    {
        map->erase(map->begin());
    }
    mapOffset->y += DEPTH * 32.0;
}

void ChunkGenerator::generateChunk()
{
    auto initial = wfcInstance->emptyTileset(NB_TILES_X, DEPTH + 1);
    for(int i = 0; i < NB_TILES_X; i++)
    {
        initial[i] = (*map)[i + map->size() - NB_TILES_X];
    }
    continuePath(&initial);
    std::vector<Tile> chunk = wfcInstance->collapse(NB_TILES_X, DEPTH + 1, initial);

    for(int i = 0; i < NB_TILES_X * DEPTH; i++)
    {
        map->push_back(chunk[i + NB_TILES_X]);

        sf::Sprite spr = (*mapSprites)[spriteCycle + i];
        spr.setTextureRect(chunk[i + NB_TILES_X].getRect());
        (*mapSprites)[spriteCycle + i] = spr;

        (*mapSprites)[spriteCycle + i].setPosition(*mapOffset + sf::Vector2f(32.0 * (i % NB_TILES_X), 32.0 * (NB_TILES_Y - DEPTH + (i / NB_TILES_X))));
    }
    spriteCycle += DEPTH * NB_TILES_X;
    spriteCycle %= NB_TILES_X * NB_TILES_Y;
}

int* ChunkGenerator::getSpriteCyclePtr()
{
    return &spriteCycle;
}

void ChunkGenerator::continuePath(std::vector<Tile>* initial)
{
    return;
    int depth = 4;
    int k;
    int k_prec = -1;
    int full = std::rand()%2;

    while(depth<DEPTH){
        k = std::rand()%3;
        if ((k == 0 and pathX == NB_TILES_X-5) or (k == 2 and pathX == 4) or (k == 2-k_prec)){
            k = 1;
        }
        if (k == 1){
            depth++;
        } else if(k == 0){
            pathX++;
        } else {
            pathX--;
        }

        pathX = std::max(pathX, 4);
        pathX = std::min(pathX, NB_TILES_X - 5);
        (*initial)[pathX+depth*NB_TILES_X].set(1,1,1,1);
        k_prec = k;
    }
}


void ChunkGenerator::process(float dt)
{
    if(playerPos->y > mapOffset->y + NB_TILES_Y * 0.5 * 32.0)
    {
        freeChunk();
        generateChunk();
    }
}