Project Title: AI Steering Behaviours Demonstration  
Author:         Foster Rae  
Course:         GD2P01 - Artificial Intelligence for Games  
Assignment:     Locomotion Project  
Date:           May 12, 2025  
================================================================================

TABLE OF CONTENTS
-----------------
1.  DESCRIPTION
2.  IMPLEMENTED STEERING BEHAVIOURS  
    2.1. Simple Behaviours (Individuals and Pairs)  
    2.2. Combined Behaviours: Flocking  
    2.3. Combined Behaviours: Leader Following  
3.  CONTROLS
4.  DEPENDENCIES
5.  RUNNING THE APPLICATION
6.  KNOWN ISSUES / NOTES

-----------------
1. DESCRIPTION
-----------------
This C++ application demonstrates various steering behaviours for autonomous agents,
developed using the SFML (Simple and Fast Multimedia Library). Agents navigate a 2D
environment containing obstacles, showcasing behaviours such as seeking, fleeing,
wandering, flocking, and leader following. The application allows for real-time
adjustment of behaviour parameters via on-screen sliders and provides debug
visualizations to illustrate the underlying mechanics.

---------------------------------
2. IMPLEMENTED STEERING BEHAVIOURS
---------------------------------
The application implements and demonstrates the following steering behaviours:

2.1. Simple Behaviours:
- SEEK/FLEE
- PURSUE/EVADE
- WANDER
- ARRIVAL
- OBSTACLE AVOIDANCE

2.2. Combined Behaviours: 
- FLOCKING
- LEADER FOLLOWING

-----------------
3. CONTROLS
-----------------
- Mouse:
    - Left Mouse Button Click:
        1. Spawns a new agent at the cursor position. The new agent will adopt
           the currently selected global behaviour.
    - Mouse Position
        1. Serves as the dynamic target point for Seek, Flee, and Arrival
           behaviours.
        2. Controls the "target/leader" agent (the first agent spawned) when
           behaviours like Pursuit, Evasion, or Leader Following are active for
           other agents (the leader itself uses Arrival/Seek to follow the mouse).

- Keyboard:
    - Number Keys (1-9): Switch between active steering behaviour presets:
        - 1: SEEK
        - 2: FLEE
        - 3: WANDER
        - 4: FLOCKING (includes Separation, Alignment, Cohesion, Wander, Obstacle Avoidance)
        - 5: PURSUIT (targets the first agent)
        - 6: EVASION (evades the first agent)
        - 7: OBSTACLE AVOIDANCE
        - 8: ARRIVAL
        - 9: LEADER FOLLOWING (follows the first agent)

    - V Key: Toggle on/off debug visualizations.

- On-Screen UI:
    - Sliders: Sliders dynamically appear/disappear based on the selected
      behaviour, allowing real-time adjustment of relevant parameters for all agents.

-----------------
4. DEPENDENCIES
-----------------
- C++ Compiler: A compiler supporting C++17.
- SFML (Simple and Fast Multimedia Library): Version 3.0.0 (or compatible).
  The necessary SFML headers and libraries must be correctly linked for compilation.
  (Solution should have these linked already, Ensure debug and release folders have
  respective .dll files in them.)
  - RELEASE:
  - sfml-graphics.
  - sfml-window
  - sfml-system
  - DEBUG:
  - sfml-graphics-d
  - sfml-window-d
  - sfml-system-d
- Font: `PixelDigivolveItalic-dV8R.ttf`.

---------------------------
5. RUNNING THE APPLICATION
---------------------------
1. Ensure the executable is in the same directory as the required DLLs 
   (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`) and the 
   font file (`PixelDigivolveItalic-dV8R.ttf`).
2. Run the executable file (e.g., `Assessment2_FosterRae.exe`).

-------------------------
6. KNOWN ISSUES / NOTES
-------------------------

- No major issues, but there's a fair bit of code repetition and some things that 
  could use a good cleanup pass. For the most part, everything works as intended.

- Screen Wrap & Distances: Right now, behaviours like Cohesion don't "see" that an
  agent on the far left is actually close to an agent on the far right if they
  wrap around the screen. Making the distance calculations aware of this screen
  wrapping would make these group behaviours feel more natural.

- Efficiency with Sqrt: I've used my `Utils::magnitude` function (which uses `sqrt`)
  all over the place for distance checks. A bunch of these could probably be swapped
  out for squared magnitude comparisons to speed things up.

- Finding the Target Agent: In a few spots, the code loops through all agents to
  find the target (like the leader for Leader Following). It'd likely be more
  efficient to just grab this target once and store a direct reference to it.

- Target Agent Hardcoding (`agents[0]`): Super sketchy. For behaviours like Pursuit, 
  Evade, and Leader Following, the target is pretty much always hardcoded to be 
  `agents[0]` (the first agent spawned). This started as a quick way to dodge some 
  null pointer issues but ended up quicklt cascading into some really gross code (sorry). 
  It works for now because the leader is always agent 0, but it's not ideal. It would 
  be a lot better to handle this properly to avoid issues and be able to reassign the 
  target agent to any agent.

- SFML C4275 Warning: You might see a C4275 warning from SFML during compilation
  (something about `std::runtime_error` and `sf::Exception`). It's a known thing
  with SFML's DLLs and doesn't seem to break anything.

-------------------------
