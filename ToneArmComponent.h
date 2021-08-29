/*
  ==============================================================================

    ToneArm.h
    Created: 27 Aug 2021 10:52:22am
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ToneArmComponent  : public juce::Component
{
public:
    ToneArmComponent();
    ~ToneArmComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setPositionAndSize(juce::Point<float>& _toneArmBase, 
                            juce::Point<float>& toneArmNeedleStart,
                            float _baseRadius);
    void updateNeedlePosition(float currentAngle);

private:
    // drawing coordinates
    juce::Point<float> toneArmBase{};
    juce::Point<float> needlePosition{};
    juce::Point<float> elbowPosition{};
    // tone arm dimensions
    float baseRadius{};     // base size
    float armDistance{};    // distance from base point to needle point

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneArmComponent)
};
