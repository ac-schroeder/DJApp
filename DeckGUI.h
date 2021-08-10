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

private:
    // button components
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };

    // slider components
    juce::Slider gainSlider;
    //juce::Slider pitchSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    // pointer to audio player
    DJAudioPlayer* player;

    // waveform display component; initialised in DeckGUI initialiser list
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
