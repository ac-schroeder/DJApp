/*
  ==============================================================================

    DeckGUI.cpp
    Created: 3 Aug 2021 12:57:58pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse) 
    : player{ _player },
      waveformDisplay{          // WaveformDisplay component
        formatManagerToUse,       // AudioFormatManager: to send to AudioThumbnail
        cacheToUse}               // AudioThumbnailCache: to send to AudioThumbnail
{
    // make button components visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);

    // make slider components visible
    /*addAndMakeVisible(gainSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(lowShelfGainSlider);
    addAndMakeVisible(highShelfGainSlider);*/

    // make position display visible
    addAndMakeVisible(positionDisplay);

    // make waveform display visible
    addAndMakeVisible(waveformDisplay);

    // add button listeners
    playButton.addListener(this);
    stopButton.addListener(this);

    // add slider listeners
    gainSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    lowShelfGainSlider.addListener(this);
    highShelfGainSlider.addListener(this);
    
    // restrict slider ranges
    gainSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    positionSlider.setRange(0.0, 1.0);
    lowShelfGainSlider.setRange(0.01, 1.0);
    highShelfGainSlider.setRange(0.01, 1.0);

    // start the timer to coordinate audio playback and waveform display
    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("DeckGUI", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowHeight = getHeight() / 9;

    // set resizing bounds on components
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    //gainSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
    //speedSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    //positionSlider.setBounds(0, rowHeight * 4, getWidth(), rowHeight);
    //lowShelfGainSlider.setBounds(0, rowHeight * 5, getWidth(), rowHeight);
    //highShelfGainSlider.setBounds(0, rowHeight * 6, getWidth(), rowHeight);
    positionDisplay.setBounds(0, rowHeight * 2, getWidth() / 2, rowHeight * 5);
    waveformDisplay.setBounds(0, rowHeight * 7, getWidth(), rowHeight * 2);
}

/** Implement Button::Listener */
void DeckGUI::buttonClicked(juce::Button* button)
{
    // process responses for each button
    if (button == &playButton)
    {
        DBG("DeckGUI::buttonClicked: Play Button was clicked");
        // begin playing
        player->start();
    }
    if (button == &stopButton)
    {
        DBG("DeckGUI::buttonClicked: Stop Button was clicked");
        // stop playing
        player->stop();
    }
}

/** Implement Slider::Listener */
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    // implement gain slider
    if (slider == &gainSlider)
    {
        DBG("DeckGUI::sliderValueChanged: gainSlider " + std::to_string(slider->getValue()));
        player->setGain(slider->getValue());
    }
    // implement speed effects
    if (slider == &speedSlider)
    {
        DBG("DeckGUI::sliderValueChanged: speedSlider " + std::to_string(slider->getValue()));
        auto speed = slider->getValue();
        if (speed != 0)
        {
            player->setSpeed(speed);
        }
    }
    // implement position slider effects
    if (slider == &positionSlider)
    {
        DBG("DeckGUI::sliderValueChanged: positionSlider " + std::to_string(slider->getValue()));
        player->setPositionRelative(slider->getValue());
    }
    // implement low shelf gain slider
    if (slider == &lowShelfGainSlider)
    {
        player->setLowShelf(slider->getValue());
    }
    // implement high shelf gain slider
    if (slider == &highShelfGainSlider)
    {
        player->setHighShelf(slider->getValue());
    }
}

/** Implement FileDragAndDrop */
bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        loadURL(juce::URL(juce::File{ files[0] }));
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}


/** Loads an audio URL to the deck's player */
void DeckGUI::loadURL(juce::URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}