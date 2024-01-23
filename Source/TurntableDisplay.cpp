#include <cmath>
#include <JuceHeader.h>
#include "TurntableDisplay.h"


TurntableDisplay::TurntableDisplay()
{
}

TurntableDisplay::~TurntableDisplay()
{
}

// Draws the turntable using the component's graphics object
// Dimension values are set in resized(), so that the component can 
// be drawn at any size.
void TurntableDisplay::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    g.setColour (juce::Colours::grey);

    // Draw the turntable
    g.setColour(juce::Colours::black);
    g.fillEllipse(turntableCentre.getX() - turntableRadius,       // outer circle
        turntableCentre.getY() - turntableRadius,
        turntableRadius * 2,
        turntableRadius * 2);
    g.setColour(juce::Colours::silver);
    g.fillEllipse(turntableCentre.getX() - turntableInnerRadius,  // inner circle
        turntableCentre.getY() - turntableInnerRadius,
        turntableInnerRadius * 2,
        turntableInnerRadius * 2);

    // Draw the tone arm base
    g.setColour(juce::Colours::dimgrey);
    g.fillEllipse(toneArmBase.getX() - toneArmBaseRadius,         // outer circle
        toneArmBase.getY() - toneArmBaseRadius,
        toneArmBaseRadius * 2,
        toneArmBaseRadius * 2);
    g.setColour(juce::Colours::grey);
    g.fillEllipse(toneArmBase.getX() - (toneArmBaseRadius * 0.5), // inner circle
        toneArmBase.getY() - (toneArmBaseRadius * 0.5),
        toneArmBaseRadius,
        toneArmBaseRadius);

    // Update the needle position to the correct point in the track
    updateNeedlePosition();

    // Draw the tone arm needle
    g.setColour(juce::Colours::grey);
    juce::Line<float> needle{ toneArmElbow.getX(),
        toneArmElbow.getY(),
        toneArmNeedle.getX(),
        toneArmNeedle.getY() };
    g.drawLine(needle, 1);

    // Draw the tone arm
    juce::Line<float> lowerArm = needle.withShortenedEnd(10);
    juce::Path arm;
    arm.addLineSegment(juce::Line<float> { toneArmBase.getX(),
        toneArmBase.getY(),
        toneArmElbow.getX(),
        toneArmElbow.getY() }, 1);
    arm.addLineSegment(lowerArm, 1);
    g.strokePath(arm, juce::PathStrokeType{ 3, juce::PathStrokeType::JointStyle::beveled });
}

// Sets dimensions for the drawing based on container size
void TurntableDisplay::resized()
{
    // Set layout variables based on component size
    componentSize = getWidth();
    margin = componentSize * 0.1;

    // Set up turntable center point and radii
    turntableCentre.setXY(componentSize / 2, componentSize / 2);
    turntableRadius = turntableCentre.getX() - margin;
    turntableInnerRadius = turntableRadius * 0.35;

    // Get the needle track start and stop points on the turntable
    float trackAngle = needleTrackAngle * (juce::MathConstants<float>::pi / 180);
    needleTrackStart = turntableCentre.getPointOnCircumference(turntableRadius,
        trackAngle);
    needleTrackEnd = turntableCentre.getPointOnCircumference(turntableInnerRadius,
        trackAngle);

    // Get the tone arm size and position relative to the turntable
    toneArmBase.setXY(componentSize - margin, margin * 1.8);
    toneArmBaseRadius = turntableRadius * 0.2;
    toneArmDistance = toneArmBase.getDistanceFrom(needleTrackStart);

    // Set start and stop position angles for the tone arm on the needle track
    startPositionAngle = toneArmBase.getAngleToPoint(needleTrackStart);
    endPositionAngle = toneArmBase.getAngleToPoint(needleTrackEnd);
    rotationRange = endPositionAngle - startPositionAngle;

    // Set the tone arm needle position to the correct place on the track
    updateNeedlePosition();
}


void TurntableDisplay::setPositionRelative(double _relativePosition)
{
    // Only change if there is change to show
    if (_relativePosition != relativePosition)
    {
        // Update the relative position
        relativePosition = _relativePosition;
        // Redraw the turntable
        repaint();
    }
}

// Uses the relative position to recalculate the angle of the tone arm
// and get the new Point coordinates for the needle.
void TurntableDisplay::updateNeedlePosition()
{
    // Calculate the new needle and elbow angles
    float needleAngle = startPositionAngle + (rotationRange * relativePosition);
    float elbowAngle = needleAngle - (11 * (juce::MathConstants<float>::pi / 180));

    // Calculate and update the new needle and elbow point coordinates
    toneArmNeedle = toneArmBase.getPointOnCircumference(toneArmDistance, needleAngle);
    toneArmElbow = toneArmBase.getPointOnCircumference(toneArmDistance * 0.63, elbowAngle);
}