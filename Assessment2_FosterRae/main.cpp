// /***********************************************************************
// Bachelor of Software Engineering
// Media Design School
// Blenheim
// New Zealand
// (c) 2025 Media Design School
// File Name : main.cpp
// Description : Entry point for the Steering Behaviours Test application.
//               Creates the SFML window and manages the main game loop,
//               demonstrating the Seek behaviour with multiple agents.
// Author : Foster Rae
// Mail : foster.rae@mds.ac.nz
// ************************************************************************/

// **== Includes ==**
#include "Slider.h"
#include <SFML/Graphics.hpp>
#include "Agent.h"

int main()
{
    // **=== Window Setup ===**
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Steering Behaviours Test");
    window.setFramerateLimit(60);

    // **=== Agent Creation ===**
	std::vector<Agent*> agents;
	for (int i = 0; i < 30; ++i)
	{
		sf::Vector2f spawnPos(static_cast<float>(rand() % window.getSize().x), static_cast<float>(rand() % window.getSize().y));
		agents.push_back(new Agent(spawnPos));
	}

    // **=== UI Setup ===**

	Slider speedSlider({ 10, 20 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getSpeed() , &window, "Speed: ");

    Slider seekMaxSteeringSlider({ 10, 65}, { 200, 20 }, 0.0f, 10.0f, agents[0]->getSeekMaxSteeringForce(), &window, "Max Seek Steering Force: ");
	Slider seekStrengthSlider({ 10, 110 } , { 200, 20 }, 0.0f, 100.0f, agents[0]->getSeekStrength(), &window, "Seek Strength: ");

	Slider fleeMaxSteeringSlider({ 10, 65 }, { 200, 20 }, 0.0f, 10.0f, agents[0]->getFleeMaxSteeringForce(), &window, "Max Flee Steering Force: ");
	Slider fleeStrengthSlider({ 10, 110 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getFleeStrength(), &window, "Flee Strength: ");

	Slider wanderMaxSteeringSlider({ 10, 65 }, { 200, 20 }, 0.0f, 10.0f, agents[0]->getWanderMaxSteeringForce(), &window, "Max Wander Steering Force: ");
	Slider wanderStrengthSlider({ 10, 110 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getWanderStrength(), &window, "Wander Strength: ");
	Slider wanderDistanceSlider({ 10, 155 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getWanderDistance(), &window, "Wander Distance: ");
	Slider wanderRadiusSlider({ 10, 200 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getWanderRadius(), &window, "Wander Radius: ");
	Slider wanderAngleRandomStrengthSlider({ 10, 245 }, { 200, 20 }, 0.0f, 1.0f, agents[0]->getWanderAngleRandomStrength(), &window, "Wander Angle Random Strength: ");
	
	bool showVisualizations = false; // Flag to toggle visualizations


    // **=== Game Loop ===**
    sf::Clock deltaClock;

    while (window.isOpen())
    {
		// Calculate delta time
        sf::Time deltaTime = deltaClock.restart();
        float dtSeconds = deltaTime.asSeconds();

        // **=== Event Handling ===**
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::V) {
                    showVisualizations = !showVisualizations; // Toggle visualizations
                }

				// -- Change Behaviour --
				if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) { // 1 = Seek
                    for (auto& agent : agents) {
                        agent->setMovementType(MovementType::SEEK);
                    }
                }
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) { // 2 = Flee
					for (auto& agent : agents) {
						agent->setMovementType(MovementType::FLEE);
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) { // 3 = Wander
					for (auto& agent : agents) {
						agent->setMovementType(MovementType::WANDER);
					}
				}
            }

            // **=== UI Interaction ===**

			// -- Slider Interaction --
            if (event) {
				speedSlider.handleEvent(*event);
				// Seek
				seekStrengthSlider.handleEvent(*event);
                seekMaxSteeringSlider.handleEvent(*event);
				// Flee
				fleeMaxSteeringSlider.handleEvent(*event);
				fleeStrengthSlider.handleEvent(*event);
				// Wander
				wanderStrengthSlider.handleEvent(*event);
				wanderMaxSteeringSlider.handleEvent(*event);
				wanderDistanceSlider.handleEvent(*event);
				wanderRadiusSlider.handleEvent(*event);
				wanderAngleRandomStrengthSlider.handleEvent(*event);
            }

        }

        // **===  Global Inputs  ===**
        sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f currentMouseTarget(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));


        // **=== Update Agents ===**
        
		// -- Update Agents from Sliders --
        // Get slider values
		float currentSpeed = speedSlider.getValue();
		float currentSeekStrength = seekStrengthSlider.getValue();
		float currentMaxSteeringForce = seekMaxSteeringSlider.getValue();
		float currentFleeMaxSteeringForce = fleeMaxSteeringSlider.getValue();
		float currentFleeStrength = fleeStrengthSlider.getValue();
		float currentWanderStrength = wanderStrengthSlider.getValue();
		float currentWanderMaxSteeringForce = wanderMaxSteeringSlider.getValue();
		float currentWanderDistance = wanderDistanceSlider.getValue();
		float currentWanderRadius = wanderRadiusSlider.getValue();
		float currentWanderAngleRandomStrength = wanderAngleRandomStrengthSlider.getValue();

        // Update agents values
        for (auto& agent : agents)
        {
            agent->setTargetPosition(currentMouseTarget);
			agent->setSpeed(currentSpeed);
			agent->setSeekStrength(currentSeekStrength);
			agent->setSeekMaxSteeringForce(currentMaxSteeringForce);
			agent->setFleeMaxSteeringForce(currentFleeMaxSteeringForce);
			agent->setFleeStrength(currentFleeStrength);
			agent->setWanderStrength(currentWanderStrength);
			agent->setWanderMaxSteeringForce(currentWanderMaxSteeringForce);
			agent->setWanderDistance(currentWanderDistance);
			agent->setWanderRadius(currentWanderRadius);
			agent->setWanderAngleRandomStrength(currentWanderAngleRandomStrength * 3);
        }

        // **=== Rendering ===**
        window.clear(sf::Color::White);

		// Draw all agents
		for (Agent* agent : agents)
		{
			agent->update(dtSeconds, window, agents);
			window.draw(*agent);
		}

        if (showVisualizations)
        {
			// Draw the target position for each agent
			for (const Agent* agent : agents)
			{
                agent->drawVisualizations(window); // Draw the agent's visualizations
			}
        }

        // Draw Sliders
		switch (agents[0]->getMovementType())
		{
		case MovementType::NONE:
			break;
		case MovementType::SEEK:
			speedSlider.setLabel("Seek Speed: ");
			window.draw(speedSlider);
			window.draw(seekStrengthSlider);
			window.draw(seekMaxSteeringSlider);
			break;
		case MovementType::FLEE:
			speedSlider.setLabel("Flee Speed: ");
			window.draw(speedSlider);
			window.draw(fleeStrengthSlider);
			window.draw(fleeMaxSteeringSlider);
			break;
		case MovementType::PURSUE:
			break;
		case MovementType::ARRIVAL:
			break;
		case MovementType::WANDER:
			speedSlider.setLabel("Wander Speed: ");
			window.draw(speedSlider);
			window.draw(wanderStrengthSlider);
			window.draw(wanderMaxSteeringSlider);
			window.draw(wanderDistanceSlider);
			window.draw(wanderRadiusSlider);
			window.draw(wanderAngleRandomStrengthSlider);
			break;
		default:
			break;
		}


        window.display();
    }
}