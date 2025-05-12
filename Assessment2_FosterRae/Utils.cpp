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
#include <random>

namespace Utils
{
    // **=== Public Methods ===**

    float magnitude(const sf::Vector2f& vector) {
		// TODO: Make squared mag func or something and use where possible to avoid sqrt
        return std::sqrt(vector.x * vector.x + vector.y * vector.y); // Magnitude = sqrt(x^2 + y^2)
    }

    float dot(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
    {
        return vec1.x * vec2.x + vec1.y * vec2.y;
    }

    sf::Vector2f truncate(const sf::Vector2f& vector, float max) {
        float length = magnitude(vector); // Calculate magnitude

        if (length > max && length > 0.00001f) { // Check length > 0 to avoid division by zero
            return (vector / length) * max;
        }
        return vector; // Return original vector if no truncation needed or if it's a zero vector
    }

    sf::Vector2f normalised(const sf::Vector2f& vector) {
        float length = magnitude(vector);
        if (length > 0.0f)
        {
            return vector / length; // Divide vector by its length to get a unit vector
        }
        return { 0.0f, 0.0f }; // Return zero vector if the original vector had zero length
    }

	// Helper function to get rng engine
    std::mt19937& getRandomEngine()
    {
        static std::mt19937 engine(std::random_device{}());
        return engine;
    }

    float randomRange(float min, float max)
    {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(getRandomEngine()); // Uses the static engine
    }
    
    bool lineIntersectsAABB(const sf::Vector2f& lineP1, const sf::Vector2f& lineP2, const sf::Vector2f& rectMin, const sf::Vector2f& rectMax, float& tIntersection, sf::Vector2f& outIntersectionNormal)
    {
		sf::Vector2f dir = lineP2 - lineP1; // Direction vector of the line segment
		sf::Vector2f invDir;                // Inverse direction vector

        // Handle division by zero
        invDir.x = (dir.x == 0.0f) ? std::numeric_limits<float>::infinity() : 1.0f / dir.x;
        invDir.y = (dir.y == 0.0f) ? std::numeric_limits<float>::infinity() : 1.0f / dir.y;

        float tx1 = (rectMin.x - lineP1.x) * invDir.x;
        float tx2 = (rectMax.x - lineP1.x) * invDir.x;
        float ty1 = (rectMin.y - lineP1.y) * invDir.y;
        float ty2 = (rectMax.y - lineP1.y) * invDir.y;

        float tEnterX = std::min(tx1, tx2);
        float tExitX = std::max(tx1, tx2);
        float tEnterY = std::min(ty1, ty2);
        float tExitY = std::max(ty1, ty2);

        float tEnter = std::max(tEnterX, tEnterY); // Latest entry time into all slabs
        float tExit = std::min(tExitX, tExitY);    // Earliest exit time from all slabs

		// Check if the line segment is completely outside the AABB
        if (tEnter > tExit || tExit < 0.0f) {
            return false;
        }

		// If intersection point is beyond p2 of the segment return false
        tIntersection = std::max(0.0f, tEnter);
        if (tIntersection > 1.0f) {
            return false;
        }

		// -- Intersected, calculate the normal --
        // Calculate the intersection normal
		if (tEnterX > tEnterY) { // If entered through X
            if (invDir.x < 0.0f) { // Line dir is negative X
                outIntersectionNormal = sf::Vector2f(1.0f, 0.0f); // Hit right face of AABB
            }
            else {
                outIntersectionNormal = sf::Vector2f(-1.0f, 0.0f); // Hit left face of AABB
            }
        }
        else { // Entered through Y
            if (invDir.y < 0.0f) { // Line dir is negative Y
                outIntersectionNormal = sf::Vector2f(0.0f, 1.0f); // Hit bottom face of AABB
            }
            else {
                outIntersectionNormal = sf::Vector2f(0.0f, -1.0f); // Hit top face of AABB
            }
        }

        return true; // Intersection found
    }

}