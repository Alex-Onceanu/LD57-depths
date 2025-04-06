#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "tile.hpp"
#include "entity.hpp"

class Block : public Entity {
public:
    Block() {};
    Block(Tile t, int _index, sf::Sprite _sprite);
    ~Block() {};

    void input(std::vector<std::optional<sf::Event>> events);
    void process(float dt);
    void draw(sf::RenderWindow& window);
private:
    Tile body;
    int index; // une instance de Block est un quart de Tile, donc on doit retenir quel quart on est
    const sf::RectangleShape hitbox; //16px*16px par défaut
    sf::Sprite sprite;

}