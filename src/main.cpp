#include <SFML/Graphics.hpp>
#include <ctime>
#include <memory>
#include <iostream>
#include "world.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({RES_X,RES_Y}), "Ludum Dare 57", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    window.setPosition({ 0,0 });
    sf::View view1;
    view1.setCenter({RES_X/2.0, RES_Y/2.0});
    view1.setSize({RES_X,RES_Y});
    view1.zoom(0.5f);
    window.setView(view1);

 
    std::unique_ptr<World> w = std::make_unique<World>();

    sf::Clock clock;
    std::vector<std::optional<sf::Event>> events;
    float timeSinceStart = 0.;
    srand(time(NULL));

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        timeSinceStart += dt; 
        events.clear();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else
            {
                events.push_back(event);
            }
        }
        window.clear({ 80,80,80 });

        w->input(events,timeSinceStart);
        w->process(dt);
        w->draw(window);

        window.display();
    }
}
