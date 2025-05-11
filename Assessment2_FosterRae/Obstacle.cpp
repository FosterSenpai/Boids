// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2025 Media Design School
// File Name : Obstacle.cpp
// Description : Implementation file for the Obstacle class. Defines the properties.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/
#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f position, sf::Vector2f size)
    : m_position(position), m_size(size) {
    // Configure the shape
    m_shape.setPosition(m_position);
    m_shape.setSize(m_size);
    m_shape.setFillColor(sf::Color(120, 120, 120));
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(2.0f);
}

void Obstacle::draw(sf::RenderTarget& target) const {
    target.draw(m_shape);
}

sf::Vector2f Obstacle::getMinBound() const {
    return m_position; // Top-left corner
}

sf::Vector2f Obstacle::getMaxBound() const {
    return m_position + m_size; // Bottom-right corner
}