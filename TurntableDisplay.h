/*
  ==============================================================================

    TurntableDisplay.h
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

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
    TurntableDisplay();
    ~TurntableDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setPositionRelative(double _position);
    void updateNeedlePosition();

private:
    // the relative position of the playhead
    double position{0};

    // layout variables
    float componentSize{};
    float margin{};

    // turntable dimensions
    float turntableRadius{};        // to outer circle edge of turntable
    float turntableInnerRadius{};   // to inner circle edge of turntable
    float toneArmBaseRadius{};      // to outer edge of tone arm base
    float toneArmDistance{};        // straight distance from tone arm base to needle

    // turntable coordinate points
    juce::Point<float> turntableCentre{};   // center of the turntable
    juce::Point<float> toneArmBase{};       // center of the tone arm base
    juce::Point<float> toneArmElbow{};      // elbow of the tone arm
    juce::Point<float> toneArmNeedle{};     // needle of the tone arm
    juce::Point<float> needleTrackStart{};  // starting point of the needle track
    juce::Point<float> needleTrackEnd{};    // end point of the needle track

    // degrees clockwise from top of the turntable for the needle to track
    float needleTrackAngle{125};  

    // tone arm angles
    float startPositionAngle{};   // angle from toneArmBase to needleTrackStart
    float endPositionAngle{};     // angle from toneArmBase to needleTrackEnd
    float rotationRange{};        // degrees rotation range of the tone arm from start to end 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TurntableDisplay)
};
