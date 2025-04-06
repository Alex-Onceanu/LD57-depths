#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iterator>
#include <optional>
#include "player.hpp"
#include <iostream>
Player::Player()
{
    texture = sf::Texture("assets/miner_grid.png");

    sprite = new sf::Sprite(texture);
    sprite->setOrigin({8.0, 8.0}); 
    sprite->setTextureRect(sf::IntRect({0,0},{16,16}));
    g = 2*VERTICAL/(HANG_TIME*HANG_TIME);
    v0 = -2*VERTICAL/HANG_TIME;
}

Player::~Player()
{
  delete sprite;
}

void Player::input(std::vector<std::optional<sf::Event>> events)
{
    sf::Clock myClock;
    float t0 = myClock.restart().asSeconds();
    int jmpPressed = false;
    int boomPressed = false;
    jmping = false;
    anythingPressed = false;
  sf::Time time  = myClock.getElapsedTime();
    for(auto event: events){
   if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            if (keyPressed->scancode == sf::Keyboard::Scancode::Space )
            {
                jmpPressed = true;
                anythingPressed = true;
            }
            if (keyPressed->scancode == sf::Keyboard::Scancode::LShift )
      {
          boomPressed = true;
          anythingPressed = true;
      }
        }
    }
  if(jmpPressed && abs(pos.y-sol) <10 )
  {
    speed.y += v0;
    jmping = true;
    frames_since_jmp = 0;
  }
  
  for(int i = 0; i < 2; i++)
  {

      if(sf::Keyboard::isKeyPressed(keyPerDirection[i]))
      {
          currentDirection = i;
          speed.x = 100 * stepPerDirection[i];
          anythingPressed = true;
      }
  }
  if(boomPressed){
    speed.y = 0;
    speed.y= 1000;
      }
   frames_since_jmp++; 
    if(!anythingPressed){
    speed.x = 0;
  }
}

void Player::process(float dt)
{
      
      if(not anythingPressed)
      {
          timeSinceLastAnim = 0.0;
          currentFrame = 0;
      }
      else if(timeSinceLastAnim > FRAME_DURATION)
      {
          timeSinceLastAnim = 0.0;
          currentFrame = (1 + currentFrame) % NB_FRAMES;
      }

      speed.y += 2*grav.y*dt;    
      //speed.y -= 0.1*dt;

      timeSinceLastAnim += dt;
      pos += speed * dt;
      if(pos.y >sol){
        speed.y = (0.0f) * speed.y;
        pos.y = sol;   
      }
      int decal = 0;
      if(jmping || frames_since_jmp <=20 )
      {
          decal = 5;
          std::cout << "jmping" << std::endl;
      }
      sprite->setTextureRect(sf::IntRect({(currentFrame+decal) * spriteW, currentDirection * spriteW}, {spriteW, spriteW}));
      sprite->setPosition(pos);
}

void Player::draw(sf::RenderWindow& window)
{ 
    window.draw(*sprite);
}
