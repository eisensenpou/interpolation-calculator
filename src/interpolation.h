/**
 * @file interpolation.h
 * @author Sinan Demir
 * @brief 
 * @version 0.1
 * @date 2025-12-20
 * 
 * @copyright Copyright (c) 2025
 */


#pragma once
#include <SFML/System.hpp>

float lerp(float a, float b, float t);
sf::Vector2f lerpVec(const sf::Vector2f& a, const sf::Vector2f& b, float t);