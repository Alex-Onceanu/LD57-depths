#pragma once
#include "SFML/Graphics.hpp"
#include "entity.hpp"
class Fog : public Entity {
public : 
    Fog();
    ~Fog(){};

    void input(std::vector<std::optional<sf::Event>> events, float time) {};
    void process(float dt);
    void draw(sf::RenderWindow& window);
    float* getHeight();
private : 
    sf::Vector2f pos{0, -800};
    sf::Texture texture;
    sf::RectangleShape collision;
    sf::Sprite* sprite;
    float height;
};