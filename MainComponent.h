#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
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
    // Implement Component 
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // sound sample modification settings
    //juce::Random rand;
    //double phase;   // sample phase
    //double dphase;  // phase difference
    //float gain;     // sample gain 

    // shared AudioFormatManager for all deck players
    // actually, this is only used by the waveform thumbnails!!! seems wrong.
    juce::AudioFormatManager formatManager;
    // shared AudioThumbnailCache for all deck waveform thumbnails
    juce::AudioThumbnailCache thumbCache{ 100 };

    // audio source players
    DJAudioPlayer player1;
    DJAudioPlayer player2;

    // deck GUI components
    // get the address of the players to assign to pointers in the objects
    // pass them the shared format manager and waveform thumbnail cache
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    juce::MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
