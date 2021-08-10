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

    // draw the deck GUI components
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    // draw the playlist component
    addAndMakeVisible(playlistComponent);

    // register basic formats for the app's formatManager
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // initialise sample settings
    //phase = 0;
    //dphase = 0;
    //gain = 0.5;

    // set up player audio sources
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // set up mixer audio source
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // add both players to the mixer audio source
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // mixer source will manage each audio block
    mixerSource.getNextAudioBlock(bufferToFill);
}

//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    // clear buffer and return if not playing
//    if (!playing)
//    {
//        bufferToFill.clearActiveBufferRegion();
//        return;
//    }
//
//    // get pointers to the buffer channel locations to write to
//    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, 
//        bufferToFill.startSample);
//    auto* rightChannel = bufferToFill.buffer->getWritePointer(1,
//        bufferToFill.startSample);
//
//    // fill the buffer channels with generated samples
//    for (auto i = 0; i < bufferToFill.numSamples; ++i)
//    {
//        // set the sample value
//        // auto sample = rand.nextFloat() * 0.125f * gain;     // white noise
//         auto sample = fmod(phase, 1.0f) * 0.125f * gain;      // sawtooth wave        
//        // auto sample = sin(phase) * 0.125f * gain;           // sine wave
//
//        // assign value to both channels
//        leftChannel[i] = sample;
//        rightChannel[i] = sample;
//
//        // increment the phase according to the frequency
//        // phase += dphase;              // for a constant frequency/pitch, add a constant dphase
//
//        // increment the phase according to the frequency
//        phase += fmod(dphase, 0.01f);   // fmod the dphase to reset the pitch to a lower one
//        dphase += 0.0000005f;           // make the frequency/pitch ramp up
//    }    
//}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs(); // TODO: what does this do, and do I need it?
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // set bounds on deck GUI components
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight() / 2);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);

    // set bounds on playlist component
    playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}


