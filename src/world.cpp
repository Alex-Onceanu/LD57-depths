#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "world.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "camera.hpp"
#include "wfc.hpp"

World::World()
{
    std::unique_ptr<Player> j = std::make_unique<Player>();
    entities.push_back(std::move(j));

    // auto c = std::make_unique<Camera>(nullptr, sf::Vector2f({ RES_X,RES_Y }));
    // entities.push_back(std::move(c));

    std::cout << "Mon buffle, je m'apprête à wfc-er." << std::endl;

    Wfc generator = Wfc(1);
    
    map = generator.collapse(NB_TILES_X, NB_TILES_Y, Wfc::emptyTileset(NB_TILES_X, NB_TILES_Y));

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

    std::cout << "\nMon buffle, j'ai fini d'initialiser mapSprites." << std::endl;
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
