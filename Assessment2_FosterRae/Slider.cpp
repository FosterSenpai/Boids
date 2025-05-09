// /***********************************************************************
// File Name : Slider.cpp
// Description : Implementation file for the Slider class.
// Author : Foster Rae
// Mail : fosterjameskiatarae@gmail.com
// ************************************************************************/

// **=== Includes ===**
#include "Slider.h"
#include <SFML/Graphics/Transform.hpp>
#include <algorithm>
#include <string>
#include <iostream> 

// **=== Constructor ===**
Slider::Slider(sf::Vector2f position, sf::Vector2f size, float minValue, float maxValue, float initialValue, sf::RenderWindow* window, const std::string& label)
    :
    m_font(),
    m_valueText(m_font),
    m_size(size),
    m_minValue(minValue),
    m_maxValue(maxValue),
    m_value(initialValue),
    m_isHandleBeingDragged(false),
    m_localHandleGrabOffset(0.f),
    m_window(window),
    m_labelPrefix(label)
{
    sf::Transformable::setPosition(position); // Set pos

    // --- Track and Handle Setup ---
    // Create track relative to the Slider's local origin
    m_track.setSize(m_size);
    m_track.setFillColor(sf::Color(100, 100, 100));
    m_track.setPosition({ 0.f, 0.f }); // Position track at Slider's local origin

    // Create handle relative to the track
    float handleHeight = size.y * 1.5f; // Make handle taller than track
    float handleWidth = handleHeight;   // Make it square
    m_handle.setSize(sf::Vector2f(handleWidth, handleHeight));
    m_handle.setFillColor(sf::Color(200, 200, 200));
    // Handle local position is set by updateHandlePosition based on m_value

    // --- Font and Text ---
    // Load the font using openFromFile (NOW the m_font object exists)
    if (!m_font.openFromFile("PixelDigivolveItalic-dV8R.ttf"))
    {
        std::cerr << "Error: Failed to load font 'PixelDigivolveItalic-dV8R.ttf' for Slider!" << std::endl;
    }

    // Set up the text properties (Font was passed in the constructor, position is set in updateText)
    m_valueText.setCharacterSize(12);
    m_valueText.setFillColor(sf::Color::Black);

    // --- Initial Update ---
    m_value = std::max(m_minValue, std::min(m_value, m_maxValue)); // Clamp initial value just in case
    updateHandlePosition(); // Set initial handle local position based on initialValue
    updateText();           // Set initial text local position and content
}

// **=== Public Methods ===**

void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Apply the Slider's transform
    states.transform *= getTransform();

    // Draw the components using the transformed states
    // Drawn relative to the Slider's origin
    target.draw(m_track, states);
    target.draw(m_handle, states);
    target.draw(m_valueText, states);
}

void Slider::handleEvent(const sf::Event& event) {
    if (!m_window) return; // Need the window pointer for coordinate mapping

    // --- Convert mouse pos to local slider coords ---
    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(*m_window);
    sf::Vector2f mouseWindowPos = m_window->mapPixelToCoords(mousePixelPos);           // Global coords
    sf::Vector2f localMousePos = getInverseTransform().transformPoint(mouseWindowPos); // Convert to local coords


    // **=== Mouse Events ===**
    // --- Left Mouse Button Pressed ---
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {

            sf::FloatRect localHandleBounds(m_handle.getPosition(), m_handle.getSize()); // Get handle's local bounds

            if (localHandleBounds.contains(localMousePos)) {                          // If mouse is over the handle
                m_isHandleBeingDragged = true;                                        // Start dragging
                m_localHandleGrabOffset = localMousePos.x - m_handle.getPosition().x; // Calculate offset from handle's local origin
            }
        }
    }

    // --- Left Mouse Button Released ---
    else if (const auto* mouseButtonReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
            m_isHandleBeingDragged = false; // Stop dragging
        }
    }

    // --- Mouse Moved ---
    else if (const auto* mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
        localMousePos = getInverseTransform().transformPoint(m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window))); // Update local mouse pos

        if (m_isHandleBeingDragged) {
            float desiredLocalHandleX = localMousePos.x - m_localHandleGrabOffset; // Calculate desired local handle X position

            // --- Handle Range ---
            float minLocalHandleX = 0.f;                                        // Min local X is 0 (left edge of track)
            float maxLocalHandleX = m_track.getSize().x - m_handle.getSize().x; // Max local X = track width - handle width

            if (maxLocalHandleX < minLocalHandleX) maxLocalHandleX = minLocalHandleX; // Ensure max >= min

            float clampedLocalHandleX = std::max(minLocalHandleX, std::min(desiredLocalHandleX, maxLocalHandleX)); // Clamp to local bounds

            // --- Calculate the Value ---
            float localRange = maxLocalHandleX - minLocalHandleX; // Range of motion for the handle
            float handlePosNormalized = (localRange > 0) ? ((clampedLocalHandleX - minLocalHandleX) / localRange) : 0.f; // Normalize the handle position

            // Prevent division by zero
            float valueRange = m_maxValue - m_minValue;
            m_value = (valueRange != 0) ? (m_minValue + valueRange * handlePosNormalized) : m_minValue;

            m_value = std::max(m_minValue, std::min(m_value, m_maxValue)); // Clamp final value to range

            // --- Set Handle Position ---
            float targetLocalHandleY = (m_track.getSize().y - m_handle.getSize().y) / 2.0f; // Center the handle vertically
            m_handle.setPosition({ clampedLocalHandleX, targetLocalHandleY });

            updateText(); // Update the text display based on the new value
        }
    }
}


float Slider::getValue() const {
    return m_value;
}

void Slider::setPosition(const sf::Vector2f& position) {
    sf::Transformable::setPosition(position); // Using Transformable's setPosition
}

void Slider::setLabel(const std::string& label) {
    m_labelPrefix = label;
    updateText(); // Update display immediately
}

void Slider::updateText() {
    if (m_font.getInfo().family.empty()) {    // Check if font is loaded
        m_valueText.setString("Font Error!"); // error if font not loaded
    }
    else {
        std::string valueString; // Use a temporary string for the value part

        // Check if the slider's range is 0.0 to 1.0 to display as percentage
        if (m_minValue == 0.0f && m_maxValue == 1.0f) {
            // Format as percentage
            valueString = std::to_string(static_cast<int>(m_value * 100.0f)) + "%";
        }
        else {
            // Otherwise just int
            valueString = std::to_string(static_cast<int>(m_value));
        }
        m_valueText.setString(m_labelPrefix + valueString);
    }

    // Position text below the track
    float textLocalX = 0.f;                       // Align with left edge of track
    float textLocalY = m_track.getSize().y + 5.f; // Place 5px below track
    m_valueText.setPosition({ textLocalX, textLocalY });
}


// **=== Private Methods ===**

void Slider::updateHandlePosition() {
    // Normalize the sliders current value (clamped in constructor/drag)
    float valueRange = m_maxValue - m_minValue;
    float sliderValueNormalized = (valueRange != 0) ? ((m_value - m_minValue) / valueRange) : 0.f;

    // Calculate the valid range for handle local X position
    float minHandleLocalX = 0.f;
    float maxHandleLocalX = m_track.getSize().x - m_handle.getSize().x;

    // Ensure max >= min
    if (maxHandleLocalX < minHandleLocalX) maxHandleLocalX = minHandleLocalX;
    float handleTravelRange = maxHandleLocalX - minHandleLocalX;

    // Calculate the handle's top-left X position from the normalized value
    float handleLocalX = minHandleLocalX + sliderValueNormalized * handleTravelRange;
    handleLocalX = std::max(minHandleLocalX, std::min(handleLocalX, maxHandleLocalX)); // Clamp to local bounds

    // Center the handle vertically relative to track
    float handleLocalY = (m_track.getSize().y - m_handle.getSize().y) / 2.0f;

    m_handle.setPosition({ handleLocalX, handleLocalY });
}