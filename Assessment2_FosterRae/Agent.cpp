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
	m_speedMultiplier(15.0f),
	m_behaviour(Behaviour::NONE),

	m_seekWeighting(0.0f),
	m_seekMaxSteeringForce(5.0f),
	m_seekStrength(2.0f),

	m_fleeWeighting(0.0f),
	m_fleeMaxSteeringForce(5.0f),
	m_fleeStrength(2.0f),

	m_wanderWeighting(0.2f),
	m_wanderMaxSteeringForce(5.0f),
	m_wanderStrength(2.0f),
	m_wanderRadius(15.0f),
	m_wanderDistance(35.0f),
	m_wanderAngle(0.0f),
	m_wanderAngleRandomStrength(0.3f),
	m_wanderAdjustmentTimer(0.0f),
	m_targetWanderAngle(0.0f),

	m_flockingWeighting(0.0f),

	m_cohesionWeighting(0.2f),
	m_cohesionMaxSteeringForce(5.0f),
	m_cohesionStrength(2.0f),
	m_cohesionNeighbourhoodRadius(100.0f),
	m_cohesionIncludesSelf(false),

	m_separationWeighting(0.5f),
	m_separationMaxSteeringForce(3.0f),
	m_separationStrength(2.0f),
	m_separationNeighbourhoodRadius(30.0f),

	m_alignmentWeighting(0.2f),
	m_alignmentMaxSteeringForce(5.0f),
	m_alignmentStrength(2.0f),
	m_alignmentNeighbourhoodRadius(50.0f),

	m_pursuitWeighting(0.0f),
	m_pursuitMaxSteeringForce(5.0f),
	m_pursuitStrength(2.0f),
	m_pursuitTarget(nullptr),

	m_evasionWeighting(0.0f),
	m_evasionMaxSteeringForce(5.0f),
	m_evasionStrength(2.0f),
	m_evasionTarget(nullptr)

{
	this->setPosition(spawnPos);    // Set the agnets position to spawn position
	m_target.setPosition(spawnPos); // set for now, gets overridden in update
	setupShape();                   // Setup the shape of the agent
}

// **=== Public Methods ===**

void Agent::update(float deltaTime, const sf::RenderWindow& window, const std::vector<Agent*>& allAgents)
{
	// -- Update Agent Behaviour --
	seek(deltaTime);
	flee(deltaTime);
	wander(deltaTime);

	separate(deltaTime, allAgents);
	cohesion(deltaTime, allAgents);
	alignment(deltaTime, allAgents);

	pursuit(deltaTime, allAgents);
	evasion(deltaTime, allAgents);

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
void Agent::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform(); // Apply the agnets entire current transform
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

// **=== Behaviours ===**

void Agent::applySteeringFromDesiredVelocity(const sf::Vector2f& desiredVelocity, float maxSteeringForce, float strength, float weighting, float deltaTime)
{
	// Update Steering Force
	sf::Vector2f steerForce = desiredVelocity - m_velocity;
	// Apply the steering force
	applySteeringFromForce(steerForce, maxSteeringForce, strength, weighting, deltaTime);
}
void Agent::applySteeringFromForce(sf::Vector2f force, float maxSteeringForce, float strength, float weighting, float deltaTime)
{
	// Truncate the force to the max steering force
	force = Utils::truncate(force, maxSteeringForce);
	// Update Velocity
	m_velocity += force * strength * weighting * deltaTime;
	m_velocity = Utils::truncate(m_velocity, m_maxSpeed);
}
void Agent::seek(float deltaTime)
{
	// If behaviour has no weight dont do calculations
	if (m_seekWeighting <= 0.0f)
	{
		m_seekDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	// Calc desired velocity seek(target position - current position) 
	m_seekDesiredVelocity = Utils::normalised(m_target.getPosition() - this->getPosition()) * m_maxSpeed;
	// Update Steering Force
	applySteeringFromDesiredVelocity(m_seekDesiredVelocity, m_seekMaxSteeringForce, m_seekStrength, m_seekWeighting, deltaTime);
}
void Agent::flee(float deltaTime) 
{
	// If behaviour has no weight dont do calculations
	if (m_fleeWeighting <= 0.0f)
	{
		m_fleeDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	// Calc desired velocity flee(target position - current position) 
	m_fleeDesiredVelocity = Utils::normalised(this->getPosition() - m_target.getPosition()) * m_maxSpeed;
	// Update Steering Force
	applySteeringFromDesiredVelocity(m_fleeDesiredVelocity, m_fleeMaxSteeringForce, m_fleeStrength, m_fleeWeighting, deltaTime);
}
void Agent::wander(float deltaTime)
{
	// If behaviour has no weight dont do calculations
	if (m_wanderWeighting <= 0.0f)
	{
		m_wanderDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

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

	sf::Vector2f wanderSteerForce = m_wanderDesiredVelocity - m_velocity; 
	applySteeringFromForce(wanderSteerForce, m_wanderMaxSteeringForce, m_wanderStrength, m_wanderWeighting, deltaTime);
}
void Agent::separate(float deltaTime, const std::vector<Agent*>& allAgents)
{
	// If behaviour has no weight dont do calculations
	if (m_separationWeighting <= 0.0f)
	{
		m_separationDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	sf::Vector2f selfPos = this->getPosition(); // Save own position
	sf::Vector2f diffAverage(0.0f, 0.0f); // Average difference vector
	int count = 0; // Count of neighbours

	for (const auto& agent : allAgents)
	{
		if (agent != this) // Dont separate from self
		{
			sf::Vector2f otherAgentPos = agent->getPosition(); // Get other agents position
			sf::Vector2f difference = selfPos - otherAgentPos; // Calculate vec between agents
			float distance = Utils::magnitude(difference);     // Calculate distance between agents

			if (distance < getSeparationNeighbourhoodRadius()) // Check if distance is within neighbourhood radius
			{
				sf::Vector2f normDifference = Utils::normalised(difference); // It was at this very point i realised sf::Vector2f has its own norm, length and other functions :(
				normDifference = normDifference / distance;
				diffAverage += normDifference; // Add to average
				count++;
			}
		}
	}

	if (count > 0) // If there are neighbours
	{
		diffAverage /= static_cast<float>(count);                                  // Average the difference vector
		m_separationDesiredVelocity = Utils::normalised(diffAverage) * m_maxSpeed; // Normalise and scale to max speed
		applySteeringFromDesiredVelocity(m_separationDesiredVelocity, m_separationMaxSteeringForce, m_separationStrength, m_separationWeighting, deltaTime);
	}
}
void Agent::cohesion(float deltaTime, const std::vector<Agent*>& allAgents)
{
	// TODO: This behaviour is done way better, refactor the other behaviours like this
	
	// If behavior has no weight dont do calculations
	if (m_cohesionWeighting <= 0.0f) 
	{
		m_cohesionDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	sf::Vector2f positionSum(0.0f, 0.0f);
	int neighborsFound = 0;
	sf::Vector2f selfPos = this->getPosition();

	for (const auto& otherAgent : allAgents) 
	{
		if (!m_cohesionIncludesSelf && otherAgent == this) 
		{
			continue; // Skip if not including self
		}

		sf::Vector2f otherPos = otherAgent->getPosition();
		float distance = Utils::magnitude(otherPos - selfPos);

		if (distance < m_cohesionNeighbourhoodRadius)
		{
			positionSum += otherPos; // Add position to sum
			neighborsFound++;
		}
	}

	if (neighborsFound > 0) 
	{
		m_cohesionCenterOfMass = positionSum / static_cast<float>(neighborsFound);                    // Average position of neighbors
		m_cohesionDesiredVelocity = Utils::normalised(m_cohesionCenterOfMass - selfPos) * m_maxSpeed; // Desired velocity towards the center of mass
		applySteeringFromDesiredVelocity(m_cohesionDesiredVelocity, m_cohesionMaxSteeringForce, m_cohesionStrength, m_cohesionWeighting, deltaTime);
	}
}
void Agent::alignment(float deltaTime, const std::vector<Agent*>& allAgents)
{
	sf::Vector2f selfPos = this->getPosition(); // Save own position
	sf::Vector2f velocityAverage(0.0f, 0.0f);   // Average velocity vector
	int count = 0; // Count of neighbours

	// If behaviour has no weight dont do calculations
	if (m_alignmentWeighting <= 0.0f)
	{
		m_alignmentDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	for (const auto& otherAgent : allAgents)
	{
		if (otherAgent == this) continue; // Skip self (wow, can call in one line)

		sf::Vector2f otherVelocity = otherAgent->getVelocity();
		sf::Vector2f otherPos = otherAgent->getPosition();
		float distance = Utils::magnitude(otherPos - selfPos); // Calculate distance between agents

		if (distance < m_alignmentNeighbourhoodRadius) // Check if distance is within neighbourhood radius
		{
			velocityAverage += otherVelocity; // Add to average
			count++;
		}
	}

	if (count > 0) // If there are neighbours
	{
		velocityAverage /= static_cast<float>(count); // Average the velocity vector
		m_alignmentDesiredVelocity = Utils::normalised(velocityAverage) * m_maxSpeed; // Normalise and scale to max speed
		applySteeringFromDesiredVelocity(m_alignmentDesiredVelocity, m_alignmentMaxSteeringForce, m_alignmentStrength, m_alignmentWeighting, deltaTime);
	}
}
void Agent::pursuit(float deltaTime, const std::vector<Agent*>& allAgents)
{
	// Set target to the first agent in the list
	if (m_pursuitTarget == nullptr && !allAgents.empty())
	{
		if (allAgents[0] != this) // Dont set target for self
		{
			m_pursuitTarget = allAgents[0]; // Set target to the first agent in the list
		}
	}

	// If behaviour has no weight or no target dont do calculations
	if (m_pursuitWeighting <= 0.0f && m_evasionWeighting <= 0.0f) // Need to check all targeted behaviour weight to not override
	{
		allAgents[0]->setColor(sf::Color(50, 50, 50)); // Set target color to default
		return;
	}
	if (m_pursuitWeighting <= 0.0f || m_pursuitTarget == nullptr)
	{
		m_pursuitDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}

	m_pursuitTarget->setColor(sf::Color::Red); // Set target color to red for visualization

	sf::Vector2f targetPos = m_pursuitTarget->getPosition(); // Get target position
	sf::Vector2f selfPos = this->getPosition();              // Save own position

	float predictionTime = Utils::magnitude(targetPos - selfPos) / m_maxSpeed;                     // Calculate time to reach target
	sf::Vector2f predictedTargetPos = targetPos + m_pursuitTarget->getVelocity() * predictionTime; // Predict target position

	// Calculate desired velocity towards predicted target position
	m_pursuitDesiredVelocity = Utils::normalised(predictedTargetPos - selfPos) * m_maxSpeed; // Normalise and scale to max speed
	// Update Steering Force
	applySteeringFromDesiredVelocity(m_pursuitDesiredVelocity, m_pursuitMaxSteeringForce, m_pursuitStrength, m_pursuitWeighting, deltaTime);
}
void Agent::evasion(float deltaTime, const std::vector<Agent*>& allAgents)
{ // TODO: Maybe add radius check so agents only evade when target close
	// Set target to the first agent in the list
	if (m_evasionTarget == nullptr && !allAgents.empty())
	{
		if (allAgents[0] != this) // Dont set target for self
		{
			m_evasionTarget = allAgents[0]; // Set target to the first agent in the list
		}
	}
	// If behaviour has no weight or no target dont do calculations
	if (m_evasionWeighting <= 0.0f && m_pursuitWeighting <= 0.0f)
	{
		allAgents[0]->setColor(sf::Color(50, 50, 50)); // Set target color to default
		return;
	}
	if (m_evasionWeighting <= 0.0f || m_evasionTarget == nullptr)
	{
		m_evasionDesiredVelocity = sf::Vector2f(0.f, 0.f);
		return;
	}
	m_evasionTarget->setColor(sf::Color::Red); // Set target color to red for visualization

	sf::Vector2f targetPos = m_evasionTarget->getPosition(); // Get target position
	sf::Vector2f selfPos = this->getPosition();              // Save own position

	float predictionTime = Utils::magnitude(targetPos - selfPos) / m_maxSpeed;                     // Calculate time to reach target
	sf::Vector2f predictedTargetPos = targetPos + m_evasionTarget->getVelocity() * predictionTime; // Predict target position

	// Calculate desired velocity towards predicted target position
	m_evasionDesiredVelocity = Utils::normalised(selfPos - predictedTargetPos) * m_maxSpeed; // Normalise and scale to max speed
	// Update Steering Force
	applySteeringFromDesiredVelocity(m_evasionDesiredVelocity, m_evasionMaxSteeringForce, m_evasionStrength, m_evasionWeighting, deltaTime);
}

// **=== Visualizations ===**

void Agent::drawVisualizations(sf::RenderTarget& target, const std::vector<Agent*>& allAgents) const
{
	// Draw visuals
	//drawVelocityLine(target);
	drawBehaviourVisuals(target, allAgents);
}
void Agent::drawVelocityLine(sf::RenderTarget& target) const
{
	float length = 3.0f; // Length of the line

	// Create a VertexArray for the line
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);

	// Set the starting point of the line (agents position + a bit)
	line[0].position = this->getPosition() + m_velocity * 0.2f;
	line[0].color = sf::Color::Red;

	// Set the end point of the line (position + velocity)
	sf::Vector2f endPoint = this->getPosition() + m_velocity * length;
	line[1].position = endPoint;
	line[1].color = sf::Color::Red;

	// Draw the line
	target.draw(line);
}
void Agent::drawBehaviourVisuals(sf::RenderTarget& target, const std::vector<Agent*>& allAgents) const
{
	// -- Seek Desired Velocity Line --
	if (m_seekWeighting > 0.0f && m_behaviour == Behaviour::SEEK)
	{
		float length = 1.0f; // Length of the line
		// Create a VertexArray for the line
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		// Set the starting point of the line (agnets position + a bit)
		line[0].position = this->getPosition() + m_velocity * 0.2f;
		line[0].color = sf::Color::Red;
		// Set the end point of the line (position + velocity)
		sf::Vector2f endPoint = this->getPosition() + m_seekDesiredVelocity * length;
		line[1].position = endPoint;
		line[1].color = sf::Color::Red;
		// Draw the line
		target.draw(line);
	}
	// -- Flee Desired Velocity Line --
	if (m_fleeWeighting > 0.0f && m_behaviour == Behaviour::FLEE)
	{
		float length = 1.0f;
		sf::VertexArray fleeLine(sf::PrimitiveType::Lines, 2);
		fleeLine[0].position = this->getPosition() + m_velocity * 0.2f;
		fleeLine[0].color = sf::Color::Blue;
		sf::Vector2f fleeEndPoint = this->getPosition() + m_fleeDesiredVelocity * length;
		fleeLine[1].position = fleeEndPoint;
		fleeLine[1].color = sf::Color::Blue;
		target.draw(fleeLine);
	}
	// -- Wander Widget --
	if (m_wanderWeighting > 0.0f && m_behaviour == Behaviour::WANDER)
	{
		// - Wander Circle -
		sf::CircleShape circle(m_wanderRadius);
		sf::Vector2f circlePos = this->getPosition() + Utils::normalised(m_velocity) * m_wanderDistance;
		circle.setFillColor(sf::Color(20, 100, 20, 20));
		circle.setPosition(circlePos);
		circle.setOrigin({ m_wanderRadius, m_wanderRadius });
		circle.setOutlineColor(sf::Color(20, 100, 20, 60));
		circle.setOutlineThickness(1.0f);
		target.draw(circle);

		// - Wander Angle -
		sf::VertexArray angleLine(sf::PrimitiveType::Lines, 2);
		angleLine[0].position = circlePos;
		angleLine[0].color = sf::Color::Green;
		// Calculate the end point of the angle line
		sf::Vector2f displacement_on_circle(
			std::cos(m_wanderAngle) * m_wanderRadius,
			std::sin(m_wanderAngle) * m_wanderRadius
		);
		sf::Vector2f angleEndPoint = circlePos + displacement_on_circle;
		angleLine[1].position = angleEndPoint;
		angleLine[1].color = sf::Color::Green;
		target.draw(angleLine);

		// - Turn Range -
		// Max turn line
		sf::VertexArray maxTurnLine(sf::PrimitiveType::Lines, 2);
		maxTurnLine[0].position = circlePos;
		maxTurnLine[0].color = sf::Color(20, 20, 100, 60);
		maxTurnLine[1].position = circlePos + sf::Vector2f(std::cos(m_wanderAngle - m_wanderAngleRandomStrength) * m_wanderRadius, std::sin(m_wanderAngle - m_wanderAngleRandomStrength) * m_wanderRadius);
		maxTurnLine[1].color = sf::Color(20, 20, 100, 60);
		target.draw(maxTurnLine);

		// Min turn line
		sf::VertexArray minTurnLine(sf::PrimitiveType::Lines, 2);
		minTurnLine[0].position = circlePos;
		minTurnLine[0].color = sf::Color(20, 20, 100, 60);
		minTurnLine[1].position = circlePos + sf::Vector2f(std::cos(m_wanderAngle + m_wanderAngleRandomStrength) * m_wanderRadius, std::sin(m_wanderAngle + m_wanderAngleRandomStrength) * m_wanderRadius);
		minTurnLine[1].color = sf::Color(20, 20, 100, 60);
		target.draw(minTurnLine);
	}
	// -- Cohesion Widget --
	if (m_cohesionWeighting > 0.0f && m_behaviour == Behaviour::FLOCKING)
	{
		// - Cohesion Lines towards center of mass -
		for (const auto& agent : allAgents)
		{
			if (agent == this)
			{
				continue;
			}
			// Draw the line
			sf::VertexArray line(sf::PrimitiveType::Lines, 2);
			line[0].position = this->getPosition();
			line[0].color = sf::Color::Green;
			line[1].position = m_cohesionCenterOfMass;
			line[1].color = sf::Color::Green;
			// if line too long, truncate
			if (Utils::magnitude(line[1].position - line[0].position) > 40.0f)
			{
				line[1].position = line[0].position + Utils::normalised(line[1].position - line[0].position) * 40.0f;
			}
			target.draw(line);
		}

		// - center of mass circles -
		sf::CircleShape circle(5);
		circle.setFillColor(sf::Color(20, 100, 20, 20));
		circle.setPosition(m_cohesionCenterOfMass);
		circle.setOrigin({ circle.getRadius(), circle.getRadius() });
		target.draw(circle);
		// Draw the center of mass
		sf::CircleShape centerOfMassCircle(5.0f);
	}
	// -- Alignment Widget --
	if (m_alignmentWeighting > 0.0f && m_behaviour == Behaviour::FLOCKING)
	{
		// - 'Aligning to' line -
		for (const auto& agent : allAgents)
		{
			if (agent == this)
			{
				continue; // Skip self
			}
			sf::Vector2f selfPos = this->getPosition();                 // Save own position
			sf::Vector2f otherAgentPos = agent->getPosition();          // Get other agnets position
			float distance = Utils::magnitude(selfPos - otherAgentPos); // Calculate distance positions
			if (distance < m_alignmentNeighbourhoodRadius)
			{
				// Draw the line
				sf::VertexArray line(sf::PrimitiveType::Lines, 2);
				line[0].position = this->getPosition();
				line[0].color = sf::Color(255, 165, 0);
				line[1].position = otherAgentPos;
				line[1].color = sf::Color(255, 165, 0);
				target.draw(line);
			}
		}
	}
	// -- Separation Widget --
	if (m_separationWeighting > 0.0f && m_behaviour == Behaviour::FLOCKING)
	{
		// - Separation Lines -
		for (const auto& agent : allAgents)
		{
			if (agent == this)
			{
				continue; // Skip self
			}
			sf::Vector2f selfPos = this->getPosition();                 // Save own position
			sf::Vector2f otherAgentPos = agent->getPosition();          // Get other agnets position
			float distance = Utils::magnitude(selfPos - otherAgentPos); // Calculate distance positions
			if (distance < m_separationNeighbourhoodRadius)
			{
				// Draw the line
				sf::VertexArray line(sf::PrimitiveType::Lines, 2);
				line[0].position = this->getPosition();
				line[0].color = sf::Color::Magenta;
				line[1].position = otherAgentPos;
				line[1].color = sf::Color::Magenta;
				target.draw(line);
			}
		}
	}
	// -- Pursuit Widget --
	if (m_pursuitWeighting > 0.0f && m_behaviour == Behaviour::PURSUIT)
	{
		// -- Line to Predicted Target --
		float length = 1.0f;
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		line[0].position = this->getPosition() + m_velocity * 0.2f;
		line[0].color = sf::Color::Red;
		sf::Vector2f endPoint = this->getPosition() + m_pursuitDesiredVelocity * length;
		line[1].position = endPoint;
		line[1].color = sf::Color::Red;
		target.draw(line);
	}
	// -- Evasion Widget --
	if (m_evasionWeighting > 0.0f && m_behaviour == Behaviour::EVASION)
	{
		// -- Line to Predicted Target --
		float length = 1.0f;
		sf::VertexArray line(sf::PrimitiveType::Lines, 2);
		line[0].position = this->getPosition() + m_velocity * 0.2f;
		line[0].color = sf::Color::Blue;
		sf::Vector2f endPoint = this->getPosition() + m_evasionDesiredVelocity * length;
		line[1].position = endPoint;
		line[1].color = sf::Color::Blue;
		target.draw(line);
	}
}


