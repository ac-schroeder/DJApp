/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 2 Aug 2021 2:28:49pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
public:
    /** Constructor */
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    /** Destructor */
    ~DJAudioPlayer();

    /** Implements AudioSource: Prepares the audio source to play. */
    void prepareToPlay(int samplesPerBlockExpected, double _sampleRate) override;
    /** Implements AudioSource: Fills the buffer with the next block. */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    /** Implements AudioSource: Releases resources from the audio sources. */
    void releaseResources() override;

    /** 
     * Loads an audio file as an audio source. 
     * 
     * @param audioURL - The URL of the audio file being loaded.
     */
    void loadURL(juce::URL audioURL);

    /**
     * Sets the gain (volume) of the audio source.
     *
     * @param gain - The new gain value to set, as a number between 0 and 1.
     */
    void setGain(double gain);

    /**
     * Sets the playback speed of the audio source.
     *
     * @param ratio - The playback speed ratio to set, as a number between 0 and 2.
     */
    void setSpeed(double ratio);

    /**
     * Sets the position in the audio source playback.
     *
     * @param positionInSeconds - The number of seconds into the playback to jump to.
     */
    void setPosition(double positionInSeconds);

    /**
     * Sets the position in the audio source playback.
     *
     * @param relativePosition - The position as a percentage into the track.
     */
    void setPositionRelative(double relativePosition);

    /**
     * Sets the low shelf filter from the provided coefficients.
     *
     * @param frequency - The cutoff frequency.
     * @param gain      - The amount to attenuate, a value between 0.01 and 1.
     * @param q         - The Q value, a value between 0.1 and 40.
     */
    void setLowShelf(double frequency, float gain, double q);

    /**
     * Sets the high shelf filter from the provided coefficients.
     *
     * @param frequency - The cutoff frequency.
     * @param gain      - The amount to attenuate, a value between 0.01 and 1.
     * @param q         - The Q value, a value between 0.1 and 40.
     */
    void setHighShelf(double frequency, float gain, double q);

    /** Starts playback on the audio source. */
    void start();

    /** Pauses playback on the audio source. */
    void pause();

    /** Stops and resets playback on the audio source. */
    void stop();

    /** 
     * Returns the relative position of the playhead as a percentage of 
     * the track length. 
     */
    double getPositionRelative();

    /** Returns the track length as a formatted string of minutes and seconds. */
    std::string getTrackLength();

private:
    // Shared format manager
    juce::AudioFormatManager& formatManager;

    // Audio source object 
    // Uses smart pointer for dynamic instantiation
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // Transport wrapper for the audio source, to control playback
    juce::AudioTransportSource transportSource;
    // Filter wrappers for the audio source, to enable filtering frequencies
    juce::IIRFilterAudioSource lowShelfFilteredSource {&transportSource, false};
    juce::IIRFilterAudioSource highShelfFilteredSource {&lowShelfFilteredSource, false};
    // Resampling wrapper for the audio source, to control speed
    juce::ResamplingAudioSource resampleSource{ &highShelfFilteredSource, false, 2 };

    // The low and high shelf coefficients for the audio source
    juce::IIRCoefficients lowShelfCoefficients;
    juce::IIRCoefficients highShelfCoefficients;

    // The audio source's sample rate
    double sampleRate;
};