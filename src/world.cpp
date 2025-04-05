#include <SFML/Graphics.hpp>
#include <memory>

#include "world.hpp"
#include "entity.hpp"
#include "player.hpp"
World::World()
{
  std::unique_ptr<Player> j = std::make_unique<Player>();
  entities.push_back(std::move(j)); 
}

World::~World()
{
    
}

void World::input()
{
    for(auto& e : entities)
    {
        e->input();
    }
}

void World::process(float dt)
{
    for(auto& e : entities)
    {
        e->process(dt);
    }
}

void World::draw(sf::RenderWindow& window)
{
    for(auto& e : entities)
    {
        e->draw(window);
    }
}
