// /***********************************************************************
// File Name : Slider.h
// Description : Header file for the Slider class. Sliders are UI elements
//               that allow users to select a value from a range by dragging
//               a handle along a track. The Slider class handles the
//               rendering, input handling, and value management of the slider.
//               It inherits from sf::Drawable and sf::Transformable.
//               I mainly use to update somethings variables from the value the 
//               slider is set to. Pretty scuffed constructor, could be better.
// Author : Foster Rae
// Mail : fosterjameskiatarae@gmail.com
// ************************************************************************/

// **=== Includes ===**
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <iostream>

class Slider : public sf::Drawable, public sf::Transformable
{
public:
    // **=== Constructor ===**

    /**
     * @brief Constructs a Slider object.
     * Positions the slider using its Transformable component.
     * Internal elements (track, handle) are positioned relative to the Slider's origin.
     * @param position The top-left position of the slider in world coordinates.
     * @param size The size of the slider track (length x height).
     * @param minValue The minimum value the slider can represent.
     * @param maxValue The maximum value the slider can represent.
     * @param initialValue The starting value of the slider.
     * @param window Pointer to the SFML window for coordinate mapping.
     * @param label The text label prefix to display.
     */
    Slider(sf::Vector2f position, sf::Vector2f size, float minValue, float maxValue, float initialValue, sf::RenderWindow* window, const std::string& label);

    // **=== Public Methods ===**

    /**
     * @brief Draws the slider (track, handle, text) to the render target.
     * Applies the Slider's transform.
     * @param target Render target to draw to.
     * @param states Current render states (including transforms).
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * @brief Handles SFML events for slider interaction (click, drag).
     * Converts mouse coordinates to the slider's local system for processing.
     * @param event The SFML event to handle.
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Gets the current value of the slider.
     * @return The current value.
     */
    float getValue() const;

    /**
     * @brief Sets the overall position of the Slider using its Transformable component.
     * @param position The new top-left position in world coordinates.
     */
    void setPosition(const sf::Vector2f& position);

    /**
     * @brief Sets the text label prefix for the slider.
     * @param label The new label prefix.
     */
    void setLabel(const std::string& label);

    /**
     * @brief Updates the text string and its local position. Called internally.
     */
    void updateText();

private:
    // **=== Private Methods ===**

    /**
     * @brief Updates the handle's local position based on the current slider value.
     */
    void updateHandlePosition();

    // **=== Private Members ===**

    sf::RectangleShape m_track;       // The slider track (positioned locally at 0,0)
    sf::RectangleShape m_handle;      // The draggable handle (positioned locally relative to track)
    sf::Font m_font;                  // Font for the value text
    sf::Text m_valueText;             // Displays the current value + label (positioned locally)

    sf::Vector2f m_size;              // Size of the slider track
    float m_minValue;                 // Minimum value of the slider
    float m_maxValue;                 // Maximum value of the slider
    float m_value;                    // Current value of the slider

    bool m_isHandleBeingDragged;      // Flag: True if the user is dragging the handle
    float m_localHandleGrabOffset;    // Horizontal offset from handle's local origin X where the mouse grabbed it

    sf::RenderWindow* m_window;       // Pointer to the window (needed for coordinate mapping)
    std::string m_labelPrefix;        // Label prefix for the text display
};