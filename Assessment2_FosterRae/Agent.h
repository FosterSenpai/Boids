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
	ARRIVAL,
	WANDER
};

class Agent : public sf::Drawable, public sf::Transformable // Inherit from sf::Drawable & sf::Transformable
{
public:
    // **=== Constructor ===**  

	/**
	 * @brief Constructs an Agent object with a specified spawn position.
	 * @param spawnPos The spawn position of the agent.
	 */
	Agent(sf::Vector2f& spawnPos);

	// **=== Public Methods ===**
	void update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents);
	void drawVisualizations(sf::RenderTarget& target) const;

	// -- Setters --
	void setTargetPosition(const sf::Vector2f& position) { m_target.setPosition(position); }
	void setSpeed(float speed) { m_speedMultiplier = speed; }

	void setSeekWeighting(float weighting) { m_seekWeighting = weighting; }
	void setSeekMaxSteeringForce(float force) { m_seekMaxSteeringForce = force; }
	void setSeekStrength(float strength) { m_seekStrength = strength; }

	void setFleeWeighting(float weighting) { m_fleeWeighting = weighting; }
	void setFleeMaxSteeringForce(float force) { m_fleeMaxSteeringForce = force; }
	void setFleeStrength(float strength) { m_fleeStrength = strength; }

	void setWanderWeighting(float weighting) { m_wanderWeighting = weighting; }
	void setWanderMaxSteeringForce(float force) { m_wanderMaxSteeringForce = force; }
	void setWanderStrength(float strength) { m_wanderStrength = strength; }
	void setWanderRadius(float radius) { m_wanderRadius = radius; }
	void setWanderDistance(float distance) { m_wanderDistance = distance; }
	void setWanderAngleRandomStrength(float strength) { m_wanderAngleRandomStrength = strength; }

	void setFlockingWeighting(float weighting) { m_flockingWeighting = weighting; }

	void setCohesionWeighting(float weighting) { m_cohesionWeighting = weighting; }
	void setCohesionMaxSteeringForce(float force) { m_cohesionMaxSteeringForce = force; }
	void setCohesionStrength(float strength) { m_cohesionStrength = strength; }
	void setCohesionNeighbourhoodRadius(float radius) { m_cohesionNeighbourhoodRadius = radius; }

	void setSeparationWeighting(float weighting) { m_separationWeighting = weighting; }
	void setSeparationMaxSteeringForce(float force) { m_separationMaxSteeringForce = force; }
	void setSeparationStrength(float strength) { m_separationStrength = strength; }
	void setSeparationNeighbourhoodRadius(float radius) { m_separationNeighbourhoodRadius = radius; }

	void setAlignmentWeighting(float weighting) { m_alignmentWeighting = weighting; }
	void setAlignmentMaxSteeringForce(float force) { m_alignmentMaxSteeringForce = force; }
	void setAlignmentStrength(float strength) { m_alignmentStrength = strength; }
	void setAlignmentNeighbourhoodRadius(float radius) { m_alignmentNeighbourhoodRadius = radius; }


	// -- Getters --
	const sf::Vector2f& getTargetPosition() const { return m_target.getPosition(); }
	float getSpeed() const { return m_speedMultiplier; }

	float getSeekWeighting() const { return m_seekWeighting; }
	float getSeekMaxSteeringForce() const { return m_seekMaxSteeringForce; }
	float getSeekStrength() const { return m_seekStrength; }

	float getFleeWeighting() const { return m_fleeWeighting; }
	float getFleeMaxSteeringForce() const { return m_fleeMaxSteeringForce; }
	float getFleeStrength() const { return m_fleeStrength; }

	float getWanderWeighting() const { return m_wanderWeighting; }
	float getWanderMaxSteeringForce() const { return m_wanderMaxSteeringForce; }
	float getWanderStrength() const { return m_wanderStrength; }
	float getWanderRadius() const { return m_wanderRadius; }
	float getWanderDistance() const { return m_wanderDistance; }
	float getWanderAngleRandomStrength() const { return m_wanderAngleRandomStrength; }

	float getFlockingWeighting() const { return m_flockingWeighting; }

	float getCohesionWeighting() const { return m_cohesionWeighting; }
	float getCohesionMaxSteeringForce() const { return m_cohesionMaxSteeringForce; }
	float getCohesionStrength() const { return m_cohesionStrength; }
	float getCohesionNeighbourhoodRadius() const { return m_cohesionNeighbourhoodRadius; }

	float getSeparationWeighting() const { return m_separationWeighting; }
	float getSeparationMaxSteeringForce() const { return m_separationMaxSteeringForce; }
	float getSeparationStrength() const { return m_separationStrength; }
	float getSeparationNeighbourhoodRadius() const { return m_separationNeighbourhoodRadius; }

	float getAlignmentWeighting() const { return m_alignmentWeighting; }
	float getAlignmentMaxSteeringForce() const { return m_alignmentMaxSteeringForce; }
	float getAlignmentStrength() const { return m_alignmentStrength; }
	float getAlignmentNeighbourhoodRadius() const { return m_alignmentNeighbourhoodRadius; }

private:
	// **=== Private Members ===**
	float m_maxSpeed;
	float m_speedMultiplier; // Speed multiplier for the agent

	sf::Transformable m_target;
	sf::Vector2f m_velocity;

	// -- Shape --
	sf::ConvexShape m_shape;  // Shape representing the agent
	sf::Vector2f m_agentSize; // Size of the shape

	// -- Steering Forces --
	// Seek
	float m_seekWeighting;
	sf::Vector2f m_seekDesiredVelocity;
	float m_seekMaxSteeringForce;
	float m_seekStrength;
	// Flee
	float m_fleeWeighting;
	sf::Vector2f m_fleeDesiredVelocity;
	float m_fleeMaxSteeringForce;
	float m_fleeStrength;
	// Wander
	float m_wanderWeighting;
	sf::Vector2f m_wanderDesiredVelocity;
	float m_wanderMaxSteeringForce;
	float m_wanderStrength;
	float m_wanderRadius;
	float m_wanderDistance;
	float m_wanderAngle;
	float m_wanderAngleRandomStrength;
	float m_wanderAdjustmentTimer;
	float m_targetWanderAngle;
	// Flocking
	float m_flockingWeighting;

	float m_cohesionWeighting;
	sf::Vector2f m_cohesionDesiredVelocity;
	float m_cohesionNeighbourhoodRadius;
	float m_cohesionStrength;
	float m_cohesionMaxSteeringForce;

	float m_separationWeighting;
	sf::Vector2f m_separationDesiredVelocity;
	float m_separationNeighbourhoodRadius;
	float m_separationStrength;
	float m_separationMaxSteeringForce;

	float m_alignmentWeighting;
	sf::Vector2f m_alignmentDesiredVelocity;
	float m_alignmentNeighbourhoodRadius;
	float m_alignmentStrength;
	float m_alignmentMaxSteeringForce;


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
	void drawDesiredVelocityLines(sf::RenderTarget& target) const;

	// **=== Steering Behaviours ===**
	void seek(float deltaTime);
	void flee(float deltaTime);
	void wander(float deltaTime);
	void separate(float deltaTime, const std::vector<Agent*>& allAgents);
};

