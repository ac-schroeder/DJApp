/*
  ==============================================================================

    MainLookAndFeel.cpp
    Created: 1 Sep 2021 3:43:28pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainLookAndFeel.h"

MainLookAndFeel::MainLookAndFeel()
{
    // colours
    setColour(juce::Slider::thumbColourId, juce::Colours::orange);

    
}
MainLookAndFeel::~MainLookAndFeel()
{

}


void MainLookAndFeel::drawGroupComponentOutline(juce::Graphics& g, int w, int h,
    const juce::String& text, const juce::Justification&, juce::GroupComponent& groupComponent)
{
    g.setColour(juce::Colours::dimgrey);
    juce::Rectangle<float> border = groupComponent.getLocalBounds().toFloat();
    border.reduce(0, 2);
    g.drawRoundedRectangle(border, 6, 2);
}