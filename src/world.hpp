#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <memory>

#include "entity.hpp"
#include "tile.hpp"

constexpr int RES_X = 1366;
constexpr int RES_Y = 768;

class World 
{
public:
    World();
    ~World();

    void input(std::vector<std::optional<sf::Event>> events);
    void process(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Tile> map;
    std::vector<sf::Sprite> mapSprites;
    sf::Texture* mapTexs;
};
