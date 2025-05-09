#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
#include <vector>
#include <memory>

#include "entity.hpp"
#include "tile.hpp"

constexpr int RES_X = 1024;
constexpr int RES_Y = 576;



class World 
{
public:
    World();
    ~World(){};

    void input(std::vector<std::optional<sf::Event>> events,float time);
    void process(float dt);
    void draw(sf::RenderWindow& window);

private:
    int initPath(std::vector<Tile>* initial);

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<Tile> map;
    std::vector<sf::Sprite> mapSprites;
    sf::Texture* mapTexs;
    sf::Vector2f mapOffset;
};
