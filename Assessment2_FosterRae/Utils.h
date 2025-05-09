// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Blenheim
// New Zealand
// (c) 2025 Media Design School
// File Name : Utils.h
// Description : Header file for general utility functions, mainly vector operations.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/

// **=== Includes ===**
#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace Utils
{
    // **=== Public Methods ===**

    /**
     * @brief Calculates the shortest vector between two points in a toroidal (wrapping) world.
     * @param from The starting position vector.
     * @param to The target position vector.
     * @param worldSize The dimensions (width, height) of the wrapping world.
     * @return The shortest displacement vector from 'from' to 'to', considering wrapping.
     */
    sf::Vector2f shortestWrappedVector(const sf::Vector2f& from, const sf::Vector2f& to, const sf::Vector2f& worldSize);

    /**
     * @brief Calculate the magnitude (length) of a vector.
     * @param vector The vector to calculate the magnitude of.
     * @return The magnitude of the vector. (float)
     */
    float magnitude(const sf::Vector2f& vector);

    /**
     * @brief Truncates a vector to a maximum length.
     * @param vector The vector to be truncated.
     * @param max The maximum length to truncate the vector to.
     * @return The truncated vector (sf::Vector2f).
     */
    sf::Vector2f truncate(const sf::Vector2f& vector, float max);

    /**
     * @brief Normalises a 2D vector to have a magnitude of 1.
     * @param vector The vector to be normalised.
     * @return The normalised vector (sf::Vector2f).
     * Returns a zero vector if the input vector's length is zero.
     */
    sf::Vector2f normalised(const sf::Vector2f& vector);
}