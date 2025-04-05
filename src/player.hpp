#pragma once
#include "entity.hpp"
#include <SFML/Graphics.hpp>

class Player: public Entity{
public:  
    Player();
    ~Player();
    void input();
    void process(float dt);
    void draw(sf::RenderWindow& window);
private:
    int spriteW = 64;
    const int NB_FRAMES = 4;
    int currentFrame = 0;
    int currentDirection = 0;
    const float FRAME_DURATION = 0.25;
    const float g = 9.81;
    float timeSinceLastAnim = 0.0;
    sf::Vector2f pos{ 100.0,110.0 };
    const float SPEED_VAL = 224.0;
    sf::Vector2f speed = { 50.0,0.0 };
    sf::Vector2f grav = {0.0, 8.0*(16.0/1.7) * g};

    sf::Texture texture;
    sf::Sprite sprite;
    

  
};
