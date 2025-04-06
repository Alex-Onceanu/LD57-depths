#include "camera.hpp"

Camera::Camera(sf::Vector2f *__following)
    : following(__following)
{
    view.setCenter({512.0, 288.0});
    view.setSize({1024.0,576.0});
    view.zoom(0.5f);
}

void Camera::process(float dt)
{
    if(following){}
        view.setCenter({512, following->y});
}
void Camera::draw(sf::RenderWindow& window)
{
    window.setView(view);
}

void Camera::setZoom(float coef)
{
    view.zoom(coef);
}
