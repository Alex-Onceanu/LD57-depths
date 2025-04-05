#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "entity.hpp"

class World
{
public:
    World();
    ~World();

    void input();
    void process(float dt);
    void draw(sf::RenderWindow& window);

private:
    std::vector<Entity> entities;
};
