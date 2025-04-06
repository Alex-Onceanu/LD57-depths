#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "world.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "fog.hpp"
#include "wfc.hpp"

World::World()
{
    std::cout << "Mon buffle, je m'apprête à wfc-er." << std::endl;

    Wfc generator = Wfc(1);
    
    std::vector<Tile> initial = Wfc::emptyTileset(NB_TILES_X, NB_TILES_Y);
    for(int j = 0; j < NB_TILES_X; j++)
    {
        initial[j] = Tile(0, 0, 0, 0);
    }
    for (int i = 1; i< NB_TILES_Y; i++){
        initial[i*NB_TILES_X] = Tile(0,0,3,3);
        initial[(i+1)*NB_TILES_X-1] = Tile(3,3,0,0);
    }
    // for(int i = 0; i < NB_TILES_Y; i++)
    // {
    //     for(int j = 0; j < NB_TILES_X; j++)
    //     {
    //         if(j == 0 or j == NB_TILES_X - 1)
    //         initial[j + i * NB_TILES_X] = Tile(0, 0, 0, 0);
    //     }
    // }
    map = generator.collapse(NB_TILES_X, NB_TILES_Y, initial);

    std::cout << "Mon buffle, j'ai fini de wfc-er." << std::endl;

    mapTexs = new sf::Texture[map.size()];
    std::cout << "taille de map : " << map.size() << std::endl;
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
    std::unique_ptr<Player> j = std::make_unique<Player>(&map, NB_TILES_X, sf::Vector2f({16 + RES_X / 2 - NB_TILES_X * 16, RES_Y / 2}), f->getHeight());
    auto c = std::make_unique<Camera>(j->getPosPtr());
    entities.push_back(std::move(j));
    entities.push_back(std::move(c));
    entities.push_back(std::move(f));

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
