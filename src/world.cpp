#include "world.hpp"
#include "entity.hpp"
#include "camera.hpp"

World::World()
{
    auto c = std::make_unique<Camera>(nullptr, sf::Vector2f({ RES_X,RES_Y }));
    entities.push_back(std::move(c));
}

World::~World()
{
    
}

void World::input(std::vector<std::optional<sf::Event>> events)
{
    for(auto& e : entities)
    {
        e->input(events);
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
