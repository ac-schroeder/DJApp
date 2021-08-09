/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 2 Aug 2021 2:28:49pm
    Author:  alana

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
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
    // create the AudioFormatReader object from the audio file, 
    // converting the audio file to an input stream first
    // we use a pointer "reader" so that the AudioFormatReader object itself persists after loadURL completes
    // AudioFormatReader itself has object-level scope
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // check that the file converted correctly
    if (reader != nullptr)
    {
        // assign a new AudioFormatReaderSource object to a new smart pointer
        // pass it the AudioFormatReader object made from the audio file
        // set to true to delete the AudioFormatReader object when... 
        // ...the AudioFormatReaderSource object is deleted
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
        (new juce::AudioFormatReaderSource(reader, true));

        // create the TransportSource object based on this AudioFormatReaderSource object
        transportSource.setSource(newSource.get(), 0, nullptr,
            reader->sampleRate);
        // pass the AudioFormatReaderSource object to the class readerSource pointer
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
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setSpeed: ratio should be between 0 and  100");
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        DBG("DJAudioPlayer::setPositionRelative: position should be between 0 and  1");
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}