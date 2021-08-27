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

    void setBasePosition(int x, int y);
    void updateNeedlePosition(float x, float y);

private:
    // drawing coordinates
    int base_x;
    int base_y;
    float needlePosition_x;
    float needlePosition_y;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToneArmComponent)
};
