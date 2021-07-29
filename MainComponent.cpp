#include "MainComponent.h"
#include <cmath>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(pitchSlider);
    addAndMakeVisible(gainSlider);


    // add our own self, a Button::Listener object, to the play button
    // we are a listener object because we inherit from Button::Listener class
    // even though we are a component too
    // we have our own buttonClicked function
    // "this" refers to MainComponent
    playButton.addListener(this);
    stopButton.addListener(this);
    volumeSlider.addListener(this);
    pitchSlider.addListener(this);
    gainSlider.addListener(this);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()

    // set to not be playing on start
    playing = false;

    // speed of change in sample value, affects frequency (pitch)
    phase = 0.0;
    dphase = 0.0001;

    // initialise gain setting
    gain = 0.5;

}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    // clear buffer and return if not playing
    if (!playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    // these channels will be sent to the soundcard
    // we are getting the channel, which is an array of samples, and which sample to start with
    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, 
        bufferToFill.startSample);
    auto* rightChannel = bufferToFill.buffer->getWritePointer(1,
        bufferToFill.startSample);

    // auto generate random samples
    for (auto i = 0; i < bufferToFill.numSamples; ++i)
    {
        // restrict volume
        // double sample = rand.nextFloat() * 0.125f;
        // assign to the channels

        // sawtooth wave
         double sample = fmod(phase, 0.2);

        // sine wave
        //double sample = sin(phase) * 0.1;
        leftChannel[i] = sample;
        rightChannel[i] = sample;

        //phase += 0.001;
        phase += dphase;
    }

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    DBG("Paint is called!");
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    DBG("MainComponent::resized");

    double rowHeight = getHeight() / 5;

    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    volumeSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
    pitchSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    gainSlider.setBounds(0, rowHeight * 4, getWidth(), rowHeight);
}

/** Implement Button::Listener */
void MainComponent::buttonClicked(juce::Button* button)
{
    // process responses for each button
    if (button == &playButton)
    {
        DBG("MainComponent::buttonClicked: Play Button was clicked");
        // begin playing
        playing = true;
    }
    if (button == &stopButton)
    {
        DBG("MainComponent::buttonClicked: Stop Button was clicked");
        // stop playing
        playing = false;
    }
}

/** Implement Slider::Listener */
void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    // implement pitch slider effects
    if (slider == &pitchSlider)
    {
        DBG("MainComponent::sliderValueChanged: pitchSlider moved " + std::to_string(slider->getValue()));
        // phase change for sawtooth waveform
         dphase = volumeSlider.getValue() * 0.001;
        // 
        // phase change for sine waveform
        //dphase = volumeSlider.getValue() * 0.01;
    }

    // implement gain slider effects
    if (slider == &gainSlider)
    {
        DBG("MainComponent:sliderValueChanged: gainSlider " + std::to_string(slider->getValue()));
        gain = gainSlider.getValue();
    }
}