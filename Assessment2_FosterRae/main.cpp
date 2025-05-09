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
	for (int i = 0; i < 10; ++i)
	{
		sf::Vector2f spawnPos(static_cast<float>(rand() % window.getSize().x), static_cast<float>(rand() % window.getSize().y));
		agents.push_back(new Agent(spawnPos));
	}

    // **=== UI Setup ===**

	Slider speedSlider({ 10, 10 }, { 200, 20 }, 0.0f, 50.0f, agents[0]->getSpeed() , &window, "Speed: ");
	Slider seekStrengthSlider({ 10, 55 }, { 200, 20 }, 0.0f, 1.0f, agents[0]->getSeekStrength(), &window, "Seek Strength: ");
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
                if (keyPressed->scancode == sf::Keyboard::Scancode::V)
					showVisualizations = !showVisualizations; // Toggle visualizations
            }

            // **=== UI Interaction ===**

			// -- Slider Interaction --
            if (event) {
				speedSlider.handleEvent(*event);
				seekStrengthSlider.handleEvent(*event);
            }

            // **=== Mouse Input ===**
        }
        // **=== Update Agents ===**
        
		// -- Update Agents from Sliders --
        // Get slider values
		float currentSpeed = speedSlider.getValue();
		float currentSeekStrength = seekStrengthSlider.getValue();

        // Update agents values
        for (auto& agent : agents)
        {
			agent->setSpeed(currentSpeed);
			agent->setSeekStrength(currentSeekStrength);
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
        window.draw(seekStrengthSlider);

        window.display();
    }
}