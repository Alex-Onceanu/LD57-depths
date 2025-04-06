#include <ctime>
#include <memory>
#include <iostream>
#include "world.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({300u, 150u}), "Ludum Dare 57");
    window.setFramerateLimit(60);
    window.setPosition({ 77,156 });
 
    std::unique_ptr<World> w = std::make_unique<World>();

    sf::Clock clock;
    std::vector<std::optional<sf::Event>> events;
    float timeSinceStart = 0.; 
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

        window.clear({ 0,0,0 });
        w->input(events);
        w->process(dt);
        w->draw(window);

        window.display();
    }
}
