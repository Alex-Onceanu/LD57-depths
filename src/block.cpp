#include <iostream>
#include <SFML/Graphics.hpp>

#include "block.hpp"

Block::Block(Tile t, int _index, sf::Sprite _sprite){
    body = t;
    index = _index;
    hitbox = sf::RectangleShape(16.f,16.f);
    sprite = _sprite;
}

bool Block::collision(sf::Vertex pickaxeHitbox[2]){
    return (hitbox.getPosition().x < pickaxeHitbox[0].x and pickaxeHitbox[0].x < hitbox.getPosition().x+hitbox.getSize().x and
        hitbox.getPosition().y < pickaxeHitbox[0].y and pickaxeHitbox[0].y < hitbox.getPosition().y+hitbox.getSize().y) or
        (hitbox.getPosition().x < pickaxeHitbox[1].x and pickaxeHitbox[1].x < hitbox.getPosition().x+hitbox.getSize().x and
        hitbox.getPosition().y < pickaxeHitbox[1].y and pickaxeHitbox[1].y < hitbox.getPosition().y+hitbox.getSize().y)
}

void Block::process(float dt){
    if(not anythingPressed)
    {
        timeSinceLastAnim = 0.0;
        currentFrame = 0;
    }
}

void Block::input(std::vector<std::optional<sf::Event>> events){
    for(auto& e : events){
        if (e == sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)){
            atckPressed = true;
            anythingPressed = true;
            frames_since_atck = 0;
        }
    }
}