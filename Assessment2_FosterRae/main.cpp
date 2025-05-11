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
#include <memory>
#include <vector>

int main()
{
    // **=== Window Setup ===**
    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Foster's Boids");
    window.setFramerateLimit(60);

    // **=== Agent Creation ===**
	std::vector<Agent*> agents;
	for (int i = 0; i < 200; ++i)
	{
		sf::Vector2f spawnPos(static_cast<float>(rand() % window.getSize().x), static_cast<float>(rand() % window.getSize().y));
		agents.push_back(new Agent(spawnPos));
	}

	if (agents.empty()) {
		std::cerr << "Error: No agents created, cannot initialize sliders." << std::endl;
		for (Agent* agent : agents) { delete agent; } // Basic cleanup
		return EXIT_FAILURE;
	}
	Agent* firstAgent = agents[0];

	// **=== UI Setup ===**

	// Load font
	sf::Font font;
	if (!font.openFromFile("PixelDigivolveItalic-dV8R.ttf")) {
		std::cerr << "Error: Failed to load font for Sliders" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<std::unique_ptr<Slider>> sliders;  // Vector to hold sliders
	sf::Vector2f sliderSize(200.0f, 10.0f);        // Size of the slider track
	float sliderStartY = 20.0f;                    // Starting Y position for the first slider
	float sliderSpacingY = 35.0f;                  // Space from the start of one slider to the start of the next
	float sectionSpacing = 15.0f;                  // Extra space between sections

	// Helper to make adding sliders cleaner
	float sliderStartX = 10.0f;
	auto addSlider = [&](float minVal, float maxVal, float initVal, const std::string& label) {
		sliders.push_back(std::make_unique<Slider>(sf::Vector2f(sliderStartX, sliderStartY), sliderSize, minVal, maxVal, initVal, &window, label, font));
		sliderStartY += sliderSpacingY; // Advance Y
	};

	// --- Speed Slider ---
	addSlider(0.0f, 100.0f, firstAgent->getSpeed(), "Speed: ");

	sliderStartY += sectionSpacing;

	// --- Seek Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getSeekWeighting(), "Seek Weighting: ");
	addSlider(0.0f, 100.0f, firstAgent->getSeekStrength(), "Seek Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeekMaxSteeringForce(), "Seek Max Force: ");

	sliderStartY += sectionSpacing;

	// --- Flee Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getFleeWeighting(), "Flee Weighting: ");
	addSlider(0.0f, 100.0f, firstAgent->getFleeStrength(), "Flee Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getFleeMaxSteeringForce(), "Flee Max Force: ");

	sliderStartY += sectionSpacing;

	// --- Wander Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getWanderWeighting(), "Wander Weighting: ");
	addSlider(0.0f, 100.0f, firstAgent->getWanderStrength(), "Wander Strength: ");
	addSlider(0.0f, 1.0f, firstAgent->getWanderAngleRandomStrength(), "Wander Angle Range: ");
	addSlider(0.0f, 10.0f, firstAgent->getWanderMaxSteeringForce(), "Wander Max Force: ");

	sliderStartY += sectionSpacing;

	// --- Separation Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getSeparationWeighting(), "Separation Weighting: ");
	addSlider(0.0f, 100.0f, firstAgent->getSeparationStrength(), "Separation Strength: ");
	addSlider(0.0f, 100.0f, firstAgent->getSeparationNeighbourhoodRadius(), "Separation Radius: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeparationMaxSteeringForce(), "Separation Max Force: ");


	bool showVisualizations = false;

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
				for (auto& slider_ptr : sliders) { // Iterate through all sliders in the vector
					slider_ptr->handleEvent(*event);
				}

			}
		}

			// **===  Global Inputs  ===**
			sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
			sf::Vector2f currentMouseTarget(static_cast<float>(mousePixelPos.x), static_cast<float>(mousePixelPos.y));


			// **=== Update Agents ===**

			// -- Update Agents from Sliders --
			// Get slider values
			float currentSpeed = sliders[0]->getValue();

			float currentSeekWeighting = sliders[1]->getValue();
			float currentSeekStrength = sliders[2]->getValue();
			float currentSeekMaxSteeringForce = sliders[3]->getValue();

			float currentFleeWeighting = sliders[4]->getValue();
			float currentFleeStrength = sliders[5]->getValue();
			float currentFleeMaxSteeringForce = sliders[6]->getValue();

			float currentWanderWeighting = sliders[7]->getValue();
			float currentWanderStrength = sliders[8]->getValue();
			float currentWanderAngleRandomStrength = sliders[9]->getValue();
			float currentWanderMaxSteeringForce = sliders[10]->getValue();

			float currentSeparationWeighting = sliders[11]->getValue();
			float currentSeparationStrength = sliders[12]->getValue();
			float currentSeparationNeighbourhoodRadius = sliders[13]->getValue();
			float currentSeparationMaxSteeringForce = sliders[14]->getValue();

			// Update agents values
			for (auto& agent : agents)
			{
				agent->setTargetPosition(currentMouseTarget);
				agent->setSpeed(currentSpeed);

				agent->setSeekWeighting(currentSeekWeighting);
				agent->setSeekStrength(currentSeekStrength);
				agent->setSeekMaxSteeringForce(currentSeekMaxSteeringForce);

				agent->setFleeWeighting(currentFleeWeighting);
				agent->setFleeStrength(currentFleeStrength);
				agent->setFleeMaxSteeringForce(currentFleeMaxSteeringForce);

				agent->setWanderWeighting(currentWanderWeighting);
				agent->setWanderStrength(currentWanderStrength);
				agent->setWanderAngleRandomStrength(currentWanderAngleRandomStrength * 3);
				agent->setWanderMaxSteeringForce(currentWanderMaxSteeringForce);

				agent->setSeparationWeighting(currentSeparationWeighting);
				agent->setSeparationStrength(currentSeparationStrength);
				agent->setSeparationNeighbourhoodRadius(currentSeparationNeighbourhoodRadius);
				agent->setSeparationMaxSteeringForce(currentSeparationMaxSteeringForce);
			}

			// **=== Rendering ===**
			window.clear(sf::Color::White);

			// Draw all agents
			for (Agent* agent : agents)
			{
				agent->update(dtSeconds, window, agents);
				window.draw(*agent);
			}

			// Draw Visualizations
			if (showVisualizations)
			{
				// Draw the target position for each agent
				for (const Agent* agent : agents)
				{
					agent->drawVisualizations(window, agents); // Draw the agent's visualizations
				}
			}

			// Draw Sliders
			for (const auto& slider_ptr : sliders) {
				window.draw(*slider_ptr); // Dereference the unique_ptr to draw the Slider
			}

			window.display();
		}
}