/*
  ==============================================================================

    FrequencyShelfFilter.cpp
    Created: 1 Sep 2021 9:43:48am
    Author:  Alana Schroeder

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FrequencyShelfFilter.h"

//==============================================================================
FrequencyShelfFilter::FrequencyShelfFilter(DJAudioPlayer* _player)
    : player {_player}
{
    // Add high shelf components 
    addAndMakeVisible(highShelfLabel);          // shelf label
    addAndMakeVisible(highShelfSliders);        // shelf sliders group component
    addAndMakeVisible(highFrequencyLabel);      // frequency slider label
    addAndMakeVisible(highFrequencySlider);     // frequency slider
    addAndMakeVisible(highGainLabel);           // gain slider label
    addAndMakeVisible(highGainSlider);          // gain slider
    addAndMakeVisible(highQLabel);              // Q slider label
    addAndMakeVisible(highQSlider);             // Q slider
    // Add low shelf components
    addAndMakeVisible(lowShelfLabel);           // shelf label
    addAndMakeVisible(lowShelfSliders);         // shelf sliders group component
    addAndMakeVisible(lowFrequencyLabel);       // frequency slider label
    addAndMakeVisible(lowFrequencySlider);      // frequency slider
    addAndMakeVisible(lowGainLabel);            // gain slider label
    addAndMakeVisible(lowGainSlider);           // gain slider
    addAndMakeVisible(lowQLabel);               // Q slider label
    addAndMakeVisible(lowQSlider);              // Q slider

    // Set up slider values, ranges, and names
    setUpSliders();

    // Set up slider labels
    setUpSliderLabels();

    // Add listeners
    highFrequencySlider.addListener(this);
    highGainSlider.addListener(this);
    highQSlider.addListener(this);
    lowFrequencySlider.addListener(this);
    lowGainSlider.addListener(this);
    lowQSlider.addListener(this);
}

FrequencyShelfFilter::~FrequencyShelfFilter()
{
}

void FrequencyShelfFilter::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
}

// Divides the area into groups of sliders for shelves.
// Bases dimensions on the bounds set by the parent component.
void FrequencyShelfFilter::resized()
{
    // Total component area
    auto area = getLocalBounds();

    // Label and slider dimensions
    auto shelfHeight = area.getHeight() / 2;
    auto shelfSlidersGroupWidth = area.getWidth() * 0.75;
    auto sliderHeight = shelfHeight * 0.8;
    auto sliderWidth = shelfSlidersGroupWidth / 3;

    // Shelf areas
    auto highShelfArea = area.removeFromTop(shelfHeight);
    auto lowShelfArea = area.removeFromTop(shelfHeight);
    // Shelf slider group areas
    auto highShelfSlidersArea = highShelfArea.removeFromRight(shelfSlidersGroupWidth);
    auto lowShelfSlidersArea = lowShelfArea.removeFromRight(shelfSlidersGroupWidth);

    // Set shelf slider group component bounds
    highShelfSliders.setBounds(highShelfSlidersArea);
    lowShelfSliders.setBounds(lowShelfSlidersArea);

    // Set individual slider component bounds
    highFrequencySlider.setBounds(highShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
    highGainSlider.setBounds(highShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
    highQSlider.setBounds(highShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
    lowFrequencySlider.setBounds(lowShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
    lowGainSlider.setBounds(lowShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
    lowQSlider.setBounds(lowShelfSlidersArea.removeFromLeft(sliderWidth).removeFromBottom(sliderHeight));
}

// Gathers slider values for each shelf set and sends results 
// to the player to filter the audio sources.
void FrequencyShelfFilter::sliderValueChanged(juce::Slider* slider)
{
    double frequency{};
    float gain{};
    double q{};

    // Whenever a high shelf slider value changes, package up all the high
    // shelf slider values and send to the player to set the high shelf filter.
    if (slider->getName() == "highShelf")
    {
        frequency = highFrequencySlider.getValue();
        gain = highGainSlider.getValue();
        q = highQSlider.getValue();
        player->setHighShelf(frequency, gain, q);
    }

    // Whenever a low shelf slider value changes, package up all the low
    // shelf slider values and send to the player to set the low shelf filter.
    if (slider->getName() == "lowShelf")
    {
        frequency = lowFrequencySlider.getValue();
        gain = lowGainSlider.getValue();
        q = lowQSlider.getValue();
        player->setLowShelf(frequency, gain, q);
    }
}


void FrequencyShelfFilter::setUpSliders()
{
    // Set slider names
    // These are used to call correct shelf functions based on slider type
    highFrequencySlider.setName("highShelf");
    highGainSlider.setName("highShelf");
    highQSlider.setName("highShelf");
    lowFrequencySlider.setName("lowShelf");
    lowGainSlider.setName("lowShelf");
    lowQSlider.setName("lowShelf");

    // Set high shelf slider ranges
    highFrequencySlider.setRange(10000, 20000, 10);     
    highGainSlider.setRange(0.01f, 1.00f, 0.01f);       
    highQSlider.setRange(0.1, 40.0, 0.1);
    // Set low shelf slider ranges
    lowFrequencySlider.setRange(1, 250, 1);
    lowGainSlider.setRange(0.01f, 1.0f, 0.01f);    
    lowQSlider.setRange(0.1, 40, 0.1);

    // Set high shelf slider starting values
    highFrequencySlider.setValue(15000);
    highGainSlider.setValue(1.0);
    highQSlider.setValue(1);
    // Set low shelf slider starting values
    lowFrequencySlider.setValue(20);
    lowGainSlider.setValue(1.0);
    lowQSlider.setValue(1.0);
}

void FrequencyShelfFilter::setUpSliderLabels()
{
    // Attach high shelf labels
    highShelfLabel.attachToComponent(&highShelfSliders, true);
    highFrequencyLabel.attachToComponent(&highFrequencySlider, false);
    highGainLabel.attachToComponent(&highGainSlider, false);
    highQLabel.attachToComponent(&highQSlider, false);
    // Attach low shelf labels
    lowShelfLabel.attachToComponent(&lowShelfSliders, true);
    lowFrequencyLabel.attachToComponent(&lowFrequencySlider, false);
    lowGainLabel.attachToComponent(&lowGainSlider, false);
    lowQLabel.attachToComponent(&lowQSlider, false);

    // Add high shelf label texts
    highShelfLabel.setText("High Shelf", juce::dontSendNotification);
    highFrequencyLabel.setText("Hz", juce::dontSendNotification);
    highGainLabel.setText("Gain", juce::dontSendNotification);
    highQLabel.setText("Q", juce::dontSendNotification);
    // Add low shelf label texts
    lowShelfLabel.setText("Low Shelf", juce::dontSendNotification);
    lowFrequencyLabel.setText("Hz", juce::dontSendNotification);
    lowGainLabel.setText("Gain", juce::dontSendNotification);
    lowQLabel.setText("Q", juce::dontSendNotification);
}