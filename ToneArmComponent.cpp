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
    //g.drawLine(base_x - 2, base_y + 2, needlePosition_x - 2, needlePosition_y + 2, 4);

    //// draw the needle
    ////g.fillRect(needlePosition_x)
    //

    //// draw the needle position
    //g.setColour(juce::Colours::lightgoldenrodyellow);
    //g.drawEllipse(needlePosition_x, needlePosition_y, 1, 1, 1);


}

void ToneArmComponent::resized()
{

}


void ToneArmComponent::setBasePosition(juce::Point<float>& _toneArmBase, float _baseRadius)
{
    toneArmBase = _toneArmBase;
    baseRadius = _baseRadius;
}

void ToneArmComponent::updateNeedlePosition(juce::Point<float>& _needlePosition)
{
    needlePosition = _needlePosition;
}