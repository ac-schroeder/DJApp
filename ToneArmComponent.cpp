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
    g.drawEllipse(base_x, base_y, 28, 28, 1);
    

    // draw the needle position
    g.setColour(juce::Colours::lightgoldenrodyellow);
    g.drawEllipse(needlePosition_x, needlePosition_y, 1, 1, 1);


}

void ToneArmComponent::resized()
{

}


void ToneArmComponent::setBasePosition(int x, int y)
{
    base_x = x;
    base_y = y;
}

void ToneArmComponent::updateNeedlePosition(float x, float y)
{
    needlePosition_x = x;
    needlePosition_y = y;
}