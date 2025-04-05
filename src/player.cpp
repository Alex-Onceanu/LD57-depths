#include <optional>
#include "player.hpp"
Player::Player()
{
    texture = sf::Texture("assets/output.png");
    sprite = new sf::Sprite(texture);
    sprite->setOrigin({32.0, 32.0});
}

Player::~Player()
{
  delete sprite;
}

void Player::input()
{

}

void Player::process(float dt){
      speed.y += grav.y;    

      timeSinceLastAnim += dt;
      pos += speed * dt;
      if(pos.y > 768){
         speed = (-1.0f) * speed;
      }
      sprite->setTextureRect(sf::IntRect({0,0},{64,64}));
      sprite->setPosition(pos);
      
}

