#include "Button.h"
#include "UIColors.h"

Button::Button()
{
    // Set default colors
    colors = {
        UIColors::Box,        // normal
        sf::Color(60, 60, 60), // hovered (brighter than Box)
        sf::Color(80, 80, 80), // pressed (brighter still)
        UIColors::Active,      // active
        UIColors::Text         // text
    };
    
    // Initialize visual elements
    background.setSize({120.f, 40.f});
    background.setFillColor(colors.normal);
    background.setOutlineThickness(2.f);
    background.setOutlineColor(UIColors::Border);
    
    label.setCharacterSize(18);
    label.setFillColor(colors.text);
}

void Button::setPosition(sf::Vector2f pos)
{
    position = pos;
    background.setPosition(pos);
    
    // Center text within button
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setPosition(
        pos.x + (size.x - textBounds.width) / 2.f,
        pos.y + (size.y - textBounds.height) / 2.f - 5.f // Adjust for text baseline
    );
}

void Button::setSize(sf::Vector2f newSize)
{
    size = newSize;
    background.setSize(newSize);
    
    // Re-center text
    setPosition(position);
}

void Button::setText(const std::string& text)
{
    label.setString(text);
    setPosition(position); // Re-center text
}

void Button::setFont(const sf::Font& font)
{
    label.setFont(font);
    setPosition(position); // Re-center text
}

void Button::setCharacterSize(unsigned int size)
{
    label.setCharacterSize(size);
    setPosition(position); // Re-center text
}

void Button::setColors(const Colors& newColors)
{
    colors = newColors;
    label.setFillColor(colors.text);
    updateVisualState();
}

void Button::setActive(bool isActive)
{
    active = isActive;
    updateVisualState();
}

bool Button::isActive() const
{
    return active;
}

void Button::handleEvent(const sf::Event& event, sf::Vector2f mousePos)
{
    // Reset clicked state each frame
    clicked = false;
    
    // Check if mouse is over button
    bool wasHovered = hovered;
    hovered = background.getGlobalBounds().contains(mousePos);
    
    // Handle mouse press
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left && hovered)
        {
            pressed = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left && pressed)
        {
            pressed = false;
            clicked = true; // Button was clicked
        }
    }
    else if (event.type == sf::Event::MouseLeft)
    {
        // Mouse left window, reset states
        pressed = false;
        hovered = false;
    }
    
    updateVisualState();
}

void Button::draw(sf::RenderTarget& target)
{
    target.draw(background);
    target.draw(label);
}

bool Button::isHovered() const
{
    return hovered;
}

bool Button::isPressed() const
{
    return pressed;
}

bool Button::wasClicked() const
{
    return clicked;
}

void Button::updateVisualState()
{
    // Determine which color to use based on state
    if (active)
    {
        background.setFillColor(colors.active);
        background.setOutlineColor(colors.active);
    }
    else if (pressed)
    {
        background.setFillColor(colors.pressed);
    }
    else if (hovered)
    {
        background.setFillColor(colors.hovered);
    }
    else
    {
        background.setFillColor(colors.normal);
        background.setOutlineColor(UIColors::Border);
    }
}