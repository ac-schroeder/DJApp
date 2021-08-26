/*
  ==============================================================================

    PositionDisplay.cpp
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include <cmath>
#include "PositionDisplay.h"

//==============================================================================
// TODO: Take this param for the change listerner out! Unless... ?
PositionDisplay::PositionDisplay(juce::AudioTransportSource* _transportSource)
    : transportSource{ _transportSource }
{
    // register change listener
    // TODO: Take this out! Unless... ?
    transportSource->addChangeListener(this);
}

PositionDisplay::~PositionDisplay()
{
}

void PositionDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    g.setColour (juce::Colours::grey);
    //g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    double componentSize = juce::jmin(getWidth(), getHeight());
    double center_x = getWidth() / 2;
    double center_y = getHeight() / 2;
    double margin = componentSize * 0.1;
    double outerDiameter = componentSize - (margin * 2);
    double outerRadius = outerDiameter / 2;
    double innerDiameter = outerDiameter * 0.35;
    double innerRadius = innerDiameter / 2;
    double vinylDistance = outerRadius - innerRadius;  

    g.drawEllipse(margin, margin, outerDiameter, outerDiameter, 1);
    g.drawEllipse(margin + vinylDistance, margin + vinylDistance,
                  innerDiameter, innerDiameter, 1);

    g.setColour(juce::Colours::red);
    g.drawEllipse(center_x, center_y, 2, 2, 2);

    const long double pi = 3.141592653589793238462643383279502884L;
    double needleTrackAngle = 30 * (pi / 180);

    // the end coordinates of the tone arm needle
    double end_x = center_x + (innerRadius * std::cos(needleTrackAngle));
    double end_y = center_y + (innerRadius * std::sin(needleTrackAngle));
    // the beginning coordinates of the tone arm needle
    double start_x = center_x + (outerRadius * std::cos(needleTrackAngle));
    double start_y = center_y + (outerRadius * std::sin(needleTrackAngle));

    g.setColour(juce::Colours::red);
    g.drawEllipse(end_x, end_y, 2, 2, 2);

    g.setColour(juce::Colours::red);
    g.drawEllipse(start_x, start_y, 2, 2, 2);

    // Do this only if you need to use transport source anyway
    //if (fileLoaded)
    //{

    //}
    // Otherwise, just check if a position value exists
    if (position > 0)
    {
        int distanceIntoTrack = position * vinylDistance;
        int newRadius = outerRadius - distanceIntoTrack;

        double x = center_x + (newRadius * std::cos(needleTrackAngle));
        double y = center_y + (newRadius * std::sin(needleTrackAngle));

        g.setColour(juce::Colours::lightgoldenrodyellow);
        g.drawEllipse(x, y, 1, 1, 1);

    }
}

void PositionDisplay::resized()
{
}

void PositionDisplay::setPositionRelative(double _position)
{
    // only change position if it's changed
    if (_position != position)
    {
        position = _position;
        repaint();
    }
}


/** Implement ChangeListener: Detect broadcasts from the audioSource in order to update the waveform display */
// TODO: Take this out! Unless... ?
void PositionDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    DBG("PositionDisplay: Change received");
    repaint();
}