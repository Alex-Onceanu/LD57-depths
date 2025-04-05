#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual void input();
    virtual void process(float dt);
    virtual void draw(sf::RenderWindow& window);
};
