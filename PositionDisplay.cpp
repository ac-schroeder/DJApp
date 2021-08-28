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
    componentSize = getWidth();
    margin = componentSize * 0.1;

    // set up turntable center point and radii
    turntableCentre.setXY(getWidth() / 2, getHeight() / 2);
    float turntableRadius = turntableCentre.getX() - margin;
    float turntableInnerRadius = turntableRadius * 0.35;

    // set up the tone arm base point and base radii
    float toneArmBaseRadius = turntableRadius * 0.25;
    toneArmBase.setXY(componentSize - margin, margin + 12);
    toneArm.setBasePosition(toneArmBase, toneArmBaseRadius);


    // What logic below, if any, should be in the tone arm component?


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
    toneArmNeedleStart = turntableCentre.getPointOnCircumference(turntableRadius,       
        needleTrackAngle);
    toneArmNeedleEnd = turntableCentre.getPointOnCircumference(turntableInnerRadius, 
        needleTrackAngle);

    // get the angle difference between start and stop positions
    float startPositionAngle = toneArmBase.getAngleToPoint(toneArmNeedleStart);
    float endPositionAngle = toneArmBase.getAngleToPoint(toneArmNeedleEnd);
    float angleDifference = endPositionAngle - startPositionAngle;


    // if track is playing, update the tone arm needle
    if (position >= 0)
    {
        // calculate the new tone arm angle proportionally to position in track
        float angleProgress = angleDifference * position;
        float currentAngle = startPositionAngle + angleProgress;

        // send the new angle to the tone arm to redraw its needle position
        // toneArm.updateNeedlePosition(currentAngle);
    }
}

void PositionDisplay::resized()
{
    // user parent container size for the tone arm component, to overlay
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