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

    // child component container blocks
    juce::Label header;
    juce::

    // button components
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    // slider components
    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;
    juce::Slider lowShelfGainSlider;
    juce::Slider highShelfGainSlider;

    // custom turntable display component
    TurntableDisplay turntableDisplay{ player->getTransportSource() };
    // waveform display component
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
