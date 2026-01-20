#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <functional>

class Button
{
public:
    Button();
    
    // Position and size
    void setPosition(sf::Vector2f position);
    void setSize(sf::Vector2f size);
    
    // Text and appearance
    void setText(const std::string& text);
    void setFont(const sf::Font& font);
    void setCharacterSize(unsigned int size);
    
    // Colors for different states
    struct Colors {
        sf::Color normal;
        sf::Color hovered;
        sf::Color pressed;
        sf::Color active;
        sf::Color text;
    };
    void setColors(const Colors& colors);
    
    // State management
    void setActive(bool active);
    bool isActive() const;
    
    // Event handling
    void handleEvent(const sf::Event& event, sf::Vector2f mousePos);
    
    // Rendering
    void draw(sf::RenderTarget& target);
    
    // State queries
    bool isHovered() const;
    bool isPressed() const;
    bool wasClicked() const;

private:
    void updateVisualState();
    
    // Visual elements
    sf::RectangleShape background;
    sf::Text label;
    
    // State
    bool hovered = false;
    bool pressed = false;
    bool active = false;
    bool clicked = false;
    
    // Appearance
    Colors colors;
    
    // Default dimensions
    sf::Vector2f position;
    sf::Vector2f size;
};