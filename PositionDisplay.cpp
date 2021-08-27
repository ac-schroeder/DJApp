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

    // add the tone arm component
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
    // todo: set the size to square and just use width or height here
    componentSize = juce::jmin(getWidth(), getHeight());
    margin = componentSize * 0.1;

    // set up turntable center point
    turntableCentre.setXY(getWidth() / 2, getHeight() / 2);

    // set up the tone arm base point
    // TODO: make a better way to calculate tonearm base, to be resizeable
    tonearmBase.setXY(componentSize - margin, margin + 12);

    // TODO: Now call toneArm.setBase? or...? how to send this point to the tonearm component?
    // Maybe I need to hard code this in the constructor? seems it might not work to send it here
    // What logic below, if any, should be in the tonearm component?
   
    // set turntable radii
    float turntableRadius = turntableCentre.getX() - margin;
    float turntableInnerRadius = turntableRadius * 0.35;

    // draw the turntable
    g.drawEllipse(turntableCentre.getX() - turntableRadius,
                  turntableCentre.getY() - turntableRadius,
                  turntableRadius * 2, 
                  turntableRadius * 2, 
                  1);
    g.drawEllipse(turntableCentre.getX() - turntableInnerRadius,
                  turntableCentre.getY() - turntableInnerRadius,
                  turntableInnerRadius * 2,
                  turntableInnerRadius * 2,
                  1);

    // get the needle start and stop points on the turntable
    float needleTrackAngle = 35 * (juce::MathConstants<float>::pi / 180);
    tonearmNeedleStart = turntableCentre.getPointOnCircumference(turntableRadius,       
        needleTrackAngle);
    tonearmNeedleEnd = turntableCentre.getPointOnCircumference(turntableInnerRadius, 
        needleTrackAngle);

    // get the angle difference between start and stop positions
    float startPositionAngle = tonearmBase.getAngleToPoint(tonearmNeedleStart);
    float endPositionAngle = tonearmBase.getAngleToPoint(tonearmNeedleEnd);
    float angleDifference = endPositionAngle - startPositionAngle;


    // if track is playing, update the tonearm needle
    if (position >= 0)
    {
        // calculate the new tonearm angle proportionally to position in track
        float angleProgress = angleDifference * position;
        float currentAngle = startPositionAngle + angleProgress;

        // send the new angle to the tonearm to redraw its needle position
        // toneArm.updateNeedlePosition(currentAngle);
    }
}

void PositionDisplay::resized()
{
    // user parent container size for the tonearm component, to overlay
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