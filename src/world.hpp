#pragma once

#include <SFML/Graphics.hpp>

#include <memory>
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
    std::vector<std::unique_ptr<Entity>> entities;
};
