#pragma once

#include <SFML/Graphics.hpp>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void input(std::vector<std::optional<sf::Event>> events, float time) = 0;
  
    virtual void process(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};
