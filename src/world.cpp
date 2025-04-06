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
        sf::Texture texture = sf::Texture("../../assets/tilemap.png");
        arrTextures[i] = sf::Texture("../../assets/tilemap.png");
        //sf::Sprite sprite(std::move(texture));
        //sf::Sprite* sprite = new sf::Sprite(texture);
        sf::Sprite* sprite = new sf::Sprite(arrTextures[i]);
        if (t.getId() == 0){
            sprite->setTextureRect(sf::IntRect({0,0}, {0,0}));
        } else {
            sprite->setTextureRect(t.getRect());
        }
        // std::cout << "print tile : ";
        // generator.printTile(t.sides);
        // std::cout << std::endl;
        
        float x = (i % MAP_WIDTH) * 32;
        float y = (i / MAP_WIDTH) * 32;
        sprite->setPosition({ x,y });
        mapSprites.push_back(*sprite);
        //mapTextures.push_back(texture);
        i++;
    }
    std::cout<<"on a fait "<<i<<" tours"<<std::endl;
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
    //std::cout<<"on est chokbar dans le draw"<<std::endl;
    for(auto& t : mapSprites)
    {
        //std::cout<<"tour"<<std::endl;
        window.draw(t);
    }
    //std::cout<<"on est heureux dans le 69"<<std::endl;
    for(auto& e : entities)
    {
        e->draw(window);
    }
}
