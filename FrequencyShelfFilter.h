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
    /** 
     * Constructor 
     *
     * @param _player - A pointer to the player for the deck this component
     *      belongs to. This component sends frequency shelf coefficent data 
     *      to the player to modify playback.
     */
    FrequencyShelfFilter(DJAudioPlayer* _player);

    /** Destructor */
    ~FrequencyShelfFilter() override;

    /**
     * Implements Component: Draws the component's content, as well as any child
     * components' content.
     *
     * @param g - The graphics context of the component.
     */
    void paint (juce::Graphics&) override;

    /**
     * Implements Component: Called when the component is resized. Used to
     * control layout of child components.
     */
    void resized() override;

     /**
      * Implements Slider::Listener: Processes slider value changes.
      * Sends frequency shelf coeeficient values to the player for filtering.
      *
      * @param slider - Pointer to the slider that triggered the event.
      */
    void sliderValueChanged(juce::Slider* slider) override;

private:
    /** Sets up slider ranges, values, and component names. */
    void setUpSliders();
    /** Sets up slider labels for the sliders. */
    void setUpSliderLabels();

    // Pointer to the player, used for setting shelf filter coefficients
    DJAudioPlayer* player;

    /*========== CHILD COMPONENTS =========*/
    // Main group labels
    juce::Label highShelfLabel;
    juce::Label lowShelfLabel;
    // High shelf filter group sliders and labels
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
    // Low shelf filter group sliders and labels
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
