#include <memory>

#include "world.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1366u, 768u}), "Ludum Dare 57");
    window.setFramerateLimit(60);
    window.setPosition({ 77,156 });
    std::unique_ptr<World> w = std::make_unique<World>();

    sf::Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear({ 80,80,80 });
        w->input();
        w->process(dt);
        w->draw(window);
        window.display();
    }
}