/*
  ==============================================================================

    DeckGUI.h
    Created: 3 Aug 2021 12:57:58pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
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
    void loadURL(juce::URL audioURL, juce::String trackTitle);

private:
    // pointer to audio player
    DJAudioPlayer* player;

    // header component to display track title
    juce::Label header;

    // start, stop, and play button block
    juce::TextButton startStopControls;
    juce::TextButton playButton{ "P" };
    juce::TextButton pauseButton{ "PS" };
    juce::TextButton stopButton{ "S" };

    // volume control component
    juce::Slider gainSlider;

    // turntable display component
    TurntableDisplay turntableDisplay{ player->getTransportSource() };

    // frequency shelf manipulation sliders block
    juce::GroupComponent frequencyControls;
    juce::Slider lowShelfGainSlider;
    juce::Slider highShelfGainSlider;

    // playback manipulation sliders block
    juce::GroupComponent playbackControls;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    // waveform display component
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
