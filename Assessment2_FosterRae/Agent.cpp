// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// (c) 2025 Media Design School
// File Name : Agent.cpp
// Description : Implementation file for the Agent class. Defines the behaviour
//               and properties of an agent in the simulation.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/


// **=== Includes ===**
#include "Agent.h"
#include "Utils.h"
#include <SFML/System/Angle.hpp>
#include <random>

// **=== Constructor ===**

Agent::Agent(sf::Vector2f& spawnPos)
	: m_maxSpeed(20.0f),
	m_velocity(0.0f, 0.0f),
	m_agentSize(5.0f, 7.0f),
	m_speedMultiplier(25.0f),
	m_movementType(MovementType::SEEK),

	m_seekMaxSteeringForce(5.0f),
	m_seekStrength(10.0f),

	m_fleeMaxSteeringForce(5.0f),
	m_fleeStrength(10.0f),

	m_wanderMaxSteeringForce(5.0f),
	m_wanderStrength(10.0f),
	m_wanderRadius(10.0f),
	m_wanderDistance(25.0f),
	m_wanderAngle(0.0f),
	m_wanderAngleRandomStrength(0.5f),
	m_wanderAdjustmentTimer(0.0f),
	m_targetWanderAngle(0.0f)
{
	this->setPosition(spawnPos);    // Set the agent's position to spawn position
	m_target.setPosition(spawnPos); // set for now, gets overridden in update
	setupShape();                   // Setup the shape of the agent
}

void Agent::update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents)
{
    //----------------------------------------------------------------
    // STEERING BEHAVIOURS
    //----------------------------------------------------------------
	switch (m_movementType)
	{
	case MovementType::NONE:
		break;
	case MovementType::SEEK:
	{
		// Calc desired velocity seek(target position - current position) 
		m_seekDesiredVelocity = Utils::normalised(m_target.getPosition() - this->getPosition()) * m_maxSpeed;
		// Update Steering Force
		sf::Vector2f seekSteerForce = m_seekDesiredVelocity - m_velocity;
		seekSteerForce = Utils::truncate(seekSteerForce, m_seekMaxSteeringForce);
		// Update Velocity
		m_velocity += seekSteerForce * m_seekStrength * deltaTime; // Apply steering force to velocity
		m_velocity = Utils::truncate(m_velocity, m_maxSpeed);
		break;
	}
	case MovementType::FLEE:
	{
		// Calc desired velocity flee(target position - current position) 
		m_fleeDesiredVelocity = Utils::normalised(this->getPosition() - m_target.getPosition()) * m_maxSpeed;
		// Update Steering Force
		sf::Vector2f fleeSteerForce = m_fleeDesiredVelocity - m_velocity;
		fleeSteerForce = Utils::truncate(fleeSteerForce, m_fleeMaxSteeringForce);
		// Update Velocity
		m_velocity += fleeSteerForce * m_fleeStrength * deltaTime;
		m_velocity = Utils::truncate(m_velocity, m_maxSpeed);
		break;
	}
	case MovementType::PURSUE:
		break;
	case MovementType::ARRIVAL:
		break;
	case MovementType::WANDER:
	{
		// Wander timer
		if (m_wanderAdjustmentTimer < 0.0f) {
			// Reset the timer
			m_wanderAdjustmentTimer = Utils::randomRange(1.0f, 3.0f); // 1 to 3 seconds
			// Randomly change the wander angle
			m_targetWanderAngle += Utils::randomRange(-1, 1) * m_wanderAngleRandomStrength;
		}
		else {
			m_wanderAdjustmentTimer -= deltaTime; // Decrease the timer
		}

		// Lerp the wander angle towards the target angle
		m_wanderAngle = std::lerp(m_wanderAngle, m_targetWanderAngle, 5.0f * deltaTime);
		// Calculate the target position for the wander circle
		sf::Vector2f circlePosition = this->getPosition() + Utils::normalised(m_velocity) * m_wanderDistance;
		sf::Vector2f targetPosition = circlePosition + sf::Vector2f(std::cos(m_wanderAngle), std::sin(m_wanderAngle)) * m_wanderRadius;
		// Calculate the desired velocity towards the target position
		m_wanderDesiredVelocity = Utils::normalised(targetPosition - this->getPosition()) * m_maxSpeed;
		// Update Steering Force
		sf::Vector2f wanderSteerForce = m_wanderDesiredVelocity - m_velocity;
		wanderSteerForce = Utils::truncate(wanderSteerForce, m_wanderMaxSteeringForce);
		// Update Velocity
		m_velocity += wanderSteerForce * m_wanderStrength * deltaTime;
		m_velocity = Utils::truncate(m_velocity, m_maxSpeed);
		break;
	}
	default:
		break;
	}

    //----------------------------------------------------------------
    // MOVEMENT & PHYSICS
    //----------------------------------------------------------------

    // -- Update Agent Position --
    this->setPosition(this->getPosition() + m_velocity * m_speedMultiplier * deltaTime); // Update position based on velocity

    // -- Update Agent Rotation --
    if (Utils::magnitude(m_velocity) > 0.01f) // Only rotate if moving to avoid jittering
    {
        float angle = std::atan2(m_velocity.y, m_velocity.x) * 180.0f / 3.1415926535f;
        this->setRotation(sf::degrees(angle + 90.0f));
    }

    // -- Handle Screen Boundaries --
    handleBoundary(window); // Wrap agent position around the screen
}

// **=== Public Methods ===**

void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // Apply the agent's entire current transform
	target.draw(m_shape, states);       // Draw the pre-configured shape
}

void Agent::setupShape()
{
	// Points
	m_shape.setPointCount(3);

	m_shape.setPoint(0, sf::Vector2f(0.0f, -m_agentSize.y));		  // Nose point
	m_shape.setPoint(1, sf::Vector2f(-m_agentSize.x, m_agentSize.y)); // Bottom left point
	m_shape.setPoint(2, sf::Vector2f(m_agentSize.x, m_agentSize.y));  // Bottom right point

	// Colors
	m_shape.setFillColor(sf::Color(50, 50, 50));
	m_shape.setOutlineColor(sf::Color(10, 10, 10));
	m_shape.setOutlineThickness(1.0f);

	// Origin
	this->setOrigin({ 0.0f,0.0f });
}

void Agent::handleBoundary(const sf::RenderWindow& window)  
{  
   sf::Vector2u windowSize = window.getSize(); // Get the window size  
   float windowSizeX = static_cast<float>(windowSize.x);
   float windowSizeY = static_cast<float>(windowSize.y);

   // Wrap on x-axis  
   if (this->getPosition().x < 0) this->setPosition({ windowSizeX, this->getPosition().y});
   else if (this->getPosition().x > windowSizeX) this->setPosition({0, this->getPosition().y});

   // Wrap on y-axis  
   if (this->getPosition().y < 0) this->setPosition({this->getPosition().x, windowSizeY });
   else if (this->getPosition().y > windowSizeY) this->setPosition({this->getPosition().x, 0});
}

// **=== Visualizations ===**

void Agent::drawVisualizations(sf::RenderTarget& target) const
{
	// TODO: Change different visuals on switch statement for different behaviours
	// Draw visuals
	drawVelocityLine(target);
	drawDesiredVelocityLines(target);
}

void Agent::drawVelocityLine(sf::RenderTarget& target) const
{
	float length = 3.0f; // Length of the line

	// Create a VertexArray for the line
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);

	// Set the starting point of the line (agent's position + a bit)
	line[0].position = this->getPosition() + m_velocity * 0.2f;
	line[0].color = sf::Color::Red;

	// Set the end point of the line (position + velocity)
	sf::Vector2f endPoint = this->getPosition() + m_velocity * length;
	line[1].position = endPoint;
	line[1].color = sf::Color::Red;

	// Draw the line
	target.draw(line);
}

void Agent::drawDesiredVelocityLines(sf::RenderTarget& target) const
{
	switch (m_movementType)
	{
	case MovementType::NONE:
		break;
	case MovementType::SEEK:
	{
		// -- Seek Desired Velocity Line --
		float length = 3.0f; // Length of the line
		// Create a VertexArray for the line
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		// Set the starting point of the line (agent's position + a bit)
		line[0].position = this->getPosition() + m_velocity * 0.2f;
		line[0].color = sf::Color::Green;
		// Set the end point of the line (position + velocity)
		sf::Vector2f endPoint = this->getPosition() + m_seekDesiredVelocity * length;
		line[1].position = endPoint;
		line[1].color = sf::Color::Green;
		// Draw the line
		target.draw(line);
		break;
	}
	case MovementType::FLEE:
	{
		// -- Flee Desired Velocity Line --
		float length = 3.0f; // Length of the line
		// Create a VertexArray for the line
		sf::VertexArray fleeLine(sf::PrimitiveType::Lines, 2);
		// Set the starting point of the line (agent's position + a bit)
		fleeLine[0].position = this->getPosition() + m_velocity * 0.2f;
		fleeLine[0].color = sf::Color::Blue;
		// Set the end point of the line (position + velocity)
		sf::Vector2f fleeEndPoint = this->getPosition() + m_fleeDesiredVelocity * length;
		fleeLine[1].position = fleeEndPoint;
		fleeLine[1].color = sf::Color::Blue;
		// Draw the line
		target.draw(fleeLine);
		break;
	}
	case MovementType::PURSUE:
		break;
	case MovementType::ARRIVAL:
		break;
	case MovementType::WANDER:
	{
		// Draw the wander circle pas + velocity + wander distance
		sf::CircleShape circle(m_wanderRadius);
		sf::Vector2f circlePos = this->getPosition() + Utils::normalised(m_velocity) * m_wanderDistance;
		circle.setFillColor(sf::Color(255, 0, 0, 100)); // Semi-transparent red
		circle.setPosition(circlePos);
		circle.setOrigin({m_wanderRadius, m_wanderRadius});
		circle.setOutlineColor(sf::Color::Red);
		circle.setOutlineThickness(1.0f); // Set outline thickness
		target.draw(circle); // Draw the circle

		// Draw the wander angle
		sf::VertexArray angleLine(sf::PrimitiveType::Lines, 2);
		angleLine[0].position = circlePos;
		angleLine[0].color = sf::Color::Yellow;

		// Calculate the end point of the angle line
		sf::Vector2f displacement_on_circle(
			std::cos(m_wanderAngle) * m_wanderRadius,
			std::sin(m_wanderAngle) * m_wanderRadius
		);

		sf::Vector2f angleEndPoint = circlePos + displacement_on_circle;
		angleLine[1].position = angleEndPoint;
		angleLine[1].color = sf::Color::Yellow;
		// Draw the angle line
		target.draw(angleLine);

		break;
	}
	default:
		break;
	}
}


