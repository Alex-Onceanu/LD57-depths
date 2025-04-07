#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "world.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "fog.hpp"
#include "wfc.hpp"
#include "chunkgenerator.hpp"

World::World()
{
    auto generator = std::make_unique<Wfc>(2);
    
    std::vector<Tile> initial = Wfc::emptyTileset(NB_TILES_X, NB_TILES_Y);
    for(int j = 0; j < NB_TILES_X; j++)
    {
        initial[j] = Tile(0, 0, 0, 0);
    }
    // for(int i = 0; i < NB_TILES_Y; i++)
    // {
    //     for(int j = 0; j < NB_TILES_X; j++)
    //     {
    //         if(j == 0 or j == NB_TILES_X - 1)
    //         initial[j + i * NB_TILES_X] = Tile(0, 0, 0, 0);
    //     }
    // }
    map = generator->collapse(NB_TILES_X, NB_TILES_Y, initial);
    assert(map.size() == NB_TILES_X * NB_TILES_Y);
    mapTexs = new sf::Texture[map.size()];

    int i = 0;
    for(auto& t : map)
    {
        mapTexs[i] = sf::Texture("assets/tilemap.png");
        mapTexs[i].setSmooth(false);
        sf::Sprite sprite(mapTexs[i]);
        sprite.setTextureRect(t.getRect());
        sprite.setOrigin({16.0,16.0});
        
        float x = (i % NB_TILES_X) * 32;
        float y = (i / NB_TILES_X) * 32;

        sprite.setPosition({ 16 + x + RES_X / 2 - NB_TILES_X * 16, y + RES_Y / 2 });
        mapSprites.push_back(sprite);
        i++;
    }
    auto f = std::make_unique<Fog>();
    mapOffset = sf::Vector2f({16 + RES_X / 2 - NB_TILES_X * 16, RES_Y / 2});
    std::unique_ptr<Player> j = std::make_unique<Player>(&map, NB_TILES_X, &mapOffset, f->getHeight(), &mapSprites);
    auto c = std::make_unique<Camera>(j->getPosPtr());
    auto g = std::make_unique<ChunkGenerator>(j->getPosPtr(), &mapOffset, &map, &mapSprites, std::move(generator));
    j->setSpriteCyclePtr(g->getSpriteCyclePtr());
    
    entities.push_back(std::move(j));
    entities.push_back(std::move(c));
    entities.push_back(std::move(f));
    entities.push_back(std::move(g));
}

World::~World()
{
    
}


void World::input(std::vector<std::optional<sf::Event>> events, float time)
{
    for(auto& e : entities)
    {
        e->input(events,time);
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
    for(auto& t : mapSprites)
    {
        window.draw(t);
    }

    for(auto& e : entities)
    {
        e->draw(window);
    }
}
