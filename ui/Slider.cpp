#include "Slider.h"
#include "UIColors.h"
#include <algorithm>

Slider::Slider(sf::Vector2f position, float width)
{
    bar.setSize({width, 6.f});
    bar.setPosition(position);
    bar.setFillColor(UIColors::Border);

    knob.setRadius(8.f);
    knob.setOrigin(8.f, 8.f);
    knob.setFillColor(UIColors::SliderKnob);
    knob.setPosition(position.x, position.y + 3.f);
}

void Slider::handleEvent(const sf::Event& event, sf::Vector2f mousePos)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (knob.getGlobalBounds().contains(mousePos))
            dragging = true;
    }

    if (event.type == sf::Event::MouseButtonReleased)
        dragging = false;

    if (event.type == sf::Event::MouseMoved && dragging)
    {
        float left = bar.getPosition().x;
        float right = left + bar.getSize().x;

        float x = std::clamp(mousePos.x, left, right);
        knob.setPosition(x, knob.getPosition().y);
        value = (x - left) / bar.getSize().x;
    }
}

float Slider::getValue() const
{
    return value;
}

void Slider::setValue(float v)
{
    value = std::clamp(v, 0.f, 1.f);

    float left = bar.getPosition().x;
    float right = left + bar.getSize().x;
    float x = left + value * (right - left);

    knob.setPosition(x, knob.getPosition().y);
}

void Slider::draw(sf::RenderWindow& window)
{
    window.draw(bar);
    window.draw(knob);
}
