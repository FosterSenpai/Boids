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

	Slider speedSlider({ 10, 10 }, { 200, 20 }, 0.0f, 100.0f, agents[0]->getSpeed() , &window, "Speed: ");
    Slider maxSeekSteeringSlider({ 10, 55 }, { 200, 20 }, 0.0f, 10.0f, agents[0]->getSeekMaxSteeringForce(), &window, "Max Seek Steering Force: ");
	Slider seekStrengthSlider({ 10, 100 } , { 200, 20 }, 0.0f, 1.0f, agents[0]->getSeekStrength(), &window, "Seek Strength: ");
	Slider maxFleeSteeringSlider({ 10, 55 }, { 200, 20 }, 0.0f, 10.0f, agents[0]->getFleeMaxSteeringForce(), &window, "Max Flee Steering Force: ");
	Slider fleeStrengthSlider({ 10, 100 }, { 200, 20 }, 0.0f, 1.0f, agents[0]->getFleeStrength(), &window, "Flee Strength: ");
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
            }

            // **=== UI Interaction ===**

			// -- Slider Interaction --
            if (event) {
				speedSlider.handleEvent(*event);
				seekStrengthSlider.handleEvent(*event);
                maxSeekSteeringSlider.handleEvent(*event);
				maxFleeSteeringSlider.handleEvent(*event);
				fleeStrengthSlider.handleEvent(*event);
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
		float currentMaxSteeringForce = maxSeekSteeringSlider.getValue();
		float currentFleeMaxSteeringForce = maxFleeSteeringSlider.getValue();
		float currentFleeStrength = fleeStrengthSlider.getValue();

        // Update agents values
        for (auto& agent : agents)
        {
            agent->setTargetPosition(currentMouseTarget);
			agent->setSpeed(currentSpeed);
			agent->setSeekStrength(currentSeekStrength);
			agent->setSeekMaxSteeringForce(currentMaxSteeringForce);
			agent->setFleeMaxSteeringForce(currentFleeMaxSteeringForce);
			agent->setFleeStrength(currentFleeStrength);
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
		case MovementType::SEEK:
			speedSlider.setLabel("Seek Speed: ");
            window.draw(speedSlider);
            window.draw(seekStrengthSlider);
            window.draw(maxSeekSteeringSlider);
			break;
		case MovementType::FLEE:
			speedSlider.setLabel("Flee Speed: ");
            window.draw(speedSlider);
			window.draw(fleeStrengthSlider);
			window.draw(maxFleeSteeringSlider);
			break;
		default:
			break;
		}


        window.display();
    }
}