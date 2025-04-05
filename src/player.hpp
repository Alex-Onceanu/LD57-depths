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
    const float HANG_TIME = 1.;
    const float VERTICAL = 50.; 
    float g;
    float v0;
    float timeSinceLastAnim = 0.0;
    sf::Vector2f pos{ 100.0,100.0 };
    const float SPEED_VAL = 224.0;
    sf::Vector2f speed = { 0.0,0.0 };
    sf::Vector2f grav = {0.0, static_cast<float>(5.0*(16.0/1.7) * 10)};
    float stepPerDirection[2] = { static_cast<float>(1.0),static_cast<float>(-1.0)};    
    sf::Keyboard::Key keyPerDirection[2] = { 
        sf::Keyboard::Key::Right,
        sf::Keyboard::Key::Left
    };
    sf::Texture texture;
    sf::Sprite* sprite;
    

  
};
