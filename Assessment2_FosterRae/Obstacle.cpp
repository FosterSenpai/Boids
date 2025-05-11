// Obstacle.cpp
#include "Obstacle.h" // Include the header file we just created

// Constructor Implementation
Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size)
    : m_position(position), m_size(size) {
    // Configure the SFML shape
    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);
    m_shape.setFillColor(sf::Color(120, 120, 120)); // A medium grey color
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(2.0f); // A visible outline
}

// Draw Method Implementation
void Obstacle::draw(sf::RenderTarget& target) const {
    target.draw(m_shape); // Draw the SFML shape
}

// getMinBound Implementation
sf::Vector2f Obstacle::getMinBound() const {
    return m_position; // Top-left corner is the min bound for AABB
}

// getMaxBound Implementation
sf::Vector2f Obstacle::getMaxBound() const {
    return m_position + m_size; // Bottom-right corner
}