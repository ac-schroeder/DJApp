/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Aug 2021 12:57:58pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MainLookAndFeel.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "FrequencyShelfFilter.h"
#include "TurntableDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    /** Constructor */
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    /** Destructor */
    ~DeckGUI() override;

    /** Implements Component: Paints the DeckGUI component. */
    void paint (juce::Graphics&) override;
    /** Implements Component: Repaints the DeckGUI on resize. */
    void resized() override;

    /** Implements Button::Listener: Processes button clicks. */
    void buttonClicked(juce::Button* button) override;

    /** Implement Slider::Listener: Processes slider value changes. */
    void sliderValueChanged(juce::Slider* slider) override;

    /** Implements FileDragAndDrop: Registers when files are being dragged. */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    /** Implements FileDragAndDrop: Processes files dropped into the component. */
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** 
     * Implements Timer. Processes the timer callback to update the display 
     * on Waveform and Turntable components.
     */
    void timerCallback() override;

    /** 
     * Loads an audio URL to the deck's player. 
     *
     * @param audioURL - The URL of the audio file being loaded.
     * @param fileName - The file name of the audio file being loaded.
     */
    void loadURL(juce::URL audioURL, juce::String fileName);

private:
    // Pointer to the audio player for the deck
    DJAudioPlayer* player;

    // Custom look and feel 
    MainLookAndFeel mainLookAndFeel;

    /*========== Child Components =========*/
    // Deck title
    juce::Label trackTitle;                 
    // Start/stop/pause button block
    juce::GroupComponent startStopControls;
    juce::ImageButton playButton;
    juce::ImageButton pauseButton;
    juce::ImageButton stopButton;
    // Volume slider component block
    juce::GroupComponent volumeControls;
    juce::Slider volumeSlider{ juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::NoTextBox };
    juce::Label volumeSliderLabel;
    // Turntable display 
    TurntableDisplay turntableDisplay;
    // Frequency shelf sliders block
    juce::GroupComponent frequencyControls;
    FrequencyShelfFilter frequencyShelfFilter{ player };
    // Playback manipulation sliders block
    juce::GroupComponent playbackControls;
    juce::Slider speedSlider;
    juce::Label speedSliderLabel;
    juce::Slider positionSlider;
    juce::Label positionSliderLabel;
    // Waveform display component
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
