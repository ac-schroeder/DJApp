/*
  ==============================================================================

    PositionDisplay.h
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PositionDisplay  : public juce::Component
{
public:
    PositionDisplay();
    ~PositionDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionDisplay)
};
