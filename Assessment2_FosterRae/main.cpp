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

    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Foster's Boids", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

	// Load font
	sf::Font font;
	if (!font.openFromFile("PixelDigivolveItalic-dV8R.ttf")) {
		std::cerr << "Error: Failed to load font for Sliders" << std::endl;
		return EXIT_FAILURE;
	}

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
	obstacles.emplace_back(Obstacle(sf::Vector2f(260.f, 230.f), sf::Vector2f(100.f, 80.f)));
	obstacles.emplace_back(Obstacle(sf::Vector2f(600.f, 300.f), sf::Vector2f(100.f, 80.f)));
	obstacles.emplace_back(Obstacle(sf::Vector2f(1000.f, 400.f), sf::Vector2f(100.f, 80.f)));

	// **=== UI Setup ===**
	// -- Text in bottom left corner --
	sf::Text behaviourText(font);
	behaviourText.setCharacterSize(20);
	behaviourText.setFillColor(sf::Color::Black);
	behaviourText.setPosition({10.f, window.getSize().y - 30.f});
	// Build string from first agents behaviour (updated down by draw)
	std::string behaviourString = "Behaviour: ";
	behaviourText.setString(behaviourString);
	// -- Text in bottom right corner --
	sf::Text instructionText(font);
	instructionText.setString("1-9: Change Behaviour | V: Toggle Visuals | LMB: Spawn/Target");
	instructionText.setCharacterSize(14); // Slightly smaller
	instructionText.setFillColor(sf::Color::Black);
	// Set position to bottom right corner
	instructionText.setPosition({ 750.0f, 690.0f});


	// ---- Sliders ----
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
	addSlider(0.0f, 30.0f, firstAgent->getSpeed(), "Speed: ");
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
	addSlider(0.0f, 300.0f, firstAgent->getObstacleDetectionBoxLength(), "Obstacle Detection Length: ");
	addSlider(0.0f, 10.0f, firstAgent->getNormalInfluence(), "Normal Influence: ");
	addSlider(0.0f, 10.0f, firstAgent->getTangentInfluence(), "Tangent Influence: ");
	// --- Arrival Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getArrivalWeighting(), "Arrival Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getArrivalStrength(), "Arrival Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getArrivalMaxSteeringForce(), "Arrival Max Force: ");
	addSlider(0.0f, 300.0f, firstAgent->getArrivalSlowingRadius(), "Arrival Slowing Radius: ");
	// --- Leader Following Sliders ---
	sliderStartY = 55.0f;
	addSlider(0.0f, 1.0f, firstAgent->getLeaderFollowingWeighting(), "Leader Following Weighting: ");
	addSlider(0.0f, 10.0f, firstAgent->getLeaderFollowingStrength(), "Leader Following Strength: ");
	addSlider(0.0f, 10.0f, firstAgent->getLeaderFollowingMaxSteeringForce(), "Leader Following Max Force: ");


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
		sliders[33]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);
		sliders[34]->setVisible(currentBehaviour == Behaviour::OBSTACLE_AVOIDANCE);

		sliders[35]->setVisible(currentBehaviour == Behaviour::ARRIVAL);
		sliders[36]->setVisible(currentBehaviour == Behaviour::ARRIVAL);
		sliders[37]->setVisible(currentBehaviour == Behaviour::ARRIVAL);
		sliders[38]->setVisible(currentBehaviour == Behaviour::ARRIVAL);

		sliders[39]->setVisible(currentBehaviour == Behaviour::LEADER_FOLLOWING);
		sliders[40]->setVisible(currentBehaviour == Behaviour::LEADER_FOLLOWING);
		sliders[41]->setVisible(currentBehaviour == Behaviour::LEADER_FOLLOWING);

		// Checking if a preset was applied in the last frame to hardcode the sliders
		Behaviour activePresetForCurrentFrame = Behaviour::NONE; // Stores the preset chosen in the current event poll
		Behaviour presetAppliedThisFrame = activePresetForCurrentFrame; // Store what was set by events
		activePresetForCurrentFrame = Behaviour::NONE; // Reset for the next event polling cycle

		// **=== Poll Events ===**
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();

			// **=== Mouse Events ===**
            if (event->is<sf::Event::MouseButtonPressed>() && event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
			{
				// spawn agents at mouse position
				sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
				agents.push_back(new Agent(mousePos));
				agents.back()->setBehaviour(firstAgent->getBehaviour());
			}
			// **=== Key Events ===**
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::V) {
					showVisualizations = !showVisualizations; // Toggle visualizations
				}
				// -- Change what visuals and sliders to show --
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {

					if (presetAppliedThisFrame == Behaviour::SEEK) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::SEEK;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
					
					if (presetAppliedThisFrame == Behaviour::FLEE) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::FLEE;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {

					if (presetAppliedThisFrame == Behaviour::WANDER) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::WANDER;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num4) {
					if (presetAppliedThisFrame == Behaviour::FLOCKING) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::FLOCKING;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num5) {
					if (presetAppliedThisFrame == Behaviour::PURSUIT) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::PURSUIT;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num6) {
					if (presetAppliedThisFrame == Behaviour::EVASION) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::EVASION;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num7) {
					if (presetAppliedThisFrame == Behaviour::OBSTACLE_AVOIDANCE) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::OBSTACLE_AVOIDANCE;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num8) {
					if (presetAppliedThisFrame == Behaviour::ARRIVAL) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::ARRIVAL;
					}
				}
				else if (keyPressed->scancode == sf::Keyboard::Scancode::Num9) {
					if (presetAppliedThisFrame == Behaviour::LEADER_FOLLOWING) {
						presetAppliedThisFrame = Behaviour::NONE; // Toggle off
						for (auto& agent : agents) { agent->setBehaviour(Behaviour::NONE); }
					}
					else {
						presetAppliedThisFrame = Behaviour::LEADER_FOLLOWING;
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
		float currentNormalInfluence = sliders[33]->getValue();
		float currentTangentInfluence = sliders[34]->getValue();

		float currentArrivalWeighting = sliders[35]->getValue();
		float currentArrivalStrength = sliders[36]->getValue();
		float currentArrivalMaxSteeringForce = sliders[37]->getValue();
		float currentArrivalSlowingRadius = sliders[38]->getValue();

		float currentLeaderFollowingWeighting = sliders[39]->getValue();
		float currentLeaderFollowingStrength = sliders[40]->getValue();
		float currentLeaderFollowingMaxSteeringForce = sliders[41]->getValue();

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
			agent->setNormalInfluence(currentNormalInfluence);
			agent->setTangentInfluence(currentTangentInfluence);

			agent->setArrivalWeighting(currentArrivalWeighting);
			agent->setArrivalStrength(currentArrivalStrength);
			agent->setArrivalMaxSteeringForce(currentArrivalMaxSteeringForce);
			agent->setArrivalSlowingRadius(currentArrivalSlowingRadius);

			agent->setLeaderFollowingWeighting(currentLeaderFollowingWeighting);
			agent->setLeaderFollowingStrength(currentLeaderFollowingStrength);
			agent->setLeaderFollowingMaxSteeringForce(currentLeaderFollowingMaxSteeringForce);

			agent->update(dtSeconds, window, agents, obstacles);
		}
		// **=== Apply Hardcoded Preset Values IF a key was pressed THIS frame ===** (im sorry for the mess)
		if (presetAppliedThisFrame != Behaviour::NONE) {
			for (auto& agent : agents) {
				if (!agent) continue;

				agent->setBehaviour(presetAppliedThisFrame); // Set the core behavior

				if (presetAppliedThisFrame == Behaviour::SEEK) {
					// Optimal values for SEEK
					sliders[1]->setValue(1.0f); // Seek Weighting
					sliders[2]->setValue(2.0f); // Seek Strength
					sliders[3]->setValue(5.0f); // Seek Max Steering Force
					// Optimal values for other behaviours
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.1f);  // Wander Weighting
					sliders[11]->setValue(0.6f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::FLEE) 
				{
					// Optimal values for FLEE
					sliders[4]->setValue(1.0f); // Flee Weighting
					sliders[5]->setValue(2.0f); // Flee Strength
					sliders[6]->setValue(5.0f); // Flee Max Steering Force
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[7]->setValue(0.1f);  // Wander Weighting
					sliders[11]->setValue(0.4f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::WANDER)
				{
					// Optimal values for WANDER
					sliders[7]->setValue(1.0f); // Wander Weighting
					sliders[8]->setValue(2.0f); // Wander Strength
					sliders[9]->setValue(0.4f); // Wander Angle Random Strength
					sliders[10]->setValue(5.0f); // Wander Max Steering Force
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[11]->setValue(0.4f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::FLOCKING)
				{
					// Optimal values for FLOCKING
					// Seperation values
					sliders[11]->setValue(1.0f); // Separation Weighting
					sliders[12]->setValue(3.0f); // Separation Strength
					sliders[13]->setValue(20.0f); // Separation Neighbourhood Radius
					sliders[14]->setValue(5.0f); // Separation Max Steering Force
					// Cohesion values
					sliders[15]->setValue(0.8f); // Cohesion Weighting
					sliders[16]->setValue(3.0f); // Cohesion Strength
					sliders[18]->setValue(150.0f); // Cohesion Neighbourhood Radius
					sliders[17]->setValue(5.0f); // Cohesion Max Steering Force
					// Alignment values
					sliders[19]->setValue(0.5f); // Alignment Weighting
					sliders[20]->setValue(2.0f); // Alignment Strength
					sliders[22]->setValue(20.0f);// Alignment Neighbourhood Radius
					sliders[21]->setValue(5.0f); // Alignment Max Steering Force
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.2f);  // Wander Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::PURSUIT)
				{
					// Optimal values for PURSUIT
					sliders[23]->setValue(1.0f); // Pursuit Weighting
					sliders[24]->setValue(2.0f); // Pursuit Strength
					sliders[25]->setValue(5.0f); // Pursuit Max Steering Force
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.1f);  // Wander Weighting
					sliders[11]->setValue(0.4f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::EVASION)
				{
					// Optimal values for EVASION
					sliders[26]->setValue(1.0f); // Evasion Weighting
					sliders[27]->setValue(4.0f); // Evasion Strength
					sliders[28]->setValue(5.0f); // Evasion Max Steering Force
					// Optimal values for other behaviours
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.1f);  // Wander Weighting
					sliders[11]->setValue(0.4f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::OBSTACLE_AVOIDANCE)
				{
					// Optimal values for OBSTACLE AVOIDANCE
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[30]->setValue(10.0f); // Obstacle Avoidance Strength
					sliders[31]->setValue(10.0f); // Obstacle Avoidance Max Steering Force
					sliders[32]->setValue(100.0f); // Obstacle Detection Length
					sliders[33]->setValue(2.0f); // Normal Influence
					sliders[34]->setValue(7.0f); // Tangent Influence
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.4f);  // Wander Weighting
					sliders[11]->setValue(0.4f); // Separation Weighting
					sliders[15]->setValue(0.2f); // Cohesion Weighting
					sliders[19]->setValue(0.3f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::ARRIVAL)
				{
					// Optimal values for ARRIVAL
					sliders[35]->setValue(1.0f); // Arrival Weighting
					sliders[36]->setValue(5.0f); // Arrival Strength
					sliders[37]->setValue(5.0f); // Arrival Max Steering Force
					sliders[38]->setValue(150.0f); // Arrival Slowing Radius
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.0f);  // Wander Weighting
					sliders[11]->setValue(0.0f); // Separation Weighting
					sliders[15]->setValue(0.0f); // Cohesion Weighting
					sliders[19]->setValue(0.0f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[39]->setValue(0.0f); // Leader Following Weighting
				}
				else if (presetAppliedThisFrame == Behaviour::LEADER_FOLLOWING)
				{
					// Optimal values for LEADER FOLLOWING
					sliders[39]->setValue(1.0f); // Leader Following Weighting
					sliders[40]->setValue(5.0f); // Leader Following Strength
					sliders[41]->setValue(5.0f); // Leader Following Max Steering Force
					// Optimal values for other behaviours
					sliders[1]->setValue(0.0f);  // Seek Weighting
					sliders[4]->setValue(0.0f);  // Flee Weighting
					sliders[7]->setValue(0.0f);  // Wander Weighting
					sliders[11]->setValue(0.8f); // Separation Weighting
					sliders[12]->setValue(10.0f); // Separation Strength
					sliders[13]->setValue(30.0f); // Separation Neighbourhood Radius
					sliders[15]->setValue(0.0f); // Cohesion Weighting
					sliders[19]->setValue(0.0f); // Alignment Weighting
					sliders[23]->setValue(0.0f); // Pursuit Weighting
					sliders[26]->setValue(0.0f); // Evasion Weighting
					sliders[29]->setValue(1.0f); // Obstacle Avoidance Weighting
					sliders[35]->setValue(0.0f); // Arrival Weighting
				}
			}
		}

		// **=== Update Behaviour Text ===**
		std::string currentBehaviourString = "Behaviour: ";
		switch (agents[1]->getBehaviour())
		{
		case Behaviour::SEEK: currentBehaviourString += "SEEK"; break;
		case Behaviour::FLEE: currentBehaviourString += "FLEE"; break;
		case Behaviour::WANDER: currentBehaviourString += "WANDER"; break;
		case Behaviour::FLOCKING: currentBehaviourString += "FLOCKING"; break;
		case Behaviour::PURSUIT: currentBehaviourString += "PURSUIT"; break;
		case Behaviour::EVASION: currentBehaviourString += "EVASION"; break;
		case Behaviour::OBSTACLE_AVOIDANCE: currentBehaviourString += "OBSTACLE AVOIDANCE"; break;
		case Behaviour::ARRIVAL: currentBehaviourString += "ARRIVAL"; break;
		case Behaviour::LEADER_FOLLOWING: currentBehaviourString += "LEADER FOLLOWING"; break;
		default: currentBehaviourString += "NONE"; break;
		}
		behaviourText.setString(currentBehaviourString);
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

		// Draw text
		window.draw(behaviourText);
		window.draw(instructionText);
		
		window.display();
	}

}