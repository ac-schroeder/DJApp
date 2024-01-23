#pragma once

#include <JuceHeader.h>
#include "MainLookAndFeel.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "FrequencyShelfFilter.h"
#include "TurntableDisplay.h"


class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    /** 
     * Constructor 
     *
     * @param player             - A pointer to the player for the deck.
     * @param formatManagerToUse - Reference to the shared audio format manager
     *                             for the app, to pass on to child components 
     *                             that need it.
     * @param cacheToUse         - Reference to the shared audio source thumbnail 
     *                             cache, to pass on to child components that 
     *                             need it.
     */
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager& formatManagerToUse,
            juce::AudioThumbnailCache& cacheToUse);

    /** 
     * Destructor 
     */
    ~DeckGUI() override;

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
     * Loads an audio URL to the deck's player.
     *
     * @param audioURL - The URL of the audio file being loaded.
     * @param fileName - The file name of the audio file being loaded.
     */
    void loadURL(const juce::URL& audioURL, const juce::String& fileName);

private:
    /** 
     * Implements Button::Listener: Processes button clicks.
     *
     * @param button - Pointer to the button that was clicked.
     */
    void buttonClicked(juce::Button* button) override;

    /** 
     * Implements Slider::Listener: Processes slider value changes. 
     *
     * @param slider - Pointer to the slider that triggered the event.
     */
    void sliderValueChanged(juce::Slider* slider) override;

    /** 
     * Implements FileDragAndDrop: Callback called repeatedly while user
     * is dragging files.
     *
     * @param files - The set of files being dragged.
     */
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /** 
     * Implements FileDragAndDrop: Processes files dropped into the component. 
     *
     * @param files - The set of files being dropped.
     * @param x     - The mouse x position, relative to the component.
     * @param y     - The mouse y position, relative to the component.
     */
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** 
     * Implements Timer. Processes the timer callback to update the display 
     * on Waveform and Turntable components.
     */
    void timerCallback() override;

    /** 
     * Loads images for GUI buttons. 
     * 
     * @param _buttonImageDirectory - The file containing the image objects.
     */
    void setUpButtonImages();

    /** 
     * Initializes slider labels and ranges. 
     */
    void setUpSliders();

    // Pointer to the audio player for the deck
    DJAudioPlayer* player;
    // Location of images for GUI buttons
    juce::File buttonImageDirectory{ 
        juce::File::getCurrentWorkingDirectory().getChildFile("button-images")};
    // Custom look and feel 
    MainLookAndFeel mainLookAndFeel;

    /*------------- Child Components ------------*/
    // Deck title
    juce::Label trackTitle;                 
    // Start/stop/pause button block
    juce::GroupComponent startStopControls;
    juce::ImageButton playButton;
    juce::ImageButton pauseButton;
    juce::ImageButton stopButton;
    // Volume slider component block
    juce::GroupComponent volumeControls;
    juce::Slider volumeSlider{ juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::NoTextBox };
    juce::Label volumeSliderLabel;
    // Turntable display 
    TurntableDisplay turntableDisplay;
    // Frequency shelf sliders block
    juce::GroupComponent frequencyControls;
    FrequencyShelfFilter frequencyShelfFilter{ player };
    // Playback manipulation sliders block
    juce::GroupComponent playbackControls;
    juce::Slider speedSlider;
    juce::Label speedSliderLabel;
    juce::Slider positionSlider;
    juce::Label positionSliderLabel;
    // Waveform display component
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
