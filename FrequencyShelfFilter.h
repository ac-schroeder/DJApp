/*
  ==============================================================================

    FrequencyShelfFilter.h
    Created: 1 Sep 2021 9:43:48am
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class FrequencyShelfFilter  : public juce::Component,
                              public juce::Slider::Listener
{
public:
    FrequencyShelfFilter(DJAudioPlayer* _player);
    ~FrequencyShelfFilter() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

private:
    juce::Label highShelfLabel;
    juce::Label lowShelfLabel;
    // high shelf filter sliders
    juce::GroupComponent highShelfSliders;
    juce::Label highFrequencyLabel;
    juce::Label highGainLabel;
    juce::Label highQLabel;
    juce::Slider highFrequencySlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };
    juce::Slider highGainSlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };
    juce::Slider highQSlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };
    // low shelf filter sliders
    juce::GroupComponent lowShelfSliders;
    juce::Label lowFrequencyLabel;
    juce::Label lowGainLabel;
    juce::Label lowQLabel;
    juce::Slider lowFrequencySlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };
    juce::Slider lowGainSlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };
    juce::Slider lowQSlider{ juce::Slider::LinearVertical, juce::Slider::TextBoxAbove };

    // pointer to the player
    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyShelfFilter)
};
