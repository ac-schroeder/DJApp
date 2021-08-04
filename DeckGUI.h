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

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener
{
public:
    DeckGUI(DJAudioPlayer* player);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement Button::Listener */
    void buttonClicked(juce::Button* button) override;

    /** Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // pointer to audio player
    DJAudioPlayer* player;

    // button and slider components
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::Slider gainSlider;
    juce::Slider pitchSlider;
    juce::Slider speedSlider;
    juce::Slider positionSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
