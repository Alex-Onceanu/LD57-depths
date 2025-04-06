#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iterator>
#include <optional>
#include "player.hpp"
#include <iostream>
#include <ostream>
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
bool Player::blocked(sf::Vector2f pos){
  return false; //TODO: a modifier quand collisios etc
}

float Player::jumpAction(float time)
{
    speed.y = v0;
    std::cout << "speedy is" << speed.y << std::endl;
    jmping = true;
    gravity = true;
    frames_since_jmp = 0;
    return time; 
}
void Player::bombAction(float t_b,float time){
 if( t_b<boom_time && bombing)
  {
    
    if(pos.y>=sol )
    {
      bombing = false; 
      gravity = true;
    }
    if(t_b<float_time)
    {
      {
        speed.y = 0.;
        gravity = false;
      }
    }

    else{
      speed.y+= 100;
      gravity = true;
    }
    speed.x = 0.;
  }
}
void Player::slideAction(float t_s,float time){

if(t_s<slide_time && sliding && !jmping)
  {
    std::cout << "im sliding " << std::endl;
    int sens = (currentDirection == 1) ? -1 : 1;
     if( blocked(pos) )
    {
      sliding = false; 
    }
    else{
      speed.x= sens * 1000;
    }
  }
}
void Player::input(std::vector<std::optional<sf::Event>> events,float time)
{
    int jmpPressed = false;
    int boomPressed = false;
    int slidePressed = false;
    sliding = false;
    jmping = false;
    anythingPressed = false;
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
         if (keyPressed->scancode == sf::Keyboard::Scancode::RShift )
      {
          slidePressed = true;
          anythingPressed = true;
      }

        }
    }
  // JUMP
  if(jmpPressed && pos.y == sol)
      float time_jmp = jumpAction(time);
    //BOOM
  if(boomPressed)
  {
    bombing = true;
    std::cout << "boom" << std::endl;
    time_boom = time;
  } 
  if(slidePressed)
  {
    sliding = true;
    time_slide = time;
    std::cout << "yogdolehihouuu" << std::endl;
  }
    float t_b  = myClock.getElapsedTime().asSeconds() - time_boom;
  //Bomb action
  bombAction(t_b,time);
    //Slide action
  float t_s  = myClock.getElapsedTime().asSeconds() - time_slide;
  slideAction(t_s,time);
 // Left and Right 
  for(int i = 0; i < 2; i++)
  {

      if(sf::Keyboard::isKeyPressed(keyPerDirection[i]) && !sliding)
      {
          currentDirection = i;
          speed.x = 100 * stepPerDirection[i];
          anythingPressed = true;
      }
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
      if(gravity){
      speed.y += 2*grav.y*dt;    
      }
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
