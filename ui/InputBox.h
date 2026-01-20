#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class InputBox
{
public:
    InputBox(const sf::Font& font, sf::Vector2f position);

    void handleEvent(const sf::Event& event, sf::Vector2f mousePos);
    void draw(sf::RenderWindow& window);

    float getValue() const;
    void setValue(float value);

private:
    // Validation helpers
    bool isValidFloat(const sf::String& str) const;
    void validateAndRevert();

    // UI components
    sf::RectangleShape box;
    sf::Text text;
    bool active = false;
    
    // Validation state
    mutable float lastValidValue = 0.f;
    bool hasValidValue = true;
};
