/*
  ==============================================================================

    WaveformDisplay.h
    Created: 5 Aug 2021 1:33:56pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    /** Implement Component */
    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement ChangeListener: Detect broadcasts from AudioThumbnail in order to update the waveform display */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /** Load the audio file by setting as source for the AudioThumbnail */
    void loadURL(juce::URL audioURL);

private:
    // audio thumbnail for displaying waveform; initialised in WaveformDisplay initialiser list
    // note to me: AudioThumbnail's constructor requires parameters. the only reason we can declare this here without instantiating it is because in the constructor initialiser list for WaveformDisplay, we initialize this object with its parameters!
    juce::AudioThumbnail audioThumb;
    // flag to decide whether to paint the waveform to the component
    bool fileLoaded;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
