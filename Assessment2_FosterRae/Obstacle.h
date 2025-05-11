// Obstacle.h
#pragma once // Prevents the header from being included multiple times

#include <SFML/Graphics.hpp> // For sf::Vector2f, sf::RectangleShape, sf::RenderTarget

class Obstacle {
public:
    // Member Variables
    sf::Vector2f m_position;      // Top-left corner of the rectangle
    sf::Vector2f m_size;          // Width and height of the rectangle
    sf::RectangleShape m_shape;   // SFML shape for drawing

    // Constructor
    Obstacle(sf::Vector2f position, sf::Vector2f size);

    // Method to draw the obstacle
    void draw(sf::RenderTarget& target) const;

    // Helper methods to get the Axis-Aligned Bounding Box (AABB) limits
    sf::Vector2f getMinBound() const; // Top-left corner
    sf::Vector2f getMaxBound() const; // Bottom-right corner
};