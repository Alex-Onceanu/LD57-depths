#include <iostream>
#include <SFML/Grapfics.hpp>

#include "pickaxe.hpp"

Pickaxe::Pickaxe(sf::Vertex _hitbox[2]){
    hitbox[0] = _hitbox[0].copy();
    hitbox[1] = _hitbox[1].copy();
    texture = sf::Texture("assets/pickaxe_grid.png");

    sprite = new sf::Sprite(texture);
    sprite->setOrigin({8.0, 8.0}); 
    sprite->setTextureRect(sf::IntRect({0,0},{16,16}));
}

Pickaxe::~Pickaxe(){
    delete sprite;
}

void Pickaxe::input(std::vector<std::optional<sf::Event>> events){
    for(auto& e : events){
        if (e == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)){
            atckPressed = true;
            anythingPressed = true;
            frames_since_atck = 0;
        }
    }
}

void Pickaxe::process(float dt){
    if(timeSinceLastAnim > FRAME_DURATION)
    {
        timeSinceLastAnim = 0.0;
        currentFrame = (1 + currentFrame) % NB_FRAMES;
    }

    if (atckPressed){

    }

    timeSinceLastAnim += dt;
    sprite->setTextureRect(sf::IntRect({(currentFrame) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
    sprite->setPosition(pos);
}

void Player::draw(sf::RenderWindow& window)
{ 
    window.draw(*sprite);
}