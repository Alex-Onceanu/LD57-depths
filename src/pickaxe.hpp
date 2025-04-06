#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "entity.hpp"

class Pickaxe : public Entity {
public:
    Pickaxe() {};
    Pickaxe(sf::Vertex _hitbox[2]);
    ~Pickaxe() {};

    void input(std::vector<std::optional<sf::Event>> events);
    void process(float dt);
    void draw(sf::RenderWindow& window);
private:
    sf::Texture texture;
    sf::Sprite* sprite;
    bool anythingPressed;
    bool atckPressed;
    int frames_since_atck;
    float timeSinceLastAnim;
    sf::Sprite* sprite;
    sf::Vertex hitbox[2];
}