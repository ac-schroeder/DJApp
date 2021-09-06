/*
  ==============================================================================

    WaveformDisplay.h
    Created: 5 Aug 2021 1:33:56pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
{
public:
    /** Constructor */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    /** Destructor */
    ~WaveformDisplay() override;

    /** Implements Component: Paints the component. */
    void paint(juce::Graphics&) override;
    /** Implements Component: Repaints the component on resize. */
    void resized() override;

    /** 
     * Implements ChangeListener: Detects broadcasts from AudioThumbnail 
     * in order to update the waveform display. 
     */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /** 
     * Loads the audio file by setting it as source for the AudioThumbnail.
     *
     * @param audioURL - The URL of the audio file to display.
     */
    void loadURL(juce::URL audioURL);

    /** 
     * Sets the relative position of the playhead, as a percentage of track length. 
     * This is called by DeckGUI::timerCallback() to sync the waveform display
     * with the audio playback.
     *
     * @param _relativePosition - The position of the playhead as a percentage of
     *     the track length.
     */
    void setPositionRelative(double  _relativePosition);

private:
    // Audio thumbnail for displaying waveform
    juce::AudioThumbnail audioThumb;
    // Flag to decide whether to paint the waveform to the component
    bool fileLoaded;
    // Playhead relative position
    double relativePosition{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
