#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
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
    // Shared AudioFormatManager for all audio players and thumbnails
    juce::AudioFormatManager formatManager;
    // Shared AudioThumbnailCache for all deck waveform AudioThumbnail objects
    juce::AudioThumbnailCache thumbCache{ 100 };

    // Audio source players
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };

    // Mixer audio source to handle combination of deck players
    juce::MixerAudioSource mixerSource;

    // Deck GUI components
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };

    // Track playlist component to display under the deck GUIs
    PlaylistComponent playlistComponent{ formatManager, &deckGUI1, &deckGUI2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
