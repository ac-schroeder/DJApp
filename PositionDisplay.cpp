/*
  ==============================================================================

    PositionDisplay.cpp
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PositionDisplay.h"

//==============================================================================
PositionDisplay::PositionDisplay()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PositionDisplay::~PositionDisplay()
{
}

void PositionDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    double maxSize = juce::jmin(getWidth(), getHeight());
    double margin = maxSize * 0.1;
    double outerDiameter = maxSize - (margin * 2);
    double outerRadius = outerDiameter / 2;
    double innerDiameter = outerDiameter * 0.35;
    double innerRadius = innerDiameter / 2;
    double radiusDifference = outerRadius - innerRadius;

    g.drawEllipse(margin, margin, outerDiameter, outerDiameter, 1);
    g.drawEllipse(margin + radiusDifference, margin + radiusDifference,
                  innerDiameter, innerDiameter, 1);
}

void PositionDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
