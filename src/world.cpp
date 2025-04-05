#include <SFML/Graphics.hpp>

#include "world.hpp"
#include "entity.hpp"

World::World()
{
    
}

World::~World()
{
    
}

void World::input()
{
    for(auto& e : entities)
    {
        e.input();
    }
}

void World::process(float dt)
{
    for(auto& e : entities)
    {
        e.process(dt);
    }
}

void World::draw(sf::RenderWindow& window)
{
    for(auto& e : entities)
    {
        e.draw(window);
    }
}
