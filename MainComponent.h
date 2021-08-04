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

    DJAudioPlayer player1;
    DJAudioPlayer player2;
    DeckGUI deckGUI1{ &player1 };
    DeckGUI deckGUI2{ &player2 };

    juce::MixerAudioSource mixerSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
