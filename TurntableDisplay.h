/*
  ==============================================================================

    TurntableDisplay.h
    Created: 25 Aug 2021 3:29:58pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class TurntableDisplay  : public juce::Component
{
public:
    /** Constructor */
    TurntableDisplay();
    /** Destructor */
    ~TurntableDisplay() override;

    /** Implements Component: Paints the component. */
    void paint (juce::Graphics&) override;
    /** Implements Component: Repaints the component on resize. */
    void resized() override;

    /** 
     * Sets the relative position of the turntable's tonearm and repaints. 
     * This is called by DeckGUI::timerCallback() to sync the turntable display 
     * with the audio playback.
     *
     * @param _relativePosition - The position of the playhead as a percentage of 
     *     the track length.
     */
    void setPositionRelative(double _relativePosition);

    /** 
     * Updates the coordinates of the toneArmNeedle. Called in paint() 
     * and resized(). Repainting is done whenever setPositionRelative() is called
     * by the DeckGUI::timerCallback function.
     */
    void updateNeedlePosition();

private:
    // The relative position of the playhead as a percentage of track length
    double relativePosition{0};

    // Layout variables
    float componentSize{};
    float margin{};

    // Turntable dimensions
    float turntableRadius{};        // to outer circle edge of turntable
    float turntableInnerRadius{};   // to inner circle edge of turntable
    float toneArmBaseRadius{};      // to outer edge of tone arm base
    float toneArmDistance{};        // straight distance from tone arm base to needle

    // Turntable coordinate points
    juce::Point<float> turntableCentre{};   // center of the turntable
    juce::Point<float> toneArmBase{};       // center of the tone arm base
    juce::Point<float> toneArmElbow{};      // elbow of the tone arm
    juce::Point<float> toneArmNeedle{};     // needle of the tone arm
    juce::Point<float> needleTrackStart{};  // starting point of the needle track
    juce::Point<float> needleTrackEnd{};    // end point of the needle track

    // Degrees clockwise from top of the turntable for the needle to track
    float needleTrackAngle{125};  

    // Tone arm angles
    float startPositionAngle{};   // angle from toneArmBase to needleTrackStart
    float endPositionAngle{};     // angle from toneArmBase to needleTrackEnd
    float rotationRange{};        // degrees rotation range of the tone arm from start to end 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TurntableDisplay)
};
