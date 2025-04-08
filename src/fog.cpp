#include "SFML/Graphics.hpp"
#include "fog.hpp"
#include "world.hpp"

Fog::Fog(){
    texture = sf::Texture("./assets/fog.png");//exemple
    sprite = new sf::Sprite(texture, sf::IntRect({0,0}, {RES_X, RES_Y}));
    sprite->setPosition(pos);
    collision = sf::RectangleShape({RES_X, RES_Y});
    height = pos.y + RES_Y;

}

float* Fog::getHeight(){
    return &height;
}


void Fog::process(float dt){
    pos.y += 120 * dt;
    sprite->setPosition(pos);
    height += 120 * dt;
}

void Fog::draw(sf::RenderWindow& window){
    window.draw(*sprite);
}
