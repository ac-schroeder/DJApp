#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
public:
    /** 
     * Constructor
     *
     * @param _formatManager - Reference to the shared audio format manager
     *      for the app. Used to create the audio source for the player.
     */
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);

    /** 
     * Destructor 
     */
    ~DJAudioPlayer();

    /** 
     * Implements AudioSource: Prepares the audio source to play. 
     *
     * @param samplesPerBlockExpected - The number of samples the source plays 
     *     when it gets an audio block
     * @param sampleRate - The sample rate of the output
     */
    void prepareToPlay(int samplesPerBlockExpected, double _sampleRate) override;

    /** 
     * Implements AudioSource: Fetches blocks of audio data. 
     *
     * @param bufferToFill - The audio source buffer.
     */
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    /** 
     * Implements AudioSource: Releases resources from the audio source
     * after playback has stopped.
     */
    void releaseResources() override;

    /** 
     * Loads an audio file as an audio source. 
     * 
     * @param audioURL - The URL of the audio file being loaded.
     */
    void loadURL(const juce::URL& audioURL);

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

    /** 
     * Starts playback on the audio source. 
     */
    void start();

    /** 
     * Pauses playback on the audio source. 
     */
    void pause();

    /** 
     * Stops and resets playback on the audio source. 
     */
    void stop();

    /** 
     * Returns the relative position of the playhead as a percentage of 
     * the track length. 
     * 
     * @return - The position as a percentage of track length
     */
    double getPositionRelative();

    /** 
     * Gets the source's track length and formats it as a string of 
     * minutes and seconds. 
     *
     * @return The track length as a formatted string
     */
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

    // The audio source's sample rate
    double sampleRate { 0 };
};