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
#include "Obstacle.h"
#include <vector>

// Behaviour enum
enum class Behaviour
{
	NONE,
	SEEK,
	FLEE,
	WANDER,
	FLOCKING,
	PURSUIT,
	EVASION,
	OBSTACLE_AVOIDANCE,
	ARRIVAL,
	LEADER_FOLLOWING
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

	/**
	 * @brief Updates the agent's position and behaviour based on the specified delta time, window, agents, and obstacles.
	 * @param deltaTime Time since the last update.
	 * @param window The window to check for boundaries.
	 * @param allAgents List of all agents in the environment.
	 * @param obstacles List of obstacles in the environment.
	 */
	void update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents, const std::vector<Obstacle>& obstacles);
	/**
	 * @brief Helper to draw an SFML line.
	 * @param window The window to draw the line on.
	 * @param start Start position of the line.
	 * @param end End position of the line.
	 * @param color Color of the line. (sf::Color)
	 */
	void drawLine(sf::RenderTarget& window, const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color) const;
	/**
	 * @brief Helper to draw an SFML circle.
	 * @param window The window to draw the circle on.
	 * @param position Position of the circle.
	 * @param radius Radius of the circle.
	 * @param color Color of the circle. (sf::Color)
	 */
	void drawCircle(sf::RenderTarget& window, const sf::Vector2f& position, float radius, const sf::Color& color) const;
	/**
	 * @brief Draws all visualizations for the agent.
	 * @param window The window to draw on.
	 * @param allAgents List of all agents in the environment.
	 */
	void drawVisualizations(sf::RenderTarget& window, const std::vector<Agent*>& allAgents) const;

	// -- Setters --

	void setTargetPosition(const sf::Vector2f& position) { m_target.setPosition(position); }
	void setSpeed(float speed) { m_speedMultiplier = speed; }
	void setColor(const sf::Color& color) { m_shape.setFillColor(color); }
	void setBehaviour(Behaviour behaviour) { m_behaviour = behaviour; }
	void setAsTargetAgent(bool isTarget) { m_isTargetAgent = isTarget; }

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

	void setPursuitWeighting(float weighting) { m_pursuitWeighting = weighting; }
	void setPursuitMaxSteeringForce(float force) { m_pursuitMaxSteeringForce = force; }
	void setPursuitStrength(float strength) { m_pursuitStrength = strength; }
	void setPursuitTarget(Agent* target) { m_pursuitTarget = target; }

	void setEvasionWeighting(float weighting) { m_evasionWeighting = weighting; }
	void setEvasionMaxSteeringForce(float force) { m_evasionMaxSteeringForce = force; }
	void setEvasionStrength(float strength) { m_evasionStrength = strength; }
	void setEvasionTarget(Agent* target) { m_evasionTarget = target; }

	void setObstacleAvoidanceWeighting(float weighting) { m_obstacleAvoidanceWeighting = weighting; }
	void setObstacleAvoidanceMaxSteeringForce(float force) { m_obstacleAvoidanceMaxSteeringForce = force; }
	void setObstacleAvoidanceStrength(float strength) { m_obstacleAvoidanceStrength = strength; }
	void setObstacleDetectionBoxLength(float length) { m_obstacleDetectionLength = length; }
	void setNormalInfluence(float influence) { m_normalInfluence = influence; }
	void setTangentInfluence(float influence) { m_tangentInfluence = influence; }

	void setArrivalSlowingRadius(float radius) { m_arrivalSlowingRadius = radius; }
	void setArrivalMaxSteeringForce(float force) { m_arrivalMaxSteeringForce = force; }
	void setArrivalStrength(float strength) { m_arrivalStrength = strength; }
	void setArrivalWeighting(float weighting) { m_arrivalWeighting = weighting; }

	void setLeaderFollowingWeighting(float weighting) { m_leaderFollowingWeighting = weighting; }
	void setLeaderFollowingMaxSteeringForce(float force) { m_leaderFollowingMaxSteeringForce = force; }
	void setLeaderFollowingStrength(float strength) { m_leaderFollowingStrength = strength; }
	void setLeaderFollowingTarget(Agent* target) { m_leaderFollowingTarget = target; }
	void setLeaderFollowingOffset(const float offset) { m_followOffset = offset; }

	// -- Getters --

	const sf::Vector2f& getTargetPosition() const { return m_target.getPosition(); }
	float getSpeed() const { return m_speedMultiplier; }
	const sf::Vector2f& getVelocity() const { return m_velocity; }
	const Behaviour& getBehaviour() const { return m_behaviour; }
	bool isTargetAgent() const { return m_isTargetAgent; }

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

	float getPursuitWeighting() const { return m_pursuitWeighting; }
	float getPursuitMaxSteeringForce() const { return m_pursuitMaxSteeringForce; }
	float getPursuitStrength() const { return m_pursuitStrength; }
	const sf::Vector2f& getPursuitTargetPosition() const { return m_pursuitTarget->getPosition(); }

	float getEvasionWeighting() const { return m_evasionWeighting; }
	float getEvasionMaxSteeringForce() const { return m_evasionMaxSteeringForce; }
	float getEvasionStrength() const { return m_evasionStrength; }
	const sf::Vector2f& getEvasionTargetPosition() const { return m_evasionTarget->getPosition(); }

	float getObstacleAvoidanceWeighting() const { return m_obstacleAvoidanceWeighting; }
	float getObstacleAvoidanceMaxSteeringForce() const { return m_obstacleAvoidanceMaxSteeringForce; }
	float getObstacleAvoidanceStrength() const { return m_obstacleAvoidanceStrength; }
	float getObstacleDetectionBoxLength() const { return m_obstacleDetectionLength; }
	float getNormalInfluence() const { return m_normalInfluence; }
	float getTangentInfluence() const { return m_tangentInfluence; }

	float getArrivalSlowingRadius() const { return m_arrivalSlowingRadius; }
	float getArrivalMaxSteeringForce() const { return m_arrivalMaxSteeringForce; }
	float getArrivalStrength() const { return m_arrivalStrength; }
	float getArrivalWeighting() const { return m_arrivalWeighting; }

	float getLeaderFollowingWeighting() const { return m_leaderFollowingWeighting; }
	float getLeaderFollowingMaxSteeringForce() const { return m_leaderFollowingMaxSteeringForce; }
	float getLeaderFollowingStrength() const { return m_leaderFollowingStrength; }
	const sf::Vector2f& getLeaderFollowingTargetPosition() const { return m_leaderFollowingTarget->getPosition(); }
	float getLeaderFollowingOffset() const { return m_followOffset; }

	void setLastRotation(float rotation) { m_lastRotation = rotation; }
	float getLastRotation() const { return m_lastRotation; }

	float getDangerZoneLength() const { return m_leaderDangerZoneLength; }
	void setDangerZoneLength(float length) { m_leaderDangerZoneLength = length; }

	void setDangerZoneHalfWidth(float width) { m_leaderDangerZoneHalfWidth = width; }
	float getDangerZoneHalfWidth() const { return m_leaderDangerZoneHalfWidth; }

	float getLateralEvasionStrength() const { return m_lateralEvasionStrength; }
	void setLateralEvasionStrength(float strength) { m_lateralEvasionStrength = strength; }

private:
	// **=== Private Members ===**
	float m_maxSpeed;
	float m_speedMultiplier;
	Behaviour m_behaviour;

	sf::Transformable m_target;
	sf::Vector2f m_velocity;
	float m_lastRotation;

	// -- Shape --
	sf::ConvexShape m_shape;  // Shape representing the agent
	sf::Vector2f m_agentSize; // Size of the shape

	// -- Steering Forces --
	bool m_isTargetAgent;
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
	// Cohesion
	float m_cohesionWeighting;
	sf::Vector2f m_cohesionDesiredVelocity;
	float m_cohesionNeighbourhoodRadius;
	float m_cohesionStrength;
	float m_cohesionMaxSteeringForce;
	sf::Vector2f m_cohesionCenterOfMass;
	bool m_cohesionIncludesSelf;
	// Separation
	float m_separationWeighting;
	sf::Vector2f m_separationDesiredVelocity;
	float m_separationNeighbourhoodRadius;
	float m_separationStrength;
	float m_separationMaxSteeringForce;
	// Alignment
	float m_alignmentWeighting;
	sf::Vector2f m_alignmentDesiredVelocity;
	float m_alignmentNeighbourhoodRadius;
	float m_alignmentStrength;
	float m_alignmentMaxSteeringForce;
	// Pursuit
	float m_pursuitWeighting;
	sf::Vector2f m_pursuitDesiredVelocity;
	float m_pursuitMaxSteeringForce;
	float m_pursuitStrength;
	Agent* m_pursuitTarget;
	sf::Vector2f m_predictedTargetPos;
	// Evasion
	float m_evasionWeighting;
	sf::Vector2f m_evasionDesiredVelocity;
	float m_evasionMaxSteeringForce;
	float m_evasionStrength;
	Agent* m_evasionTarget;
	// Obstacle avoidance
	float m_obstacleAvoidanceWeighting;
	sf::Vector2f m_obstacleAvoidanceDesiredVelocity;
	float m_obstacleAvoidanceMaxSteeringForce;
	float m_obstacleAvoidanceStrength;
	float m_obstacleDetectionLength; // How far ahead the agent looks
	sf::Vector2f m_detectionFeelerP1;
	sf::Vector2f m_detectionFeelerP2;
	bool m_closestThreatFound;
	sf::Vector2f m_intersectionPoint;
	sf::Vector2f m_threatNormal;
	float m_normalInfluence;
	float m_tangentInfluence;
	// Arrival
	float m_arrivalWeighting;
	float m_arrivalMaxSteeringForce;
	float m_arrivalStrength;
	float m_arrivalSlowingRadius;
	sf::Vector2f m_arrivalDesiredVelocity;
	// Leader following
	Agent* m_leaderFollowingTarget;
	float m_followOffset;
	float m_leaderFollowingWeighting;
	float m_leaderFollowingMaxSteeringForce;
	float m_leaderFollowingStrength;
	sf::Vector2f m_leaderFollowingDesiredVelocity;
	float m_leaderDangerZoneLength;
	float m_leaderDangerZoneHalfWidth;
	float m_lateralEvasionStrength;

	// **=== Private Methods ===**

	/**
	 * @brief Draws the agent on the specified render window. (Overriding sf::Drawable's draw method)
	 * @param window The window to draw on.
	 * @param states State used for drawing to the window.
	 */
	virtual void draw(sf::RenderTarget& window, sf::RenderStates states) const override;
	/**
	 * @brief Sets up the shape of the agent.
	 */
	void setupShape(); 
	/**
	 * @brief Handles the agent's position when it reaches the window boundaries.
	 * @param windowSize Size of the window for boundary handling.
	 */
	void handleBoundary(const sf::RenderWindow& window);

	// **=== Visualizations ===**

	/**
	 * @brief Draws velocity line for the agent.
	 * @param window The window to draw on.
	 */
	void drawVelocityLine(sf::RenderTarget& window) const;
	/**
	 * @brief Draws the visuals for the current behaviour.
	 * @param window The window to draw on.
	 */
	void drawBehaviourVisuals(sf::RenderTarget& target, const std::vector<Agent*>& allAgents) const;

	// **=== Steering Behaviours ===**

	/**
	 * @brief Applies steering force to the agent based on desired velocity.
	 * @param desiredVelocity The desired velocity to steer towards.
	 * @param maxSteeringForce The maximum steering force to apply.
	 * @param strength The strength of the steering force.
	 * @param weighting The weighting of the steering force.
	 * @param deltaTime Time since the last update.
	 */
	void applySteeringFromDesiredVelocity(const sf::Vector2f& desiredVelocity, float maxSteeringForce, float strength, float weighting, float deltaTime);
	/**
	 * @brief Applies steering force to the agent based on a given force.
	 * @param force The steering force to apply.
	 * @param maxSteeringForce The maximum steering force to apply.
	 * @param strength The strength of the steering force.
	 * @param weighting The weighting of the steering force.
	 * @param deltaTime Time since the last update.
	 */
	void applySteeringFromForce(sf::Vector2f force, float maxSteeringForce, float strength, float weighting, float deltaTime);
	/**
	 * @brief Calculates and applies the steering force for the seek behaviour.
	 * @param deltaTime Time since the last update.
	 */
	void seek(float deltaTime);
	/**
	 * @brief Calculates and applies the steering force for the flee behaviour.
	 * @param deltaTime Time since the last update.
	 */
	void flee(float deltaTime);
	/**
	 * @brief Calculates and applies the steering force for the wander behaviour.
	 * @param deltaTime Time since the last update.
	 */
	void wander(float deltaTime);
	/**
	 * @brief Calculates and applies the steering force for the seperation behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void separation(float deltaTime, const std::vector<Agent*>& allAgents);
	/**
	 * @brief Calculates and applies the steering force for the cohesion behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void cohesion(float deltaTime, const std::vector<Agent*>& allAgents);
	/**
	 * @brief Calculates and applies the steering force for the alignment behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void alignment(float deltaTime, const std::vector<Agent*>& allAgents);
	/**
	 * @brief Calculates and applies the steering force for the pursuit behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void pursuit(float deltaTime, const std::vector<Agent*>& allAgents);
	/**
	 * @brief Calculates and applies the steering force for the evasion behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void evasion(float deltaTime, const std::vector<Agent*>& allAgents);
	/**
	 * @brief Calculates and applies the steering force for the obstacle avoidance behaviour.
	 * @param deltaTime Time since the last update.
	 * @param obstacles List of obstacles in the environment.
	 */
	void obstacleAvoidance(float deltaTime, const std::vector<Obstacle>& obstacles);
	/**
	 * @brief Calculates and applies the steering force for the arrival behaviour.
	 * @param deltaTime Time since the last update.
	 */
	void arrival(float deltaTime);
	/**
	 * @brief Calculates and applies the steering force for the leader following behaviour.
	 * @param deltaTime Time since the last update.
	 * @param allAgents List of all agents in the environment.
	 */
	void leaderFollowing(float deltaTime, const std::vector<Agent*>& allAgents);
};

