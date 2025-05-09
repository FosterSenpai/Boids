// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2025 Media Design School
// File Name : Agent.h
// Description : Header file for the Agent class. Represents an entity that can
//               move within the environment and exhibit steering behaviours.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/

// **=== Includes ===**
#pragma once
#include <SFML/Graphics.hpp>

// Movement enum
enum class MovementType
{
	NONE,
	SEEK,
	FLEE,
	PURSUE,
	ARRIVAL
};

class Agent : public sf::Drawable, public sf::Transformable // Inherit from sf::Drawable & sf::Transformable
{
public:
    // **=== Constructor ===**  
	Agent(sf::Vector2f& spawnPos);


	// **=== Public Methods ===**
	void update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents);
	void drawVisualizations(sf::RenderTarget& target) const;
	// -- Setters --
	void setTargetPosition(const sf::Vector2f& position) { m_target.setPosition(position); }
	void setSeekStrength(float strength) { m_seekStrength = strength; }
	void setSpeed(float speed) { m_speedMultiplier = speed; }
	// -- Getters --
	const sf::Vector2f& getTargetPosition() const { return m_target.getPosition(); }
	float getSeekStrength() const { return m_seekStrength; }
	float getSpeed() const { return m_speedMultiplier; }
private:
	// **=== Private Members ===**
	float m_maxSpeed;
	float m_speedMultiplier; // Speed multiplier for the agent

	sf::Transformable m_target;
	sf::Vector2f m_velocity;
	sf::Vector2f m_desiredVelocity; // Desired velocity towards the target (Update from movement behaviours)

	// -- Shape --
	sf::ConvexShape m_shape;  // Shape representing the agent
	sf::Vector2f m_agentSize; // Size of the shape

	// -- Steering Forces --
	MovementType m_movementType; // Type of movement behaviour
	float m_seekSteerMaxForce; // Maximum steering force for seek behaviour
	float m_seekStrength;      // Strength multiplier for the seek force

	// **=== Private Methods ===**
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override; // Overriding sf::Drawable's draw method
	void setupShape(); 

	/**
	 * @brief Handles the agent's position when it reaches the window boundaries.
	 * @param windowSize Size of the window for boundary handling.
	 */
	void handleBoundary(const sf::RenderWindow& window);

	// **=== Visualizations ===**
	void drawVelocityLine(sf::RenderTarget& target) const;
	void drawDesiredVelocityLine(sf::RenderTarget& target) const;
};

