#pragma once

#include "entity.hpp"

class Camera : public Entity
{
public:
    Camera(sf::Vector2f *__following);
    ~Camera() {};

    void input(std::vector<std::optional<sf::Event>> events, float time) {};
    void process(float dt);
    void draw(sf::RenderWindow& window);

    void setZoom(float coef);
private:
    sf::View view;
    sf::Vector2f *following;
};
