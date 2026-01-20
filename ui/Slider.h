#pragma once
#include <SFML/Graphics.hpp>

class Slider
{
public:
    Slider(sf::Vector2f position, float width);

    void handleEvent(const sf::Event& event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    float getValue() const;
    void setValue(float v);

private:
    sf::RectangleShape bar;
    sf::CircleShape knob;
    bool dragging = false;
    float value = 0.f;
};
