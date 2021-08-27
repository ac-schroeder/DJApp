/*
  ==============================================================================

    PositionDisplay.h
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ToneArmComponent.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PositionDisplay  : public juce::Component,
                         public juce::ChangeListener // TODO: Take this out! Unless... ?
{
public:
    // TODO: Take param for the change listener out! Unless... ?
    PositionDisplay(juce::AudioTransportSource* _transportSource);
    ~PositionDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setPositionRelative(double _position);

    /** Implement ChangeListener: Detect broadcasts from the audioSource in order to update the waveform display */
    // TODO: Take this out! Unless... ?
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    // pointer to the transport source of the player for listening to changes in playback
    // TODO: Take this out! Unless... ?
    juce::AudioTransportSource* transportSource;

    // the relative position of the playhead
    double position{ 0 };

    // layout variables
    int componentSize;
    int margin;
    // the draw coordinates for the record player components
    int turntable_x;
    int turntable_y;
    int tonearm_x;
    int tonearm_y;

    // the record player components
    // Turntable turntable;
    ToneArmComponent toneArm;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionDisplay)
};
