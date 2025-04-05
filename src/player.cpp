#include <SFML/Window/Keyboard.hpp>
#include <optional>
#include "player.hpp"
Player::Player()
{
    texture = sf::Texture("assets/output.png");
    sprite = new sf::Sprite(texture);
    sprite->setOrigin({32.0, 32.0}); 
    g = 2*VERTICAL/(HANG_TIME*HANG_TIME);
    v0 = -2*VERTICAL/HANG_TIME;
}

Player::~Player()
{
  delete sprite;
}

void Player::input()
{

    bool anythingPressed = false;
    for(int i = 0; i < 2; i++)
    {
        if(sf::Keyboard::isKeyPressed(keyPerDirection[i]))
        {
            currentDirection = i;
            speed.x = 100 * stepPerDirection[i];
            anythingPressed = true;
        }
    }
    if(sf::Event::KeyPressed::Keyboard::Key::Space)
    {
       speed.y += v0;  
    }
    if(!anythingPressed){
    speed.x = 0;
  }
}

void Player::process(float dt)
{
      speed.y += grav.y*dt;    

      timeSinceLastAnim += dt;
      pos += speed * dt;
      if(pos.y > 300){
        speed.y = (0.0f) * speed.y;
        pos.y = 300;   
      }
      sprite->setTextureRect(sf::IntRect({0,0},{64,64}));
      sprite->setPosition(pos);
      
}

void Player::draw(sf::RenderWindow& window)
{ 
    window.draw(*sprite);
}

