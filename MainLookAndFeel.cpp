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

void MainLookAndFeel::fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MainLookAndFeel::drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor)
{
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(textEditor.getLocalBounds(), 1);

    //if (textEditor.hasKeyboardFocus(false))
    //{
    //    //g.setColour(juce::Colours::darkgrey);
    //    //g.drawRect(area, 1);
    //}
}