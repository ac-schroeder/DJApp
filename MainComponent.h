#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent,
                       public juce::Button::Listener,
                       public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================#
    // Implement AudioAppComponent
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    // Implement Component ?
    void paint (juce::Graphics& g) override;
    void resized() override;

    /** Implement Button::Listener */
    void buttonClicked(juce::Button* button) override;

    /** Implement Slider::Listener */
    void sliderValueChanged(juce::Slider* slider) override;

private:
    //==============================================================================
    // Your private member variables go here...
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    juce::Slider gainSlider;
    juce::Slider pitchSlider;
    juce::Slider speedSlider;
    

    bool playing;

    // sound sample modification settings
    juce::Random rand;
    double phase;   // sample phase
    double dphase;  // phase difference
    float gain;     // sample gain 

    // sound file playback objects
    juce::AudioFormatManager formatManager;
    // use smart pointer for dynamic instantiation
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};

    void loadURL(juce::URL audioURL);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
