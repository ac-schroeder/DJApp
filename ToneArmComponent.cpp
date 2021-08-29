/*
  ==============================================================================

    ToneArm.cpp
    Created: 27 Aug 2021 10:52:22am
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ToneArmComponent.h"

//==============================================================================
ToneArmComponent::ToneArmComponent()
{
}

ToneArmComponent::~ToneArmComponent()
{
}

void ToneArmComponent::paint (juce::Graphics& g)
{

    g.setColour (juce::Colours::grey);

    // draw the tonearm base
    g.drawEllipse(toneArmBase.getX() - baseRadius,          // outer circle
                  toneArmBase.getY() - baseRadius, 
                  baseRadius * 2, 
                  baseRadius * 2, 
                  1);
    g.drawEllipse(toneArmBase.getX() - (baseRadius * 0.5),  // inner circle
                  toneArmBase.getY() - (baseRadius * 0.5), 
                  baseRadius,
                  baseRadius,
                  1);

    // draw the tonearm arm
    juce::Path arm;
    arm.addLineSegment(juce::Line<float> { toneArmBase.getX(),
                                           toneArmBase.getY(),
                                           elbowPosition.getX(),
                                           elbowPosition.getY() }, 1);
    arm.addLineSegment(juce::Line<float> { elbowPosition.getX(),
                                           elbowPosition.getY(),
                                           needlePosition.getX(),
                                           needlePosition.getY() }, 1);
    g.strokePath(arm, juce::PathStrokeType{ 4 });
}

void ToneArmComponent::resized()
{

}


void ToneArmComponent::setPositionAndSize(juce::Point<float>& _toneArmBase,
                                          juce::Point<float>& toneArmNeedleStart,
                                          float _baseRadius)
{
    // store the base point
    toneArmBase = _toneArmBase;
    // store the base radius
    baseRadius = _baseRadius;
    // calculate and store the arm distance
    armDistance = toneArmBase.getDistanceFrom(toneArmNeedleStart);
}

void ToneArmComponent::updateNeedlePosition(float currentAngle)
{
    // update the needle position by rotating the arm by the current angle
    needlePosition = toneArmBase.getPointOnCircumference(armDistance, currentAngle);
    float elbowAngle = currentAngle - (11 * (juce::MathConstants<float>::pi / 180));
    elbowPosition = toneArmBase.getPointOnCircumference(armDistance * 0.63, elbowAngle);
}