/*
  ==============================================================================

    TurntableDisplay.cpp
    Created: 25 Aug 2021 3:29:58pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#include <cmath>
#include <JuceHeader.h>
#include "TurntableDisplay.h"

//==============================================================================
// TODO: Take this param for the change listerner out! Unless... ?
TurntableDisplay::TurntableDisplay()
{
}

TurntableDisplay::~TurntableDisplay()
{
}

void TurntableDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    g.setColour (juce::Colours::grey);

    // draw the turntable
    g.setColour(juce::Colours::black);
    g.fillEllipse(turntableCentre.getX() - turntableRadius,         // outer circle
        turntableCentre.getY() - turntableRadius,
        turntableRadius * 2,
        turntableRadius * 2);
    g.setColour(juce::Colours::silver);
    g.fillEllipse(turntableCentre.getX() - turntableInnerRadius,    // inner circle
        turntableCentre.getY() - turntableInnerRadius,
        turntableInnerRadius * 2,
        turntableInnerRadius * 2);

    // draw the tone arm base
    g.setColour(juce::Colours::dimgrey);
    g.fillEllipse(toneArmBase.getX() - toneArmBaseRadius,           // outer circle
        toneArmBase.getY() - toneArmBaseRadius,
        toneArmBaseRadius * 2,
        toneArmBaseRadius * 2);
    g.setColour(juce::Colours::grey);
    g.fillEllipse(toneArmBase.getX() - (toneArmBaseRadius * 0.5),   // inner circle
        toneArmBase.getY() - (toneArmBaseRadius * 0.5),
        toneArmBaseRadius,
        toneArmBaseRadius);

    // update the needle position to the correct position in the track
    updateNeedlePosition();

    // draw the tone arm needle
    g.setColour(juce::Colours::grey);
    juce::Line<float> needle{ toneArmElbow.getX(),
        toneArmElbow.getY(),
        toneArmNeedle.getX(),
        toneArmNeedle.getY() };
    g.drawLine(needle, 1);

    // draw the tone arm
    juce::Line<float> lowerArm = needle.withShortenedEnd(10);
    juce::Path arm;
    arm.addLineSegment(juce::Line<float> { toneArmBase.getX(),
        toneArmBase.getY(),
        toneArmElbow.getX(),
        toneArmElbow.getY() }, 1);
    arm.addLineSegment(lowerArm, 1);
    g.strokePath(arm, juce::PathStrokeType{ 3, juce::PathStrokeType::JointStyle::beveled });
}

void TurntableDisplay::resized()
{
    // set layout variables based on size
    componentSize = getWidth();
    margin = componentSize * 0.1;

    // set up turntable center point and radii
    turntableCentre.setXY(componentSize / 2, componentSize / 2);
    turntableRadius = turntableCentre.getX() - margin;
    turntableInnerRadius = turntableRadius * 0.35;

    // get the needle track start and stop points on the turntable
    float trackAngle = needleTrackAngle * (juce::MathConstants<float>::pi / 180);
    needleTrackStart = turntableCentre.getPointOnCircumference(turntableRadius,
        trackAngle);
    needleTrackEnd = turntableCentre.getPointOnCircumference(turntableInnerRadius,
        trackAngle);

    // get the tone arm size and position relative to the turntable
    toneArmBase.setXY(componentSize - margin, margin * 1.8);
    toneArmBaseRadius = turntableRadius * 0.2;
    toneArmDistance = toneArmBase.getDistanceFrom(needleTrackStart);

    // set start and stop position angles for the tone arm on the needle track
    startPositionAngle = toneArmBase.getAngleToPoint(needleTrackStart);
    endPositionAngle = toneArmBase.getAngleToPoint(needleTrackEnd);
    rotationRange = endPositionAngle - startPositionAngle;

    // set the tone arm needle position to the correct place on the track
    updateNeedlePosition();
}

void TurntableDisplay::setPositionRelative(double _relativePosition)
{
    // only change position if it's changed
    if (_relativePosition != relativePosition)
    {
        relativePosition = _relativePosition;
        repaint();
    }
}

void TurntableDisplay::updateNeedlePosition()
{
    float needleAngle = startPositionAngle + (rotationRange * relativePosition);
    float elbowAngle = needleAngle - (11 * (juce::MathConstants<float>::pi / 180));
    toneArmNeedle = toneArmBase.getPointOnCircumference(toneArmDistance, needleAngle);
    toneArmElbow = toneArmBase.getPointOnCircumference(toneArmDistance * 0.63, elbowAngle);
}