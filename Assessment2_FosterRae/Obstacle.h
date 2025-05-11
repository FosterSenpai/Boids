// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2025 Media Design School
// File Name : Obstacle.h
// Description : Header file for the Obstacle class. Represents a rectangular obstacle.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/
#pragma once

#include <SFML/Graphics.hpp> // For sf::Vector2f, sf::RectangleShape, sf::RenderTarget

class Obstacle {
public:
	// **=== Constructor ===**

    /**
	 * @brief Constructs an Obstacle object with a specified position and size.
	 * @param position The top-left corner position of the obstacle.
	 * @param size The size (width and height) of the obstacle.
     */
    Obstacle(sf::Vector2f position, sf::Vector2f size);

	// **=== Public Methods ===**

    /**
	 * @brief Draws the obstacle on the specified render window.
	 * @param window The window to draw the obstacle on.
     */
    void draw(sf::RenderTarget& window) const;

    // Helper methods to get the Axis-Aligned Bounding Box (AABB) limits

    sf::Vector2f getMinBound() const; // Top-left corner
    sf::Vector2f getMaxBound() const; // Bottom-right corner

	// **=== Member Variables ===**

    sf::Vector2f m_position;      // Top-left corner of the rectangle
    sf::Vector2f m_size;          // Width and height of the rectangle
    sf::RectangleShape m_shape;   // SFML shape for drawing
};