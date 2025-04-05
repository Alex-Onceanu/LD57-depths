#include "world.hpp"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1366u, 768u}), "Ludum Dare 57");
    window.setFramerateLimit(60);
    window.setPosition({ 77,156 });

    World* w = new World();

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

        w->input();
        w->process(dt);
        w->draw(window);

        window.clear({ 80,80,80 });
        window.display();
    }

    delete w;
}