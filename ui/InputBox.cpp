#include "InputBox.h"
#include "UIColors.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <string>

InputBox::InputBox(const sf::Font& font, sf::Vector2f position)
{
    box.setSize({120.f, 40.f});
    box.setPosition(position);
    box.setFillColor(UIColors::Box);
    box.setOutlineThickness(2.f);
    box.setOutlineColor(UIColors::Border);

    text.setFont(font);
    text.setCharacterSize(18);
    text.setFillColor(UIColors::Text);
    text.setPosition(position + sf::Vector2f(8.f, 8.f));
    text.setString("0");
    
    // Initialize with valid value
    lastValidValue = 0.f;
    hasValidValue = true;
}

bool InputBox::isValidFloat(const sf::String& str) const
{
    // Empty string is not valid
    if (str.isEmpty()) return false;
    
    std::string s = str.toAnsiString();
    
    // Lone '-' or '.' is not valid
    if (s == "-" || s == ".") return false;
    
    // Count decimal points
    int decimalCount = 0;
    int signCount = 0;
    
    for (size_t i = 0; i < s.length(); ++i)
    {
        if (s[i] == '.') {
            decimalCount++;
            if (decimalCount > 1) return false; // Multiple decimals not allowed
        } else if (s[i] == '-') {
            signCount++;
            if (signCount > 1) return false; // Multiple signs not allowed
            if (i != 0) return false; // Sign only allowed at start
        } else if (!std::isdigit(s[i])) {
            return false; // Invalid character
        }
    }
    
    return true;
}

void InputBox::validateAndRevert()
{
    if (!isValidFloat(text.getString()) || text.getString().isEmpty())
    {
        // Revert to last valid value
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(6) << lastValidValue;
        // Remove trailing zeros
        std::string str = ss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        str.erase(str.find_last_not_of('.') + 1, std::string::npos);
        text.setString(str);
        hasValidValue = false;
    }
    else
    {
        // Update last valid value
        try {
            lastValidValue = std::stof(text.getString().toAnsiString());
            hasValidValue = true;
        } catch (...) {
            // Should not happen with isValidFloat check
            hasValidValue = false;
        }
    }
}

void InputBox::handleEvent(const sf::Event& event, sf::Vector2f mousePos)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        active = box.getGlobalBounds().contains(mousePos);
        box.setOutlineColor(active ? UIColors::Active : UIColors::Border);
    }

    if (!active) return;

    if (event.type == sf::Event::TextEntered)
    {
        sf::String s = text.getString();

        if (event.text.unicode == 8 && s.getSize() > 0) // backspace
        {
            s.erase(s.getSize() - 1, 1);
            text.setString(s);
            // Validate after backspace
            validateAndRevert();
        }
        else if ((event.text.unicode >= '0' && event.text.unicode <= '9') ||
                 event.text.unicode == '.' || event.text.unicode == '-')
        {
            s += static_cast<char>(event.text.unicode);
            text.setString(s);
            // Validate after input
            validateAndRevert();
        }
    }
}

float InputBox::getValue() const
{
    // If current text is invalid, return last valid value
    if (!isValidFloat(text.getString()) || text.getString().isEmpty())
    {
        return lastValidValue;
    }
    
    try
    {
        float value = std::stof(text.getString().toAnsiString());
        // Update last valid value on successful parse
        lastValidValue = value;
        return value;
    }
    catch (...)
    {
        return lastValidValue;
    }
}

void InputBox::setValue(float value)
{
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6) << value;
    // Remove trailing zeros
    std::string str = ss.str();
    str.erase(str.find_last_not_of('0') + 1, std::string::npos);
    str.erase(str.find_last_not_of('.') + 1, std::string::npos);
    
    text.setString(str);
    lastValidValue = value;
    hasValidValue = true;
}

void InputBox::draw(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(text);
}
