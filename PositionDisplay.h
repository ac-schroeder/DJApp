/*
  ==============================================================================

    PositionDisplay.h
    Created: 25 Aug 2021 3:29:58pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
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

    // flag to decide whether to paint the tone arm position to the component
    // Do this only if you need to use transport source anyway
    bool fileLoaded;
    // the relative position of the playhead
    double position{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PositionDisplay)
};
