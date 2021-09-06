/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 2 Aug 2021 2:28:49pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager{ _formatManager }
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double _sampleRate)
{
    // store the sample rate
    sampleRate = _sampleRate;
    // prepare the transport source
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // prepare the resample source, for playback speed control
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    // release resources for both source objects
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    // convert audioURL to an input stream and create an AudioFormatReader for it
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // check that the file converted correctly
    if (reader != nullptr)
    {
        // dynamically create an AudioFormatReaderSource based on the reader
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
            { new juce::AudioFormatReaderSource(reader, true) };

        // wrap in a TransportSource 
        transportSource.setSource(newSource.get(), 0, nullptr,
            reader->sampleRate);
        // move the AudioFormatReaderSource object to the readerSource pointer
        readerSource.reset(newSource.release());
    }
    else
    {
        DBG("Something went wrong loading the file.");
    }
}

void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain: gain should be between 0 and  1");
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 2.0)
    {
        DBG("DJAudioPlayer::setSpeed: ratio should be between 0 and 2");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double positionInSeconds)
{
    transportSource.setPosition(positionInSeconds);
}

void DJAudioPlayer::setPositionRelative(double relativePosition)
{
    if (relativePosition < 0 || relativePosition > 100.0)
    {
        DBG("DJAudioPlayer::setPositionRelative: position should be between 0 and  100");
    }
    else
    {
        double positionInSeconds = transportSource.getLengthInSeconds() 
                                   * (relativePosition / 100);
        setPosition(positionInSeconds);
    }
}

void DJAudioPlayer::setLowShelf(double frequency, float gain, double q)
{
    lowShelfCoefficients = juce::IIRCoefficients::makeLowShelf(sampleRate, frequency, q, gain);
    lowShelfFilteredSource.setCoefficients(lowShelfCoefficients);
}

void DJAudioPlayer::setHighShelf(double frequency, float gain, double q)
{
    highShelfCoefficients = juce::IIRCoefficients::makeHighShelf(sampleRate, frequency, q, gain);
    highShelfFilteredSource.setCoefficients(highShelfCoefficients);
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::pause()
{
    transportSource.stop();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
    transportSource.setNextReadPosition(0);
}

double DJAudioPlayer::getPositionRelative()
{
    double position{};
    if (transportSource.getLengthInSeconds() != 0)
    {
        position = transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    }
    return position;
}



/** get the track length */
std::string DJAudioPlayer::getTrackLength()
{
    int lengthInSeconds = round(transportSource.getLengthInSeconds());
    int secondsLong = lengthInSeconds % 60;
    int minutesLong = (int)(lengthInSeconds / 60);
    return std::to_string(minutesLong) + "m " + std::to_string(secondsLong) + "s";
}