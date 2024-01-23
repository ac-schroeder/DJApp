#include <JuceHeader.h>
#include "DeckGUI.h"


DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager& formatManagerToUse,
                 juce::AudioThumbnailCache& cacheToUse) 
    : player { _player },
      waveformDisplay {          
        formatManagerToUse,   // AudioFormatManager: to pass to AudioThumbnail
        cacheToUse }          // AudioThumbnailCache: to pass to AudioThumbnail
{
    // Set custom look and feel
    setLookAndFeel(&mainLookAndFeel);

    // Set default text for track title
    trackTitle.setColour(juce::Label::ColourIds::textColourId, juce::Colours::orange);
    trackTitle.setText("Load a track below to get started...",          
                       juce::dontSendNotification);

    // Load button images
    setUpButtonImages();

    // Add components
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
    addAndMakeVisible(frequencyShelfFilter);
    addAndMakeVisible(playbackControls);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(speedSliderLabel);
    addAndMakeVisible(positionSlider);
    addAndMakeVisible(positionSliderLabel);
    addAndMakeVisible(waveformDisplay);

    // Set up slider ranges, values, and labels
    setUpSliders();

    // Add listeners
    playButton.addListener(this);
    pauseButton.addListener(this);
    stopButton.addListener(this);
    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    // Start the timer to coordinate audio playback with waveform and turntable displays
    startTimer(100);
}

DeckGUI::~DeckGUI()
{
    // Stop the timer from continuing
    stopTimer();
    // Remove this component's look and feel
    setLookAndFeel(nullptr);
}

void DeckGUI::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   
    // Draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

}

// Lays out deck component areas based on container size
void DeckGUI::resized()
{
    auto area = getLocalBounds();

    /*------------- Block Section Bounds ------------*/
    // Header section
    trackTitle.setBounds(area.removeFromTop(36));
    // Waveform display section
    waveformDisplay.setBounds(area.removeFromBottom(area.getHeight() / 5));
    // Playback controls section
    auto playbackControlsArea = area.removeFromBottom(area.getHeight() / 4);
    playbackControls.setBounds(playbackControlsArea);
    // Frequency controls section
    auto frequencyControlsArea = area.removeFromRight(area.getWidth() * 0.6);
    frequencyControls.setBounds(frequencyControlsArea);
    // Start-stop controls section
    auto startStopControlsArea = area.removeFromTop(area.getHeight() * 0.2);
    startStopControls.setBounds(startStopControlsArea);
    // Volume slider
    auto volumeControlsArea = area.removeFromTop(area.getHeight() / 6);
    volumeControls.setBounds(volumeControlsArea);
    // Turntable display - set to be square and centered horizontally
    auto turntableDimension = juce::jmin(area.getHeight(), area.getWidth());
    auto turntableX = area.getX() + ((area.getWidth() - turntableDimension) / 2);
    turntableDisplay.setBounds(turntableX, area.getY(), turntableDimension, turntableDimension);

    /*------------- Sub-Section Bounds ------------*/
    // Start-stop buttons
    auto startStopButtonWidth = startStopControlsArea.getWidth() / 3;
    auto startStopButtonMargin = 10;
    playButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));
    pauseButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));
    stopButton.setBounds(startStopControlsArea.removeFromLeft(startStopButtonWidth).reduced(startStopButtonMargin));
    // Volume controls
    auto volumeLabelWidth = 60;
    volumeSliderLabel.setBounds(volumeControlsArea.removeFromLeft(volumeLabelWidth));
    volumeSlider.setBounds(volumeControlsArea);
    // Frequency control sliders
    frequencyShelfFilter.setBounds(frequencyControlsArea.reduced(5));
    // Playback control sliders
    auto playbackSliderHeight = playbackControlsArea.getHeight() / 2;
    auto playbackLabelWidth = 50;
    auto playbackSliderMargin = 10;
    auto speedSliderArea = playbackControlsArea.removeFromTop(playbackSliderHeight);
    auto positionSliderArea = playbackControlsArea.removeFromTop(playbackSliderHeight);
    positionSliderLabel.setBounds(positionSliderArea.removeFromLeft(playbackLabelWidth));
    speedSliderLabel.setBounds(speedSliderArea.removeFromLeft(playbackLabelWidth));
    positionSlider.setBounds(positionSliderArea.reduced(playbackSliderMargin));
    speedSlider.setBounds(speedSliderArea.reduced(playbackSliderMargin));
}

void DeckGUI::loadURL(const juce::URL& audioURL, const juce::String& fileName)
{
    // Load the URL with the player (audio source)
    player->loadURL(audioURL);
    // Load the URL with the waveform display (audio thumbnail)
    waveformDisplay.loadURL(audioURL);

    // Set the track title for the deck
    trackTitle.setText(fileName, juce::dontSendNotification);
    // Reset position slider
    positionSlider.setValue(0, juce::dontSendNotification);
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)  // Play Button
    {
        player->start();        
    }
    if (button == &pauseButton) // Pause Button
    {
        player->pause();        
    }
    if (button == &stopButton)  // Stop Button
    {
        // Stopping also resets playhead position to 
        // beginning of track
        player->stop();     
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)    // Volume slider
    {
        // Set playback gain (volume) from the slider value
        player->setGain(slider->getValue());
    } 
    if (slider == &speedSlider)     // Speed slider
    {
        // Set playback speed from the slider value
        auto speed = slider->getValue();
        if (speed != 0)
        {
            player->setSpeed(speed);
        }
    }
    if (slider == &positionSlider)  // Position slider
    {
        // Set playhead position based on slider value
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    // Load files dragged into the deck GUI areas straight to the deck,
    // bypassing the playlist
    if (files.size() == 1)
    {
        // Convert the file to a URL and filename string
        juce::File file { files[0] };
        juce::String fileName = file.getFileName();
        // Load the file
        loadURL(juce::URL(file), fileName);
    }
}

// Called at interval and coordinates playback with the waveform and
// turntable displays
void DeckGUI::timerCallback()
{
    // Update the relative position of the waveform display
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    // Update the relative position of the turntable display
    turntableDisplay.setPositionRelative(player->getPositionRelative());
}

// Button images are free stock images licensed under the MIT license, 
// from iconfinder.com.
void DeckGUI::setUpButtonImages()
{
    // Get the image files
    juce::File playButtonImageFile = buttonImageDirectory.getChildFile("play.png");
    juce::File pauseButtonImageFile = buttonImageDirectory.getChildFile("pause.png");
    juce::File stopButtonImageFile = buttonImageDirectory.getChildFile("stop.png");

    // Convert to image objects
    juce::Image playButtonImage = juce::ImageFileFormat::loadFrom(playButtonImageFile);
    juce::Image pauseButtonImage = juce::ImageFileFormat::loadFrom(pauseButtonImageFile);
    juce::Image stopButtonImage = juce::ImageFileFormat::loadFrom(stopButtonImageFile);

    // Create a dummy null image object for no overlays on mouse over, mouse down
    juce::Image nullImage{};

    // Set button images
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
}

void DeckGUI::setUpSliders()
{
    // Attach slider labels
    volumeSliderLabel.setText("Volume", juce::dontSendNotification);
    volumeSliderLabel.attachToComponent(&volumeSlider, true);
    speedSliderLabel.setText("Speed", juce::dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, true);
    positionSliderLabel.setText("Position", juce::dontSendNotification);
    positionSliderLabel.attachToComponent(&positionSlider, true);

    // Format slider text boxes
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false,
        50, speedSlider.getTextBoxHeight());
    positionSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false,
        50, positionSlider.getTextBoxHeight());

    // Restrict slider ranges
    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 2.0, 0.01);
    positionSlider.setRange(0.0, 100.0, 0.01);

    // Set initial slider values
    volumeSlider.setValue(0.7, juce::dontSendNotification);
    speedSlider.setValue(1.0, juce::dontSendNotification);
    positionSlider.setValue(0, juce::dontSendNotification);
}
