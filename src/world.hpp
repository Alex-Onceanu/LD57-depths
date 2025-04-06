#pragma once

#include <SFML/Graphics.hpp>

#include <vector>
#include <memory>

#include "entity.hpp"
#include "wfc.hpp"
#include "tile.hpp"

constexpr int RES_X = 1366;
constexpr int RES_Y = 768;

constexpr int MAP_WIDTH = 12;
constexpr int MAP_HEIGHT = 48;

class World
{
public:
    World();
    ~World();

    void input();
    void process(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Tile> map;
    std::vector<sf::Sprite> mapSprites;
    //std::vector<sf::Texture> mapTextures;
    sf::Texture arrTextures[12*48];
};
