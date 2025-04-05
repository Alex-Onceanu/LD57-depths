#include "camera.hpp"

Camera::Camera(sf::Vector2f *__following, sf::Vector2f resolution)
    : following(__following)
{
    rect = sf::FloatRect(0.5f * resolution, resolution);

    if(following)
    {
        rect.position = *following - 0.5f * resolution;
    }

    view.setSize(rect.size);
}

void Camera::input(std::vector<std::optional<sf::Event>> events)
{

}

void Camera::process(float dt)
{
    if(following)
        rect.position = *following - 0.5f * rect.size;
}

void Camera::draw(sf::RenderWindow& window)
{
    view.setCenter(rect.getCenter());
}

sf::Vector2f Camera::getPos()
{
    return rect.position;
}

void Camera::setZoom(float coef)
{
    view.zoom(coef);
}
