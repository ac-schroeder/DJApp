#include "MainComponent.h"
#include <cmath>


MainComponent::MainComponent()
{
    // Set fixed size - app is non-resizable (see main)
    setSize (900, 700);

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

    // Add components
    addAndMakeVisible(deckGUI1);            // left deck
    addAndMakeVisible(deckGUI2);            // right deck
    addAndMakeVisible(playlistComponent);   // playlist 

    // Register basic formats in the formatManager
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // Shut down the audio device and clears the audio source
    shutdownAudio();
}


void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // Set up player audio sources
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Set up mixer audio source
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Add both players to the mixer audio source
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Mixer source will manage each audio block
    mixerSource.getNextAudioBlock(bufferToFill);
}


void MainComponent::releaseResources()
{
    // Clear player resources
    mixerSource.removeAllInputs(); 
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}


void MainComponent::paint (juce::Graphics& g)
{
    // Set background colour
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    // Set bounds on deck GUI components
    int deckHeight = getHeight() * 0.65;
    deckGUI1.setBounds(0, 0, getWidth()/2, deckHeight);
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, deckHeight);

    // Set bounds on playlist component
    playlistComponent.setBounds(0, deckHeight, getWidth(), getHeight() - deckHeight);
}


