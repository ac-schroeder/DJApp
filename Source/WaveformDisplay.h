#pragma once

#include <JuceHeader.h>


class WaveformDisplay : public juce::Component,
                        public juce::ChangeListener
{
public:
    /** 
     * Constructor 
     *
     * @param formatManagerToUse - Reference to the shared audio format manager
     *                             for the app. Used to create an audio thumbnail
     *                             to display a waveform of the audio source.
     * @param cacheToUse         - Reference to the shared audio source thumbnail 
     *                             cache. Used to create the audio thumbnail.
     */
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                    juce::AudioThumbnailCache& cacheToUse);

    /** 
     * Destructor 
     */
    ~WaveformDisplay() override;

    /**
     * Implements Component: Draws the component's content, as well as any child
     * components' content.
     *
     * @param g - The graphics context of the component.
     */
    void paint(juce::Graphics&) override;

    /**
     * Implements Component: Called when the component is resized. Used to
     * control layout of child components.
     */
    void resized() override;

    /** 
     * Loads the audio file by setting it as source for the AudioThumbnail.
     *
     * @param audioURL - The URL of the audio file to display.
     */
    void loadURL(const juce::URL& audioURL);

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
    /**
     * Implements ChangeListener: Detects broadcasts from AudioThumbnail
     * in order to update the waveform display.
     */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    // Audio thumbnail for displaying waveform
    juce::AudioThumbnail audioThumb;
    // Flag to decide whether to paint the waveform to the component
    bool fileLoaded;
    // Playhead relative position
    double relativePosition{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
