#pragma once

#include "entity.hpp"

class Camera : public Entity
{
public:
    Camera(sf::Vector2f *__following, sf::Vector2f resolution);
    ~Camera() {};

    void input();
    void process(float dt);
    void draw(sf::RenderWindow& window);

    sf::Vector2f getPos();
    void setZoom(float coef);
private:
    sf::View view;
    sf::Vector2f *following;
    sf::FloatRect rect;
};
