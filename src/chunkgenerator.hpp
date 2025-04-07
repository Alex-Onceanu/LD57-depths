#pragma once

#include "SFML/Graphics.hpp"

#include <memory>

#include "entity.hpp"
#include "tile.hpp"
#include "wfc.hpp"


constexpr int NB_TILES_X = 20;
constexpr int NB_TILES_Y = 24;
constexpr int DEPTH = 4;

class ChunkGenerator : public Entity {
public : 
    ChunkGenerator(sf::Vector2f* __playerPos, sf::Vector2f* __mapOffset, std::vector<Tile>* __map, std::vector<sf::Sprite>* __mapSprites, std::unique_ptr<Wfc> __wfcInstance);

    int* getSpriteCyclePtr();

    void input(std::vector<std::optional<sf::Event>> events, float time) {};
    void process(float dt);
    void draw(sf::RenderWindow& window) {};

private:
    void freeChunk();
    void generateChunk();

private: 
    sf::Vector2f* playerPos;
    sf::Vector2f* mapOffset;
    std::vector<Tile>* map;
    std::unique_ptr<Wfc> wfcInstance;
    std::vector<sf::Sprite>* mapSprites;

    int spriteCycle = 0;
};