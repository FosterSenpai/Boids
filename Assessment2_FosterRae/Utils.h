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

    /**
     * @brief Generates a random floating-point number within a specified range [min, max].
     * @param min The minimum value of the range (inclusive).
     * @param max The maximum value of the range (inclusive).
     * @return A random float between min and max.
     */
    float randomRange(float min, float max);

    /**
	 * @brief Checks if a line segment intersects with an Axis-Aligned Bounding Box (AABB).
	 * @param lineP1 Point 1 of the line segment.
	 * @param lineP2 Point 2 of the line segment.
	 * @param rectMin The minimum corner of the AABB (top-left).
	 * @param rectMax The maximum corner of the AABB (bottom-right).
	 * @param tIntersection How far along the line the intersection occurs (0.0 to 1.0), passed by reference to have access to the value.
	 * @param outIntersectionNormal The normal vector at the intersection point.
	 * @return True if the line intersects the AABB, false otherwise.
     */
    bool lineIntersectsAABB(const sf::Vector2f& lineP1, const sf::Vector2f& lineP2, const sf::Vector2f& rectMin,
        const sf::Vector2f & rectMax, float& tIntersection, sf::Vector2f& outIntersectionNormal);
}