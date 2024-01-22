#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


class MainComponent  : public juce::AudioAppComponent
{
public:
    /** Constructor */
    MainComponent();
    /** Destructor */
    ~MainComponent() override;

    /**
     * Implements AudioSource: Prepares the audio source to play.
     *
     * @param samplesPerBlockExpected - The number of samples the source plays
     *     when it gets an audio block
     * @param sampleRate - The sample rate of the output
     */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /**
     * Implements AudioSource: Fetches blocks of audio data.
     *
     * @param bufferToFill - The audio source buffer.
     */
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    /**
     * Implements AudioSource: Releases resources from the audio source
     * after playback has stopped.
     */
    void releaseResources() override;

    /**  
     * Implements Component: Draws the component's content, as well as any child 
     * components' content.
     * 
     * @param g - The graphics context of the component.
     */
    void paint (juce::Graphics& g) override;

    /**
     * Implements Component: Called when the component is resized. Used to
     * control layout of child components.
     */
    void resized() override;

private:
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
