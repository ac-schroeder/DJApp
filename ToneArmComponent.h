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

    void setBasePosition(juce::Point<float>& _toneArmBase, float _baseRadius);
    void updateNeedlePosition(juce::Point<float>& _needlePosition);

private:
    // drawing coordinates
    juce::Point<float> toneArmBase{};
    juce::Point<float> needlePosition{};
    // base size
    float baseRadius{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneArmComponent)
};
