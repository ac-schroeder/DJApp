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

    // add the tone arm graphic
    addAndMakeVisible(toneArm);
}

PositionDisplay::~PositionDisplay()
{
}

void PositionDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    g.setColour (juce::Colours::grey);

    // set layout variables
    componentSize = juce::jmin(getWidth(), getHeight());
    margin = componentSize * 0.1;
    turntable_x = getWidth() / 2;
    turntable_y = getHeight() / 2;
    
    tonearm_x = componentSize - margin - 15;
    tonearm_y = margin;
    toneArm.setBasePosition(tonearm_x, tonearm_y);
    
    // calculate turntable positions
    int turntableDiameter = componentSize - (margin * 2);
    int turntableRadius = turntableDiameter / 2;
    int turntableInnerDiameter = turntableDiameter * 0.35;
    int turntableInnerRadius = turntableInnerDiameter / 2;
    int vinylLength = turntableRadius - turntableInnerRadius;

    // draw the turntable
    g.drawEllipse(margin, margin, turntableDiameter, turntableDiameter, 1);
    g.drawEllipse(margin + vinylLength, margin + vinylLength,
                  turntableInnerDiameter, turntableInnerDiameter, 1);

    // get the needle track path angle
    const long double pi = 3.141592653589793238462643383279502884L;
    float needleTrackAngle = 35 * (pi / 180);

    // the end coordinates of the tone arm needle track path
    float end_x = turntable_x + (turntableInnerRadius * std::cos(needleTrackAngle));
    float end_y = turntable_y + (turntableInnerRadius * std::sin(needleTrackAngle));
    // the beginning coordinates of the tone arm needle track path
    float start_x = turntable_x + (turntableRadius * std::cos(needleTrackAngle));
    float start_y = turntable_y + (turntableRadius * std::sin(needleTrackAngle));

    // mark the end coordinates
    g.setColour(juce::Colours::red);
    g.drawEllipse(end_x, end_y, 2, 2, 2);
    // mark the starting coordinates
    g.setColour(juce::Colours::red);
    g.drawEllipse(start_x, start_y, 2, 2, 2);

    // Check if the playhead position has progressed into the track
    if (position > 0)
    {
        float distanceIntoTrack = position * vinylLength;
        float newRadius = turntableRadius - distanceIntoTrack;

        float needle_x = turntable_x + (newRadius * std::cos(needleTrackAngle));
        float needle_y = turntable_y + (newRadius * std::sin(needleTrackAngle));

        // call toneArmComponent.updateNeedlePosition(x, y) with the above coords.
        toneArm.updateNeedlePosition(needle_x, needle_y);
    }
}

void PositionDisplay::resized()
{
    toneArm.setBounds(getLocalBounds());
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