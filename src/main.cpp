#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#include "interpolation.h"
#include "InputBox.h"
#include "Slider.h"
#include "Button.h"
#include "UIColors.h"

// UI Layout Constants
namespace UILayout {
    // Window dimensions
    constexpr float WINDOW_WIDTH = 800.f;
    constexpr float WINDOW_HEIGHT = 500.f;
    
    // Input box positions
    const sf::Vector2f INPUT_A_POS = {50.f, 50.f};
    const sf::Vector2f INPUT_B_POS = {50.f, 120.f};
    const sf::Vector2f SLIDER_POS = {250.f, 80.f};
    
    // Label positions
    const sf::Vector2f LABEL1_POS = {20.f, 58.f};
    const sf::Vector2f LABEL2_POS = {20.f, 128.f};
    const sf::Vector2f LABEL3_POS = {220.f, 58.f};
    
    // Unit label positions (relative to inputs)
    const sf::Vector2f UNIT1_POS = {170.f, 62.f};
    const sf::Vector2f UNIT2_POS = {170.f, 132.f};
    const sf::Vector2f UNIT3_POS = {290.f, 62.f};
    
    // Button positions and sizes
    const sf::Vector2f RESET_BUTTON_POS = {680.f, 50.f};
    const sf::Vector2f RESET_BUTTON_SIZE = {80.f, 30.f};
    
    // Text positions
    const sf::Vector2f MODE_TEXT_POS = {20.f, 15.f};
    const sf::Vector2f FEEDBACK_TEXT_POS = {20.f, 35.f};
    const sf::Vector2f RESULT_TEXT_POS = {50.f, 200.f};
    const sf::Vector2f WARNING_TEXT_POS = {50.f, 250.f};
    
    // Visualization positions
    const sf::Vector2f VISUALIZATION_A = {250.f, 350.f};
    const sf::Vector2f VISUALIZATION_B = {650.f, 350.f};
    
    // Help overlay
    const sf::Vector2f HELP_OVERLAY_POS = {100.f, 75.f};
    const sf::Vector2f HELP_OVERLAY_SIZE = {600.f, 350.f};
    const sf::Vector2f HELP_TITLE_POS = {120.f, 85.f};
    const sf::Vector2f HELP_CONTENT_POS = {120.f, 115.f};
    
    // Slider dimensions
    constexpr float SLIDER_WIDTH = 400.f;
    
    // Default values
    constexpr float DEFAULT_A = 0.f;
    constexpr float DEFAULT_B = 100.f;
    constexpr float DEFAULT_T = 0.f;
    
    // Range constraints
    constexpr float MIN_TEMP = -50.f;
    constexpr float MAX_TEMP = 200.f;
    constexpr float MIN_SLIDER = 0.f;
    constexpr float MAX_SLIDER = 1.f;
    
    // Text sizes
    constexpr unsigned int LABEL_TEXT_SIZE = 18;
    constexpr unsigned int MODE_TEXT_SIZE = 16;
    constexpr unsigned int FEEDBACK_TEXT_SIZE = 14;
    constexpr unsigned int RESULT_TEXT_SIZE = 20;
    constexpr unsigned int WARNING_TEXT_SIZE = 12;
    constexpr unsigned int HELP_TITLE_SIZE = 18;
    constexpr unsigned int HELP_CONTENT_SIZE = 14;
}

// Mode system
enum class Mode {
    LinearInterpolation,
    TemperatureGradient,
    CustomScale
};

struct ModeInfo {
    Mode mode;
    std::string name;
    std::string label1;
    std::string label2;
    std::string label3;
    std::string unit1;
    std::string unit2;
    std::string unit3;
    std::string unitResult;
};

// Mode dispatcher - returns scalar result based on current mode
// Centralizes all computation logic and maintains separation from UI
float computeResult(Mode mode, float val1, float val2, float t) {
    switch (mode) {
        case Mode::LinearInterpolation:
            // Standard linear interpolation: result = val1 + t * (val2 - val1)
            return lerp(val1, val2, t);
        case Mode::TemperatureGradient:
            // Temperature interpolation at position t between temperatures val1 (T0) and val2 (T1)
            // Physics interpretation: temperature varies linearly with position
            return lerp(val1, val2, t);
        case Mode::CustomScale:
            // Apply normalized scale factor t [0,1] to range [val1, val2]
            // Display shows both absolute result and percentage of scale
            return lerp(val1, val2, t);
        default:
            return lerp(val1, val2, t);
    }
}

// Get mode information
ModeInfo getModeInfo(Mode mode) {
    switch (mode) {
        case Mode::LinearInterpolation:
            return {mode, "Linear Interpolation", "A:", "B:", "t:", "", "", "", ""};
        case Mode::TemperatureGradient:
            return {mode, "Temperature Gradient", "T0:", "T1:", "x:", "°C", "°C", "", "°C"};
        case Mode::CustomScale:
            return {mode, "Custom Scale", "A:", "B:", "s:", "", "", "%", "%"};
        default:
            return {mode, "Linear Interpolation", "A:", "B:", "t:", "", "", "", ""};
    }
}

// Cycle through modes
Mode cycleMode(Mode current) {
    switch (current) {
        case Mode::LinearInterpolation: return Mode::TemperatureGradient;
        case Mode::TemperatureGradient: return Mode::CustomScale;
        case Mode::CustomScale: return Mode::LinearInterpolation;
        default: return Mode::LinearInterpolation;
    }
}

// Get mode by number key
Mode getModeByKey(int key) {
    switch (key) {
        case 1: return Mode::LinearInterpolation;
        case 2: return Mode::TemperatureGradient;
        case 3: return Mode::CustomScale;
        default: return Mode::LinearInterpolation;
    }
}

// Temperature to color mapping (blue → red gradient)
// Visual physics: maps temperature to intuitive color representation
sf::Color temperatureToColor(float temperature, float minTemp = 0.f, float maxTemp = 100.f) {
    // Normalize temperature to [0, 1] for color mapping
    float normalized = (temperature - minTemp) / (maxTemp - minTemp);
    normalized = std::clamp(normalized, 0.f, 1.f);
    
    // Blue (cold) → Cyan → Green → Yellow → Red (hot) gradient
    // Each segment represents 25% of the temperature range
    if (normalized < 0.25f) {
        // Blue to Cyan: increasing green component
        float t = normalized / 0.25f;
        return sf::Color(0, static_cast<int>(255 * t), 255);
    } else if (normalized < 0.5f) {
        // Cyan to Green: decreasing blue component
        float t = (normalized - 0.25f) / 0.25f;
        return sf::Color(0, 255, static_cast<int>(255 * (1.f - t)));
    } else if (normalized < 0.75f) {
        // Green to Yellow: increasing red component
        float t = (normalized - 0.5f) / 0.25f;
        return sf::Color(static_cast<int>(255 * t), 255, 0);
    } else {
        // Yellow to Red: decreasing green component
        float t = (normalized - 0.75f) / 0.25f;
        return sf::Color(255, static_cast<int>(255 * (1.f - t)), 0);
    }
}



int main()
{
    sf::RenderWindow window(
        sf::VideoMode(UILayout::WINDOW_WIDTH, UILayout::WINDOW_HEIGHT),
        "Interpolation Calculator"
    );
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    // Mode system initialization
    Mode currentMode = Mode::LinearInterpolation;
    ModeInfo modeInfo = getModeInfo(currentMode);

    // UI elements
    InputBox inputA(font, UILayout::INPUT_A_POS);
    InputBox inputB(font, UILayout::INPUT_B_POS);
    Slider slider(UILayout::SLIDER_POS, UILayout::SLIDER_WIDTH);

    // Reset button
    Button resetButton;
    resetButton.setFont(font);
    resetButton.setText("Reset");
    resetButton.setSize(UILayout::RESET_BUTTON_SIZE);
    resetButton.setPosition(UILayout::RESET_BUTTON_POS);

    inputA.setValue(UILayout::DEFAULT_A);
    inputB.setValue(UILayout::DEFAULT_B);
    slider.setValue(UILayout::DEFAULT_T);

    // Dynamic labels (will be updated based on mode)
    sf::Text label1(modeInfo.label1, font, UILayout::LABEL_TEXT_SIZE);
    sf::Text label2(modeInfo.label2, font, UILayout::LABEL_TEXT_SIZE);
    sf::Text label3(modeInfo.label3, font, UILayout::LABEL_TEXT_SIZE);

    label1.setPosition(UILayout::LABEL1_POS);
    label2.setPosition(UILayout::LABEL2_POS);
    label3.setPosition(UILayout::LABEL3_POS);
    
    // Unit labels (will be updated based on mode)
    sf::Text unit1("", font, UILayout::LABEL_TEXT_SIZE - 4);
    sf::Text unit2("", font, UILayout::LABEL_TEXT_SIZE - 4);
    sf::Text unit3("", font, UILayout::LABEL_TEXT_SIZE - 4);
    
    unit1.setFillColor(sf::Color(200, 200, 200)); // Lighter gray for units
    unit2.setFillColor(sf::Color(200, 200, 200));
    unit3.setFillColor(sf::Color(200, 200, 200));
    
    unit1.setPosition(UILayout::UNIT1_POS);
    unit2.setPosition(UILayout::UNIT2_POS);
    unit3.setPosition(UILayout::UNIT3_POS);

    // Mode display text
    sf::Text modeText("", font, UILayout::MODE_TEXT_SIZE);
    modeText.setPosition(UILayout::MODE_TEXT_POS);
    modeText.setFillColor(sf::Color::Cyan);

    // Mode switch feedback text
    sf::Text feedbackText("", font, UILayout::FEEDBACK_TEXT_SIZE);
    feedbackText.setPosition(UILayout::FEEDBACK_TEXT_POS);
    feedbackText.setFillColor(sf::Color::Yellow);
    sf::Clock feedbackClock;
    
    // Help overlay
    bool showHelp = false;
    sf::RectangleShape helpOverlay;
    helpOverlay.setSize(UILayout::HELP_OVERLAY_SIZE);
    helpOverlay.setPosition(UILayout::HELP_OVERLAY_POS);
    helpOverlay.setFillColor(sf::Color(20, 20, 20, 230)); // Semi-transparent
    helpOverlay.setOutlineThickness(2.f);
    helpOverlay.setOutlineColor(sf::Color::White);
    
    // Help content
    sf::Text helpTitle("INTERPOLATION CALCULATOR - HELP", font, UILayout::HELP_TITLE_SIZE);
    helpTitle.setPosition(UILayout::HELP_TITLE_POS);
    helpTitle.setFillColor(sf::Color::Cyan);
    
    sf::Text helpContent("", font, UILayout::HELP_CONTENT_SIZE);
    helpContent.setPosition(UILayout::HELP_CONTENT_POS);
    helpContent.setFillColor(sf::Color::White);
    
    // Warning text for clamping feedback
    sf::Text warningText("", font, UILayout::WARNING_TEXT_SIZE);
    warningText.setPosition(UILayout::WARNING_TEXT_POS);
    warningText.setFillColor(sf::Color::Yellow);

    // Result text
    sf::Text resultText("", font, UILayout::RESULT_TEXT_SIZE);
    resultText.setPosition(UILayout::RESULT_TEXT_POS);

    // Visualization points
    sf::Vector2f A(UILayout::VISUALIZATION_A);
    sf::Vector2f B(UILayout::VISUALIZATION_B);

    sf::CircleShape point(6.f);
    point.setOrigin(6.f, 6.f);
    point.setFillColor(sf::Color::Red);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Mode switching
            if (event.type == sf::Event::KeyPressed)
            {
                Mode oldMode = currentMode;
                
                if (event.key.code == sf::Keyboard::Tab)
                {
                    currentMode = cycleMode(currentMode);
                    feedbackText.setString("Mode: " + getModeInfo(currentMode).name);
                    feedbackClock.restart();
                }
                else if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num3)
                {
                    int keyNum = event.key.code - sf::Keyboard::Num1 + 1;
                    currentMode = getModeByKey(keyNum);
                    feedbackText.setString("Mode: " + getModeInfo(currentMode).name);
                    feedbackClock.restart();
                }
                else if (event.key.code == sf::Keyboard::H)
                {
                    showHelp = !showHelp;
                    feedbackText.setString(showHelp ? "Help overlay enabled" : "Help overlay disabled");
                    feedbackClock.restart();
                }
            }

            sf::Vector2f mousePos =
                static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

            inputA.handleEvent(event, mousePos);
            inputB.handleEvent(event, mousePos);
            slider.handleEvent(event, mousePos);
            resetButton.handleEvent(event, mousePos);
            
            // Reset button click handling
            if (resetButton.wasClicked())
            {
                // Reset to sensible defaults while keeping current mode
                if (currentMode == Mode::TemperatureGradient)
                {
                    inputA.setValue(0.f);  // T0 = 0°C
                    inputB.setValue(100.f); // T1 = 100°C
                }
                else if (currentMode == Mode::CustomScale)
                {
                    inputA.setValue(UILayout::DEFAULT_A);  // A = 0
                    inputB.setValue(UILayout::MAX_SLIDER);  // B = 1 (normalized range)
                }
                else // LinearInterpolation
                {
                    inputA.setValue(UILayout::DEFAULT_A);  // A = 0
                    inputB.setValue(UILayout::DEFAULT_B); // B = 100
                }
                slider.setValue(UILayout::DEFAULT_T); // Reset slider/t position
                
                feedbackText.setString("Values reset to defaults");
                feedbackClock.restart();
            }
        }

        // Update mode info and labels if mode changed
        modeInfo = getModeInfo(currentMode);
        label1.setString(modeInfo.label1);
        label2.setString(modeInfo.label2);
        label3.setString(modeInfo.label3);
        
        // Update unit labels
        unit1.setString(modeInfo.unit1);
        unit2.setString(modeInfo.unit2);
        unit3.setString(modeInfo.unit3);
        
        modeText.setString("Mode: " + modeInfo.name + " (Tab/1-3 to switch)");

        // Hide feedback text after 2 seconds
        if (feedbackClock.getElapsedTime().asSeconds() > 2.f) {
            feedbackText.setString("");
        }

        // Input semantics: read user-provided values
        // val1/val2: range endpoints (A/B, T0/T1, or scale limits)
        // t: interpolation parameter or scale factor
        float val1 = inputA.getValue();
        float val2 = inputB.getValue();
        float t = slider.getValue();
        
        // Input validation and clamping feedback
        std::string warningMessage;
        bool needsClamping = false;
        
        // Check slider range (should be [MIN_SLIDER, MAX_SLIDER])
        if (t < UILayout::MIN_SLIDER || t > UILayout::MAX_SLIDER) {
            needsClamping = true;
            t = std::clamp(t, UILayout::MIN_SLIDER, UILayout::MAX_SLIDER);
            warningMessage += "Slider clamped to [0, 1] range. ";
        }
        
        // Mode-specific range checking
        if (currentMode == Mode::TemperatureGradient) {
            // Temperature range: assume reasonable limits MIN_TEMP to MAX_TEMP
            if (val1 < UILayout::MIN_TEMP || val1 > UILayout::MAX_TEMP) {
                needsClamping = true;
                val1 = std::clamp(val1, UILayout::MIN_TEMP, UILayout::MAX_TEMP);
                inputA.setValue(val1); // Update input box with clamped value
                warningMessage += "T0 clamped to [-50, 200]°C. ";
            }
            if (val2 < UILayout::MIN_TEMP || val2 > UILayout::MAX_TEMP) {
                needsClamping = true;
                val2 = std::clamp(val2, UILayout::MIN_TEMP, UILayout::MAX_TEMP);
                inputB.setValue(val2); // Update input box with clamped value
                warningMessage += "T1 clamped to [-50, 200]°C. ";
            }
            // Ensure T1 >= T0
            if (val2 < val1) {
                needsClamping = true;
                val2 = val1;
                inputB.setValue(val2);
                warningMessage += "T1 adjusted to be >= T0. ";
            }
        } else if (currentMode == Mode::CustomScale) {
            // For CustomScale, ensure val2 >= val1
            if (val2 < val1) {
                needsClamping = true;
                val2 = val1;
                inputB.setValue(val2);
                warningMessage += "B adjusted to be >= A. ";
            }
        }
        
        // Update warning text
        if (needsClamping) {
            warningText.setString("⚠ " + warningMessage);
        } else {
            warningText.setString("");
        }
        
        float result = computeResult(currentMode, val1, val2, t);

        // Update result text with dynamic labels and units
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << modeInfo.label1.substr(0, 1) << " = " << val1 << modeInfo.unit1 << "\n";
        ss << modeInfo.label2.substr(0, 1) << " = " << val2 << modeInfo.unit2 << "\n";
        ss << modeInfo.label3.substr(0, 1) << " = " << t << modeInfo.unit3 << "\n";
        
        // For CustomScale, show both actual result and percentage
        if (currentMode == Mode::CustomScale) {
            float percentage = (result - val1) / (val2 - val1) * 100.f;
            ss << "result = " << result << " (" << percentage << "%)";
        } else {
            ss << "result = " << result << modeInfo.unitResult;
        }

        resultText.setString(ss.str());

        // Mode-specific visualization: colors encode physical meaning
        sf::Color pointColor = sf::Color::Red;
        sf::Color lineStartColor = sf::Color::White;
        sf::Color lineEndColor = sf::Color::White;
        
        switch (currentMode) {
            case Mode::TemperatureGradient:
                // Physics-based coloring: temperature maps to intuitive colors
                pointColor = temperatureToColor(result, val1, val2);
                lineStartColor = temperatureToColor(val1, val1, val2); // T0 color
                lineEndColor = temperatureToColor(val2, val1, val2);   // T1 color
                break;
            case Mode::CustomScale:
                // Visual distinction: purple theme for scaling operations
                pointColor = sf::Color(128, 0, 255); // Purple for interpolated result
                lineStartColor = sf::Color(100, 100, 150); // Blue-tinted start
                lineEndColor = sf::Color(150, 100, 100);   // Red-tinted end
                break;
            case Mode::LinearInterpolation:
            default:
                // Neutral colors for mathematical interpolation
                pointColor = sf::Color::Red;
                lineStartColor = sf::Color::White;
                lineEndColor = sf::Color::White;
                break;
        }

        // Interpolated point
        sf::Vector2f P = lerpVec(A, B, t);
        point.setPosition(P);
        point.setFillColor(pointColor);

        // Line AB with mode-specific colors (gradient for temperature mode)
        sf::Vertex line[] =
        {
            sf::Vertex(A, lineStartColor),
            sf::Vertex(B, lineEndColor)
        };

        // Render
        window.clear(UIColors::Background);

        window.draw(modeText);
        window.draw(feedbackText);
        window.draw(label1);
        window.draw(label2);
        window.draw(label3);
        window.draw(unit1);
        window.draw(unit2);
        window.draw(unit3);

        inputA.draw(window);
        inputB.draw(window);
        slider.draw(window);
        resetButton.draw(window);

        window.draw(resultText);
        window.draw(warningText);
        window.draw(line, 2, sf::Lines);
        window.draw(point);
        
        // Help overlay (rendered last to appear on top)
        if (showHelp)
        {
            window.draw(helpOverlay);
            window.draw(helpTitle);
            window.draw(helpContent);
        }

        window.display();
    }

    return 0;
}
