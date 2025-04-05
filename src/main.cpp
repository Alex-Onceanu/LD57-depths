#include <SFML/Graphics.hpp>

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1366u, 768u}), "Ludum Dare 57");
    window.setFramerateLimit(60);
    window.setPosition({ 277,156 });

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear({ 80,80,80 });
        window.display();
    }
}