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
    // set look and feel
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::orange);


    // set default text for track title
    trackTitle.setColour(juce::Label::ColourIds::textColourId, juce::Colours::orange);
    trackTitle.setText("Load a track below to get started...", juce::NotificationType::dontSendNotification);

    // load button images
    juce::File playButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("play.png");
    juce::File pauseButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("pause.png");
    juce::File stopButtonImageFile = juce::File::getCurrentWorkingDirectory().getChildFile("stop.png");

    juce::Image playButtonImage = juce::ImageFileFormat::loadFrom(playButtonImageFile);
    juce::Image pauseButtonImage = juce::ImageFileFormat::loadFrom(pauseButtonImageFile);
    juce::Image stopButtonImage = juce::ImageFileFormat::loadFrom(stopButtonImageFile);
    juce::Image nullImage{};

    playButton.setImages(true, true, true, 
                         playButtonImage, 1, juce::Colours::transparentWhite, 
                         nullImage, 0, juce::Colours::orange,
                         nullImage, 0, juce::Colours::orange);
    pauseButton.setImages(true, true, true,
                         pauseButtonImage, 1, juce::Colours::transparentWhite,
                         nullImage, 0, juce::Colours::orange,
                         nullImage, 0, juce::Colours::orange);
    stopButton.setImages(true, true, true,
                         stopButtonImage, 1, juce::Colours::transparentWhite,
                         nullImage, 0, juce::Colours::orange,
                         nullImage, 0, juce::Colours::orange);

    // add components
    addAndMakeVisible(trackTitle);
    addAndMakeVisible(startStopControls);
    addAndMakeVisible(playButton);
    addAndMakeVisible(pauseButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volumeControls);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeSliderLabel);
    addAndMakeVisible(turntableDisplay);
    addAndMakeVisible(frequencyControls);
    addAndMakeVisible(lowShelfGainSlider);
    addAndMakeVisible(highShelfGainSlider);
    addAndMakeVisible(playbackControls);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(positionSliderLabel);
    addAndMakeVisible(waveformDisplay);

    // add listeners
    playButton.addListener(this);
    pauseButton.addListener(this);
    stopButton.addListener(this);
    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);
    lowShelfGainSlider.addListener(this);
    highShelfGainSlider.addListener(this);

    // attach slider labels
    volumeSliderLabel.setText("Volume", juce::dontSendNotification);
    volumeSliderLabel.attachToComponent(&volumeSlider, true);
    speedSliderLabel.setText("Speed", juce::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, true);
    positionSliderLabel.setText("Position", juce::dontSendNotification);
    positionSliderLabel.attachToComponent(&positionSlider, true);

    // format slider text boxes
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 
                                50, speedSlider.getTextBoxHeight());
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 
                                50, positionSlider.getTextBoxHeight());
        
    // restrict slider ranges
    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 2.0, 0.01);
    positionSlider.setRange(0.0, 100.0, 0.01);
    lowShelfGainSlider.setRange(0.01, 1.0);
    highShelfGainSlider.setRange(0.01, 1.0);

    // set initial slider values
    volumeSlider.setValue(1.0, juce::dontSendNotification);
    speedSlider.setValue(1.0, juce::dontSendNotification);
    positionSlider.setValue(0, juce::dontSendNotification);

    // start the timer to coordinate audio playback with waveform and turntable displays
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
    trackTitle.setBounds(area.removeFromTop(36));

    // waveform display section
    waveformDisplay.setBounds(area.removeFromBottom(area.getHeight() / 5));

    // playback controls section
    auto playbackControlsArea = area.removeFromBottom(area.getHeight() / 4);
    playbackControls.setBounds(playbackControlsArea);

    // frequency controls section
    auto frequencyControlsArea = area.removeFromRight(area.getWidth() * 0.6);
    frequencyControls.setBounds(frequencyControlsArea);

    // start-stop controls section
    auto startStopControlsArea = area.removeFromTop(area.getHeight() * 0.2);
    startStopControls.setBounds(startStopControlsArea);

    // volume slider
    auto volumeControlsArea = area.removeFromTop(area.getHeight() / 6);
    volumeControls.setBounds(volumeControlsArea);

    // turntable display - set to be square and centered horizontally
    auto turntableDimension = juce::jmin(area.getHeight(), area.getWidth());
    auto turntableX = area.getX() + ((area.getWidth() - turntableDimension) / 2);
    turntableDisplay.setBounds(turntableX, area.getY(), turntableDimension, turntableDimension);

    /*=== Section Subcomponent Bounds ===*/

    // start-stop buttons
    auto startStopButtonWidth = startStopControlsArea.getWidth() / 3;
    auto startStopButtonMargin = 10;
    playButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));
    pauseButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));
    stopButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));

    // volume controls
    auto volumeLabelWidth = 60;
    volumeSliderLabel.setBounds(volumeControlsArea.removeFromLeft(volumeLabelWidth));
    volumeSlider.setBounds(volumeControlsArea);

    // playback control sliders
    auto playbackSliderHeight = playbackControlsArea.getHeight() / 2;
    auto playbackLabelWidth = 50;
    auto playbackSliderMargin = 15;

    auto speedSliderArea = playbackControlsArea.removeFromTop(playbackSliderHeight);
    auto positionSliderArea = playbackControlsArea.removeFromTop(playbackSliderHeight);

    positionSliderLabel.setBounds(positionSliderArea.removeFromLeft(playbackLabelWidth));
    speedSliderLabel.setBounds(speedSliderArea.removeFromLeft(playbackLabelWidth));

    positionSlider.setBounds(positionSliderArea.reduced(playbackSliderMargin));
    speedSlider.setBounds(speedSliderArea.reduced(playbackSliderMargin));

    // frequency control sliders

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
    if (button == &pauseButton)
    {
        DBG("DeckGUI::buttonClicked: Pause Button was clicked");
        // pause playing
        player->pause();
    }
    if (button == &stopButton)
    {
        DBG("DeckGUI::buttonClicked: Stop Button was clicked");
        // pause playing
        player->stop();
    }
}

/** Implement Slider::Listener */
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    // implement volume slider
    if (slider == &volumeSlider)
    {
        DBG("DeckGUI::sliderValueChanged: volumeSlider " + std::to_string(slider->getValue()));
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
    // implement low shelf volume slider
    if (slider == &lowShelfGainSlider)
    {
        player->setLowShelf(slider->getValue());
    }
    // implement high shelf volume slider
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
void DeckGUI::loadURL(juce::URL audioURL, juce::String fileName)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);

    // set the track title in the header
    trackTitle.setText(fileName, juce::dontSendNotification);
}