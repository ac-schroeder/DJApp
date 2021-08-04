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
    DJAudioPlayer();
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

private:
    // audio format manager object to process different sound file formats
    juce::AudioFormatManager formatManager;
    // audio source object; use smart pointer for dynamic instantiation
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    // wrapper object for readerSource, to control playback
    juce::AudioTransportSource transportSource;
    // wrapper object for transportSource, to control speed
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};