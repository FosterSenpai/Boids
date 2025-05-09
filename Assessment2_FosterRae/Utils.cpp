// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Blenheim
// New Zealand
// (c) 2025 Media Design School
// File Name : Utils.cpp
// Description : Implementation file for general utility functions, mainly vector operations.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/

// **=== Includes ===**
#include "Utils.h"

namespace Utils
{
    // **=== Public Methods ===**

    sf::Vector2f shortestWrappedVector(const sf::Vector2f& from, const sf::Vector2f& to, const sf::Vector2f& worldSize) {
        float dx = to.x - from.x;
        float dy = to.y - from.y;

        // Check horizontal wrapping
        if (std::abs(dx) > worldSize.x / 2.0f) {
            if (dx > 0.0f) { // Target is far right, wrap left
                dx -= worldSize.x;
            }
            else { // Target is far left, wrap right
                dx += worldSize.x;
            }
        }

        // Check vertical wrapping
        if (std::abs(dy) > worldSize.y / 2.0f) {
            if (dy > 0.0f) { // Target is far down, wrap up
                dy -= worldSize.y;
            }
            else { // Target is far up, wrap down
                dy += worldSize.y;
            }
        }

        return sf::Vector2f(dx, dy);
    }

    float magnitude(const sf::Vector2f& vector) {
        return std::sqrt(vector.x * vector.x + vector.y * vector.y); // Magnitude = sqrt(x^2 + y^2)
    }

    sf::Vector2f truncate(const sf::Vector2f& vector, float max) {
        float length = magnitude(vector);

        if (length > max) { // If the vector's length is greater than max
            return normalised(vector) * max; // Normalize the vector and scale it to max length
        }
        return vector;
    }

    sf::Vector2f normalised(const sf::Vector2f& vector) {
        float length = magnitude(vector);
        if (length > 0.0f)
        {
            return vector / length; // Divide vector by its length to get a unit vector
        }
        return { 0.0f, 0.0f }; // Return zero vector if the original vector had zero length
    }
}