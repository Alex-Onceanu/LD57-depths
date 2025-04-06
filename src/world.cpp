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
    // std::unique_ptr<Player> j = std::make_unique<Player>();
    // entities.push_back(std::move(j));

    // auto c = std::make_unique<Camera>(nullptr, sf::Vector2f({ RES_X,RES_Y }));
    // entities.push_back(std::move(c));

    std::cout << "Mon buffle, je m'apprête à wfc-er." << std::endl;

    const int TEST_W = 24;
    Wfc generator = Wfc(TEST_W, TEST_W, 1);
    map = generator.collapse();

    std::cout << "Mon buffle, j'ai fini de wfc-er." << std::endl;

    mapTexs = new sf::Texture[map.size()];
    std::cout << "taille de map : " << map.size() << std::endl;
    int i = 0;
    for(auto& t : map)
    {
        mapTexs[i] = sf::Texture("assets/tilemap.png");
        sf::Sprite sprite(mapTexs[i]);
        sprite.setTextureRect(t.getRect());
        
        float x = (i % TEST_W) * 32;
        float y = (i / TEST_W) * 32;
        sprite.setPosition({ 64 + x, 64 + y });
        mapSprites.push_back(sprite);
        // if(x < 0.9)
        // {
        //     std::cout << std::endl;
        // }
        // std::cout << t.getRect().position.x << " ";
        i++;
    }

    std::cout << "\nMon buffle, j'ai fini d'initialiser mapSprites." << std::endl;
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
    for(auto& t : mapSprites)
    {
        window.draw(t);
    }

    for(auto& e : entities)
    {
        e->draw(window);
    }
}
