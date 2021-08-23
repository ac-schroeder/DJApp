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
    //addAndMakeVisible(loadButton);

    // make slider components visible
    addAndMakeVisible(gainSlider);
    //addAndMakeVisible(pitchSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    // make waveform display visible
    addAndMakeVisible(waveformDisplay);

    // add button listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    //loadButton.addListener(this);

    // add slider listeners
    gainSlider.addListener(this);
    //pitchSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    
    // restrict slider ranges
    gainSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    positionSlider.setRange(0.0, 1.0);

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
    //double rowHeight = getHeight() / 9;
    double rowHeight = getHeight() / 7;

    // set resizing bounds on components
    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    gainSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
    //pitchSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    speedSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    positionSlider.setBounds(0, rowHeight * 4, getWidth(), rowHeight);
    waveformDisplay.setBounds(0, rowHeight * 5, getWidth(), rowHeight * 2);
    //loadButton.setBounds(0, rowHeight * 7, getWidth(), rowHeight);
}

/** Implement Button::Listener */
void DeckGUI::buttonClicked(juce::Button* button)
{
    // process responses for each button
    if (button == &playButton)
    {
        DBG("DeckGUI::buttonClicked: Play Button was clicked");
        // begin playing
        //player->setPosition(0);
        player->start();
        // reset dphase
        // dphase = 0;
    }
    if (button == &stopButton)
    {
        DBG("DeckGUI::buttonClicked: Stop Button was clicked");
        // stop playing
        player->stop();
    }
    //if (button == &loadButton)
    //{
    //    DBG("DeckGUI::buttonClicked: Load Button was clicked");

    //    // create a file chooser GUI for the user to select a file
    //    juce::FileChooser chooser{ "Select file..." };
    //    // if the user selects a file to open, load the file
    //    if (chooser.browseForFileToOpen())
    //    {
    //        // convert the chosen file to a URL and load it
    //        auto audioURL = juce::URL{ chooser.getResult() };
    //        //player->loadURL(fileURL);
    //        //waveformDisplay.loadURL(fileURL);
    //        loadURL(audioURL);
    //    }
    //}
}

/** Implement Slider::Listener */
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        DBG("DeckGUI::sliderValueChanged: gainSlider " + std::to_string(slider->getValue()));
        player->setGain(slider->getValue());
    }
    // implement pitch slider effects
    //if (slider == &pitchSlider)
    //{
    //    DBG("DeckGUI::sliderValueChanged: pitchSlider moved " + std::to_string(slider->getValue()));
    //    // phase change for sawtooth waveform
    //    //dphase = pitchSlider.getValue() * 0.001;
    //     
    //    // phase change for sine waveform
    //    //dphase = volumeSlider.getValue() * 0.01;
    //}
    // implement gain slider effects
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
        // player->loadURL(juce::URL(juce::File{ files[0] }));
        //  TODO: load waveformdisplay, too?

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