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

// **=== Constructor ===**

Agent::Agent(sf::Vector2f& spawnPos)
	: m_maxSpeed(20.0f),
	m_velocity(0.0f, 0.0f),
	m_agentSize(5.0f, 7.0f),
	m_speedMultiplier(1.0f),
	m_seekSteerMaxForce(100.0f),
	m_seekStrength(0.5f),
	m_movementType(MovementType::SEEK)
{
	this->setPosition(spawnPos);    // Set the agent's position to spawn position
	m_target.setPosition(spawnPos); // set for now, gets overridden in update
	setupShape();                   // Setup the shape of the agent
}

void Agent::update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents)
{
    //----------------------------------------------------------------
    // INPUT & TARGETING
    //----------------------------------------------------------------
    // -- Update Target from Mouse Position --
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
    sf::Vector2f mouseFloatPos(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));
    m_target.setPosition(mouseFloatPos); // Set the target position to the mouse position

    //----------------------------------------------------------------
    // STEERING BEHAVIOURS
    //----------------------------------------------------------------
    // -- Seek Behaviour Calculation --
	/*switch (m_movementType) {
	case MovementType::SEEK:
		break;

	}*/

    m_desiredVelocity = Utils::normalised(m_target.getPosition() - this->getPosition()) * m_maxSpeed;
    sf::Vector2f steerForce = m_desiredVelocity - m_velocity;
    Utils::truncate(steerForce, m_seekSteerMaxForce); // Limit the steering force to max

    //----------------------------------------------------------------
    // MOVEMENT & PHYSICS
    //----------------------------------------------------------------
    // -- Update Velocity --
    m_velocity += steerForce * m_seekStrength * deltaTime; // Apply steering force to velocity
    m_velocity = Utils::truncate(m_velocity, m_maxSpeed);  // Limit velocity to max speed

    // -- Update Position --
    this->setPosition(this->getPosition() + m_velocity * m_speedMultiplier * deltaTime); // Update position based on velocity

    //----------------------------------------------------------------
    // VISUALS & ORIENTATION
    //----------------------------------------------------------------
    // -- Update Agent Rotation --
    if (Utils::magnitude(m_velocity) > 0.01f) // Only rotate if moving to avoid jittering
    {
        float angle = std::atan2(m_velocity.y, m_velocity.x) * 180.0f / 3.1415926535f;
        this->setRotation(sf::degrees(angle + 90.0f));
    }

    //----------------------------------------------------------------
    // WORLD INTERACTION
    //----------------------------------------------------------------
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
	// Draw visuals
	drawVelocityLine(target);
	drawDesiredVelocityLine(target);
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

void Agent::drawDesiredVelocityLine(sf::RenderTarget& target) const
{
	float length = 3.0f; // Length of the line
	// Create a VertexArray for the line
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	// Set the starting point of the line (agent's position + a bit)
	line[0].position = this->getPosition() + m_velocity * 0.2f;
	line[0].color = sf::Color::Green;
	// Set the end point of the line (position + velocity)
	sf::Vector2f endPoint = this->getPosition() + m_desiredVelocity * length;
	line[1].position = endPoint;
	line[1].color = sf::Color::Green;
	// Draw the line
	target.draw(line);
}


