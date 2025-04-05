#include "camera.hpp"

Camera::Camera(sf::Vector2f* __following, sf::Vector2f resolution)
    : following(__following)
{
    sf::Vector2f pos{ 0.0,0.0 };

    if(following)
    {
        pos = *following - 0.5f * resolution;
    }
    
    rect = sf::FloatRect(pos, resolution);
}

void Camera::process(float dt)
{
    rect.position = *following - 0.5f * rect.size;
}

sf::Vector2f Camera::getPos()
{
    return rect.getCenter();
}

void Camera::setZoom(float coef)
{
    if(coef > 0.0)
    {
        zoom = coef;
        rect.size /= zoom;
    }
}