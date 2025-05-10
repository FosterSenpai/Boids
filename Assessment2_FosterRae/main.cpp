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
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Foster's Boids");
    window.setFramerateLimit(60);

    // **=== Agent Creation ===**
	std::vector<Agent*> agents;
	for (int i = 0; i < 100; ++i)
	{
		sf::Vector2f spawnPos(static_cast<float>(rand() % window.getSize().x), static_cast<float>(rand() % window.getSize().y));
		agents.push_back(new Agent(spawnPos));
	}

    // **=== UI Setup ===**

	sf::Font font;
	if (!font.openFromFile("PixelDigivolveItalic-dV8R.ttf")) {
		std::cerr << "Error: Failed to load font for Sliders" << std::endl;
		return EXIT_FAILURE;
	}

	sf::Vector2f sliderSize(200.0f, 10.0f);

	Slider speedSlider({ 10, 20 }, sliderSize, 0.0f, 100.0f, agents[0]->getSpeed() , &window, "Speed: ", font);

	Slider seekWeightingSlider({ 10, 200 }, sliderSize, 0.0f, 1.0f, agents[0]->getSeekWeighting(), &window, "Seek Weighting: ", font);
	Slider seekStrengthSlider({ 10, 80 } , sliderSize, 0.0f, 100.0f, agents[0]->getSeekStrength(), &window, "Seek Strength: ", font);

	Slider fleeWeightingSlider({ 10, 230 }, sliderSize, 0.0f, 1.0f, agents[0]->getFleeWeighting(), &window, "Flee Weighting: ", font);
	Slider fleeStrengthSlider({ 10, 110 }, sliderSize, 0.0f, 100.0f, agents[0]->getFleeStrength(), &window, "Flee Strength: ", font);

	Slider wanderWeightingSlider({ 10, 260 }, sliderSize, 0.0f, 1.0f, agents[0]->getWanderWeighting(), &window, "Wander Weighting: ", font);
	Slider wanderStrengthSlider({ 10, 140 }, sliderSize, 0.0f, 100.0f, agents[0]->getWanderStrength(), &window, "Wander Strength: ", font);
	Slider wanderAngleRandomStrengthSlider({ 10, 320 }, sliderSize, 0.0f, 1.0f, agents[0]->getWanderAngleRandomStrength(), &window, "Wander Angle Range: ", font);
	
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
				/* Removed, behaviour now weighted
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
				*/
            }

            // **=== UI Interaction ===**

			// -- Slider Interaction --
            if (event) {
				speedSlider.handleEvent(*event);

				seekWeightingSlider.handleEvent(*event);
				seekStrengthSlider.handleEvent(*event);

				fleeWeightingSlider.handleEvent(*event);
				fleeStrengthSlider.handleEvent(*event);

				wanderWeightingSlider.handleEvent(*event);
				wanderStrengthSlider.handleEvent(*event);
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

		float currentSeekWeighting = seekWeightingSlider.getValue();
		float currentSeekStrength = seekStrengthSlider.getValue();

		float currentFleeWeighting = fleeWeightingSlider.getValue();
		float currentFleeStrength = fleeStrengthSlider.getValue();

		float currentWanderWeighting = wanderWeightingSlider.getValue();
		float currentWanderStrength = wanderStrengthSlider.getValue();
		float currentWanderAngleRandomStrength = wanderAngleRandomStrengthSlider.getValue();

        // Update agents values
        for (auto& agent : agents)
        {
            agent->setTargetPosition(currentMouseTarget);
			agent->setSpeed(currentSpeed);

			agent->setSeekWeighting(currentSeekWeighting);
			agent->setSeekStrength(currentSeekStrength);

			agent->setFleeWeighting(currentFleeWeighting);
			agent->setFleeStrength(currentFleeStrength);

			agent->setWanderWeighting(currentWanderWeighting);
			agent->setWanderStrength(currentWanderStrength);
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
		window.draw(speedSlider);

		window.draw(seekWeightingSlider);
		window.draw(seekStrengthSlider);

		window.draw(fleeWeightingSlider);
		window.draw(fleeStrengthSlider);

		window.draw(wanderWeightingSlider);
		window.draw(wanderStrengthSlider);
		window.draw(wanderAngleRandomStrengthSlider);

        window.display();
    }
}