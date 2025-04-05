#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    Entity() {};

    virtual void input();
    virtual void process(float dt);
    virtual void draw(sf::RenderWindow& window);
};
