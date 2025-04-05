#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void input() = 0;
    virtual void process(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};
