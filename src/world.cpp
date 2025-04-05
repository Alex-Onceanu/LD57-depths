#include "world.hpp"
#include "entity.hpp"
#include "camera.hpp"

World::World()
{
    auto c = std::make_unique<Camera>(nullptr, sf::Vector2f({ RES_X,RES_Y }));
    entities.push_back(std::move(c));

    std::cout << "Mon buffle, je m'apprête à wfc-er." << std::endl;

    Wfc generator = Wfc(15, 2);
    map = generator.wfc(MAP_WIDTH, MAP_HEIGHT, MAP_HEIGHT * MAP_WIDTH);

    std::cout << "Mon buffle, j'ai fini de wfc-er." << std::endl;

    std::cout << "taille de map : " << map.size() << std::endl;
    int i = 0;
    for(auto& t : map)
    {
        const sf::Texture texture("assets/tilemap.png");
        sf::Sprite sprite(texture);
        sprite.setTextureRect(t.getRect());
        
        // std::cout << "print tile : ";
        // generator.printTile(t.sides);
        // std::cout << std::endl;
        
        float x = (i % MAP_WIDTH) * 32;
        float y = (i / MAP_WIDTH) * 32;
        sprite.setPosition({ x,y });
        mapSprites.push_back(sprite);
        i++;
    }

    std::cout << "Mon buffle, j'ai fini d'initialiser mapSprites." << std::endl;
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
    for(auto& t : mapSprites)
    {
        window.draw(t);
    }

    for(auto& e : entities)
    {
        e->draw(window);
    }
}
