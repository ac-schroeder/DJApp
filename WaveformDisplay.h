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

    /** Implement ChangeListener */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL(juce::URL audioURL);

private:
    // audio thumbnail for displaying waveform
    juce::AudioThumbnail audioThumb;
    // 
    bool fileLoaded;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
