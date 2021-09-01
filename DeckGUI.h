/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Aug 2021 12:57:58pm
    Author:  alana

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
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement Button::Listener */
    void buttonClicked(juce::Button* button) override;

    /** Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

    /** Implement FileDragAndDrop */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** Implement Timer */
    void timerCallback() override;

    /** Loads an audio URL to the deck's player */
    void loadURL(juce::URL audioURL, juce::String fileName);

private:
    // custom look and feel 
    MainLookAndFeel mainLookAndFeel;

    // pointer to audio player
    DJAudioPlayer* player;

    // header component to display track title
    juce::Label trackTitle;

    // start, stop, and play button block
    juce::GroupComponent startStopControls;
    juce::ImageButton playButton;
    juce::ImageButton pauseButton;
    juce::ImageButton stopButton;

    // volume slider component
    juce::GroupComponent volumeControls;
    juce::Slider volumeSlider{ juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::NoTextBox };
    juce::Label volumeSliderLabel;

    // turntable display component
    TurntableDisplay turntableDisplay;

    // frequency shelf manipulation sliders block
    juce::GroupComponent frequencyControls;
    FrequencyShelfFilter frequencyShelfFilter{ player };

    // playback manipulation sliders block
    juce::GroupComponent playbackControls;
    juce::Slider speedSlider;
    juce::Label speedSliderLabel;
    juce::Slider positionSlider;
    juce::Label positionSliderLabel;

    // waveform display component
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
