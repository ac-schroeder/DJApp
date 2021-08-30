#include "MainComponent.h"
#include <cmath>

//==============================================================================
MainComponent::MainComponent()
{
    // set size
    setSize (1000, 800);

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
    int deckHeight = getHeight() * 0.65;
    deckGUI1.setBounds(0, 0, getWidth()/2, deckHeight);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, deckHeight);

    // set bounds on playlist component
    playlistComponent.setBounds(0, deckHeight, getWidth(), getHeight() - deckHeight);
}


