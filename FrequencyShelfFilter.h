/*
  ==============================================================================

    FrequencyShelfFilter.h
    Created: 1 Sep 2021 9:43:48am
    Author:  Alana Schroeder

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
    /** Constructor */
    FrequencyShelfFilter(DJAudioPlayer* _player);
    /** Destructor */
    ~FrequencyShelfFilter() override;

    /** Implements Component: Paints the component. */
    void paint (juce::Graphics&) override;
    /** Implements Component: Repaints the component on resize. */
    void resized() override;

    /** Implements Slider::Listener: Processes slider value changes. */
    void sliderValueChanged(juce::Slider* slider) override;

private:
    // Pointer to the player, used for setting shelf filter coefficients
    DJAudioPlayer* player;

    /*========== Child Components =========*/
    // main group labels
    juce::Label highShelfLabel;
    juce::Label lowShelfLabel;
    // high shelf filter group sliders and labels
    juce::GroupComponent highShelfSliders;
    juce::Label highFrequencyLabel;
    juce::Label highGainLabel;
    juce::Label highQLabel;
    juce::Slider highFrequencySlider{ juce::Slider::LinearVertical, 
                                      juce::Slider::TextBoxAbove };
    juce::Slider highGainSlider{ juce::Slider::LinearVertical, 
                                 juce::Slider::TextBoxAbove };
    juce::Slider highQSlider{ juce::Slider::LinearVertical, 
                              juce::Slider::TextBoxAbove };
    // low shelf filter group sliders and labels
    juce::GroupComponent lowShelfSliders;
    juce::Label lowFrequencyLabel;
    juce::Label lowGainLabel;
    juce::Label lowQLabel;
    juce::Slider lowFrequencySlider{ juce::Slider::LinearVertical, 
                                     juce::Slider::TextBoxAbove };
    juce::Slider lowGainSlider{ juce::Slider::LinearVertical, 
                                juce::Slider::TextBoxAbove };
    juce::Slider lowQSlider{ juce::Slider::LinearVertical, 
                             juce::Slider::TextBoxAbove };


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FrequencyShelfFilter)
};
