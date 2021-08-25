/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 2 Aug 2021 2:28:49pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double _sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    void setLowShelf(double gain);
    void setHighShelf(double gain);

    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

    /** get the track length */
    std::string getTrackLength();

private:
    // reference to the app's audio format manager object, to process different sound file formats
    juce::AudioFormatManager& formatManager;
    // audio source object; use smart pointer for dynamic instantiation
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // transport wrapper for the audio source, to control playback
    juce::AudioTransportSource transportSource;
    // filter wrappers for the audio source, to enable filtering frequencies
    juce::IIRFilterAudioSource lowShelfFilteredSource {&transportSource, false};
    juce::IIRFilterAudioSource highShelfFilteredSource {&lowShelfFilteredSource, false};
    // resampling wrapper for the audio source, to control speed
    juce::ResamplingAudioSource resampleSource{ &highShelfFilteredSource, false, 2 };

    // the sample rate
    double sampleRate;
    // the low and high shelf coefficients
    juce::IIRCoefficients lowShelfCoefficients;
    juce::IIRCoefficients highShelfCoefficients;
};