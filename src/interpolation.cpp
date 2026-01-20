/**
 * @file interpolation.cpp
 * @author Sinan Demir
 * @brief 
 * @version 0.1
 * @date 2025-12-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "interpolation.h"

// Linear interpolation
float lerp(float a, float b, float t)
{
    return a + t * (b - a);
}

// Vector interpolation (for visualization)
sf::Vector2f lerpVec(const sf::Vector2f& a, const sf::Vector2f& b, float t)
{
    return a + t * (b - a);
}