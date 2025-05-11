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
#include "Obstacle.h"

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

	// **=== Obstacle Creation ===**
	std::vector<Obstacle> obstacles;
	obstacles.emplace_back(Obstacle(sf::Vector2f(200.f, 150.f), sf::Vector2f(100.f, 80.f)));
	obstacles.emplace_back(Obstacle(sf::Vector2f(500.f, 400.f), sf::Vector2f(120.f, 60.f)));
	obstacles.emplace_back(Obstacle(sf::Vector2f(800.f, 250.f), sf::Vector2f(70.f, 70.f)));


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
	float sliderStartX = 10.0f; 			       // Starting X position for the sliders

	// Helper to make adding sliders cleaner
	auto addSlider = [&](float minVal, float maxVal, float initVal, const std::string& label) {
		sliders.push_back(std::make_unique<Slider>(sf::Vector2f(sliderStartX, sliderStartY), sliderSize, minVal, maxVal, initVal, &window, label, font));
		sliderStartY += sliderSpacingY; // Advance Y
	};

	// --- Speed Slider ---
	addSlider(0.0f, 100.0f, firstAgent->getSpeed(), "Speed: ");
	// --- Seek Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getSeekWeighting(), "Seek Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeekStrength(), "Seek Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeekMaxSteeringForce(), "Seek Max Force: ");
	// --- Flee Sliders ---
	sliderStartY = 55.0f; // Reset Y position for next section
	addSlider(0.0f, 1.0f, firstAgent->getFleeWeighting(), "Flee Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getFleeStrength(), "Flee Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getFleeMaxSteeringForce(), "Flee Max Force: ");
	// --- Wander Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getWanderWeighting(), "Wander Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getWanderStrength(), "Wander Strength: ");
	addSlider(0.0f, 1.0f, firstAgent->getWanderAngleRandomStrength(), "Wander Angle Range: ");
	addSlider(0.0f, 10.0f, firstAgent->getWanderMaxSteeringForce(), "Wander Max Force: ");
	// --- Separation Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getSeparationWeighting(), "Separation Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeparationStrength(), "Separation Strength: ");
	addSlider(0.0f, 100.0f, firstAgent->getSeparationNeighbourhoodRadius(), "Separation Radius: ");
	addSlider(0.0f, 10.0f, firstAgent->getSeparationMaxSteeringForce(), "Separation Max Force: ");
	sliderStartY += sectionSpacing; // Extra space between sections
	// --- Cohesion Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getCohesionWeighting(), "Cohesion Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getCohesionStrength(), "Cohesion Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getCohesionMaxSteeringForce(), "Cohesion Max Force: ");
	addSlider(0.0f, 200.0f, firstAgent->getCohesionNeighbourhoodRadius(), "Cohesion Radius: ");
	sliderStartY += sectionSpacing;
	// --- Alignment Sliders ---
	addSlider(0.0f, 1.0f, firstAgent->getAlignmentWeighting(), "Alignment Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getAlignmentStrength(), "Alignment Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getAlignmentMaxSteeringForce(), "Alignment Max Force: ");
	addSlider(0.0f, 200.0f, firstAgent->getAlignmentNeighbourhoodRadius(), "Alignment Radius: ");
	// --- Pursuit Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getPursuitWeighting(), "Pursuit Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getPursuitStrength(), "Pursuit Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getPursuitMaxSteeringForce(), "Pursuit Max Force: ");
	// --- Evasion Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getEvasionWeighting(), "Evasion Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getEvasionStrength(), "Evasion Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getEvasionMaxSteeringForce(), "Evasion Max Force: ");
	// --- Obstacle Avoidance Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getObstacleAvoidanceWeighting(), "Obstacle Avoidance Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getObstacleAvoidanceStrength(), "Obstacle Avoidance Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getObstacleAvoidanceMaxSteeringForce(), "Obstacle Avoidance Max Force: ");
	addSlider(0.0f, 200.0f, firstAgent->getObstacleDetectionBoxLength(), "Obstacle Detection Length: ");

	bool showVisualizations = false;

    // **=== Game Loop ===**
    sf::Clock deltaClock;

	while (window.isOpen())
	{
		// Calculate delta time
		sf::Time deltaTime = deltaClock.restart();
		float dtSeconds = deltaTime.asSeconds();

		// **=== Event Handling ===**
		// -- Set sliders visibility based on behaviour (doing here so can toggle event handling for invis slider)--
		Behaviour currentBehaviour = firstAgent->getBehaviour();
		sliders[0]->setVisible(currentBehaviour != Behaviour::NONE); // Speed slider visible for all behaviours, just not none

		sliders[1]->setVisible(currentBehaviour == Behaviour::SEEK);
		sliders[2]->setVisible(currentBehaviour == Behaviour::SEEK);
		sliders[3]->setVisible(currentBehaviour == Behaviour::SEEK);

		sliders[4]->setVisible(currentBehaviour == Behaviour::FLEE);
		sliders[5]->setVisible(currentBehaviour == Behaviour::FLEE);
		sliders[6]->setVisible(currentBehaviour == Behaviour::FLEE);

		sliders[7]->setVisible(currentBehaviour == Behaviour::WANDER);
		sliders[8]->setVisible(currentBehaviour == Behaviour::WANDER);
		sliders[9]->setVisible(currentBehaviour == Behaviour::WANDER);
		sliders[10]->setVisible(currentBehaviour == Behaviour::WANDER);

		sliders[11]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[12]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[13]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[14]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[15]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[16]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[17]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[18]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[19]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[20]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[21]->setVisible(currentBehaviour == Behaviour::FLOCKING);
		sliders[22]->setVisible(currentBehaviour == Behaviour::FLOCKING);

		sliders[23]->setVisible(currentBehaviour == Behaviour::PURSUIT);
		sliders[24]->setVisible(currentBehaviour == Behaviour::PURSUIT);
		sliders[25]->setVisible(currentBehaviour == Behaviour::PURSUIT);

		sliders[26]->setVisible(currentBehaviour == Behaviour::EVASION);
		sliders[27]->setVisible(currentBehaviour == Behaviour::EVASION);
		sliders[28]->setVisible(currentBehaviour == Behaviour::EVASION);

		sliders[29]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);
		sliders[30]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);
		sliders[31]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);
		sliders[32]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);

		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
			// **=== Key Events ===**
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::V) {
					showVisualizations = !showVisualizations; // Toggle visualizations
				}
				// -- Change what visuals and sliders to show --
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {

					// If already in seek change to none
					if (firstAgent->getBehaviour() == Behaviour::SEEK) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to Seek behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::SEEK);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {

					// If already in flee change to none
					if (firstAgent->getBehaviour() == Behaviour::FLEE) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to flee behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::FLEE);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
					// If already in wander change to none
					if (firstAgent->getBehaviour() == Behaviour::WANDER) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to wander behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::WANDER);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4) {
					// If already in flocking change to none
					if (firstAgent->getBehaviour() == Behaviour::FLOCKING) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to flocking behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::FLOCKING);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5) {
					// If already in pursuit change to none
					if (firstAgent->getBehaviour() == Behaviour::PURSUIT) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to pursuit behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::PURSUIT);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num6) {
					// If already in evasion change to none
					if (firstAgent->getBehaviour() == Behaviour::EVASION) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to evasion behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::EVASION);
						}
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num7) {
					// If already in obstacle avoidance change to none
					if (firstAgent->getBehaviour() == Behaviour::OBSTACLE_AVOIDANCE) {
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::NONE);
						}
					}
					else {
						// Change to obstacle avoidance behaviour
						for (auto& agent : agents) {
							agent->setBehaviour(Behaviour::OBSTACLE_AVOIDANCE);
						}
					}
				}
			}

			// **=== UI Interaction ===**

			// -- Slider Interaction --
			if (event) {
				for (auto& slider_ptr : sliders) { // Iterate through all sliders in the vector
					if (slider_ptr->getIsVisible()) { // Only handle events for visible sliders
						slider_ptr->handleEvent(*event);
					}
				}

			}
		}

		// Mouse position
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

		float currentCohesionWeighting = sliders[15]->getValue();
		float currentCohesionStrength = sliders[16]->getValue();
		float currentCohesionMaxSteeringForce = sliders[17]->getValue();
		float currentCohesionNeighbourhoodRadius = sliders[18]->getValue();

		float currentAlignmentWeighting = sliders[19]->getValue();
		float currentAlignmentStrength = sliders[20]->getValue();
		float currentAlignmentMaxSteeringForce = sliders[21]->getValue();
		float currentAlignmentNeighbourhoodRadius = sliders[22]->getValue();

		float currentPursuitWeighting = sliders[23]->getValue();
		float currentPursuitStrength = sliders[24]->getValue();
		float currentPursuitMaxSteeringForce = sliders[25]->getValue();

		float currentEvasionWeighting = sliders[26]->getValue();
		float currentEvasionStrength = sliders[27]->getValue();
		float currentEvasionMaxSteeringForce = sliders[28]->getValue();

		float currentObstacleAvoidanceWeighting = sliders[29]->getValue();
		float currentObstacleAvoidanceStrength = sliders[30]->getValue();
		float currentObstacleAvoidanceMaxSteeringForce = sliders[31]->getValue();
		float currentObstacleDetectionBoxLength = sliders[32]->getValue();

		// Update agents
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

			agent->setCohesionWeighting(currentCohesionWeighting);
			agent->setCohesionStrength(currentCohesionStrength);
			agent->setCohesionNeighbourhoodRadius(currentCohesionNeighbourhoodRadius);
			agent->setCohesionMaxSteeringForce(currentCohesionMaxSteeringForce);

			agent->setAlignmentWeighting(currentAlignmentWeighting);
			agent->setAlignmentStrength(currentAlignmentStrength);
			agent->setAlignmentMaxSteeringForce(currentAlignmentMaxSteeringForce);
			agent->setAlignmentNeighbourhoodRadius(currentAlignmentNeighbourhoodRadius);

			agent->setPursuitWeighting(currentPursuitWeighting);
			agent->setPursuitStrength(currentPursuitStrength);
			agent->setPursuitMaxSteeringForce(currentPursuitMaxSteeringForce);

			agent->setEvasionWeighting(currentEvasionWeighting);
			agent->setEvasionStrength(currentEvasionStrength);
			agent->setEvasionMaxSteeringForce(currentEvasionMaxSteeringForce);

			agent->setObstacleAvoidanceWeighting(currentObstacleAvoidanceWeighting);
			agent->setObstacleAvoidanceStrength(currentObstacleAvoidanceStrength);
			agent->setObstacleAvoidanceMaxSteeringForce(currentObstacleAvoidanceMaxSteeringForce);
			agent->setObstacleDetectionBoxLength(currentObstacleDetectionBoxLength);

			agent->update(dtSeconds, window, agents, obstacles);
		}

		// **=== Rendering ===**
		window.clear(sf::Color::White);

		// Draw obstacles
		for (const Obstacle& obs : obstacles) {
			obs.draw(window);
		}

		// Draw all agents
		for (Agent* agent : agents)
		{
			window.draw(*agent);
		}
		// Draw Visualizations
		if (showVisualizations)
		{
			for (const Agent* agent : agents)
			{
				agent->drawVisualizations(window, agents); // Draw the agent's visualizations
			}
		}
	    
		// Draw sliders
		for (auto& slider_ptr : sliders) {
			if (slider_ptr->getIsVisible()) {
				window.draw(*slider_ptr);
			}
		}
		
		window.display();
	}
}