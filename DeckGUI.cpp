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
    // set up components
    header.setText("Load a track below to get started...", juce::NotificationType::dontSendNotification);

    // add components
    addAndMakeVisible(header);
    addAndMakeVisible(startStopControls);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(turntableDisplay);
    addAndMakeVisible(frequencyControls);
    addAndMakeVisible(lowShelfGainSlider);
    addAndMakeVisible(highShelfGainSlider);
    addAndMakeVisible(playbackControls);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(waveformDisplay);

    // add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
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

    // start the timer to coordinate audio playback and waveform and turntable displays
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

}

void DeckGUI::resized()
{
    auto area = getLocalBounds();

    /*==== Block Section Bounds ====*/

    // header section
    header.setBounds(area.removeFromTop(36));

    // waveform display section
    waveformDisplay.setBounds(area.removeFromBottom(area.getHeight() / 5));

    // playback controls section
    auto playbackControlsArea = area.removeFromBottom(area.getHeight() / 5);
    playbackControls.setBounds(playbackControlsArea);

    // frequency controls section
    auto frequencyControlsArea = area.removeFromRight(area.getWidth() * 0.6);
    frequencyControls.setBounds(frequencyControlsArea);

    // start-stop controls section
    auto startStopControlsArea = area.removeFromTop(area.getHeight() * 0.23);
    startStopControls.setBounds(startStopControlsArea);

    // volume slider
    gainSlider.setBounds(area.removeFromTop(area.getHeight() / 4));

    // turntable display - set to be square and centered horizontally
    auto turntableDimension = juce::jmin(area.getHeight(), area.getWidth());
    auto turntableX = area.getX() + ((area.getWidth() - turntableDimension) / 2);
    turntableDisplay.setBounds(turntableX, area.getY(), turntableDimension, turntableDimension);

    /*=== Section Subcomponent Bounds ===*/

    // start-stop buttons
    auto buttonHeight = 40;
    auto buttonWidth = 40;
    playButton.setBounds(startStopControlsArea.removeFromLeft(buttonWidth).reduced(20));
    pauseButton.setBounds(startStopControlsArea.removeFromLeft(buttonWidth).reduced(20));
    stopButton.setBounds(startStopControlsArea.removeFromLeft(buttonWidth).reduced(20));
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
        juce::File file { files[0] };
        juce::String fileName = file.getFileName();
        loadURL(juce::URL(file), fileName);
    }
}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    turntableDisplay.setPositionRelative(player->getPositionRelative());
}


/** Loads an audio URL to the deck's player */
void DeckGUI::loadURL(juce::URL audioURL, juce::String trackTitle)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

    // set the track title in the header
    header.setText(trackTitle, juce::NotificationType::dontSendNotification);
}