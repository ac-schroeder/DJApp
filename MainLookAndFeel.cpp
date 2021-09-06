/*
  ==============================================================================

    MainLookAndFeel.cpp
    Created: 1 Sep 2021 3:43:28pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainLookAndFeel.h"

MainLookAndFeel::MainLookAndFeel()
{
    // Set slider thumb colours
    setColour(juce::Slider::thumbColourId, juce::Colours::orange);
}
MainLookAndFeel::~MainLookAndFeel()
{
}

void MainLookAndFeel::drawGroupComponentOutline(juce::Graphics& g, int w, int h,
    const juce::String& text, const juce::Justification&, juce::GroupComponent& groupComponent)
{
    // Draw a rounded edge rectangle to outline the group component
    juce::Rectangle<float> border = groupComponent.getLocalBounds().toFloat();
    border.reduce(0, 2);
    g.setColour(juce::Colours::dimgrey);
    g.drawRoundedRectangle(border, 6, 2);
}

void MainLookAndFeel::fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    // Fill the text box with pale grey
    g.fillAll(juce::Colours::darkgrey);
}

void MainLookAndFeel::drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    // Blend the outline in with the box 
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(textEditor.getLocalBounds(), 1);
}