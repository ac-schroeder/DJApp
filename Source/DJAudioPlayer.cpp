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
    // Get the sample rate
    sampleRate = _sampleRate;
    // Prepare the transport source
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    // Pprepare the resample source, for playback speed control
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    // Release resources for both source objects
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

// Creates JUCE audio source objects for the file 
void DJAudioPlayer::loadURL(const juce::URL& audioURL)
{
    // Convert audioURL to an input stream and create an AudioFormatReader for it
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // Check that the file converted correctly
    if (reader != nullptr)
    {
        // Dynamically create an AudioFormatReaderSource based on the reader
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
            { new juce::AudioFormatReaderSource(reader, true) };

        // Wrap in a TransportSource 
        transportSource.setSource(newSource.get(), 0, 
                                  nullptr, reader->sampleRate);
        // Move the AudioFormatReaderSource object to the readerSource pointer
        readerSource.reset(newSource.release());
    }
    else
    {
        DBG("Something went wrong loading the file.");
    }
}

void DJAudioPlayer::setGain(double gain)
{
    // Make sure gain is in the expected range
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
    // Make sure speed ratio is in the expected range
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
    // Update the position of the playhead 
    transportSource.setPosition(positionInSeconds);
}

void DJAudioPlayer::setPositionRelative(double relativePosition)
{
    // Make sure the relative position is in the expected range
    if (relativePosition < 0 || relativePosition > 100.0)
    {
        DBG("DJAudioPlayer::setPositionRelative: position should be between 0 and  100");
    }
    else
    {
        // Convert relative position to actual position in seconds
        double positionInSeconds = transportSource.getLengthInSeconds() 
                                   * (relativePosition / 100);
        // Update the position
        setPosition(positionInSeconds);
    }
}

void DJAudioPlayer::setLowShelf(double frequency, float gain, double q)
{
    // Set the coefficients for the filter
    juce::IIRCoefficients lowShelfCoefficients = juce::IIRCoefficients::makeLowShelf(sampleRate, frequency, q, gain);
    // Update the filtered audio source with the coefficients
    lowShelfFilteredSource.setCoefficients(lowShelfCoefficients);
}

void DJAudioPlayer::setHighShelf(double frequency, float gain, double q)
{
    // Set the coefficients for the filter
    juce::IIRCoefficients highShelfCoefficients = juce::IIRCoefficients::makeHighShelf(sampleRate, frequency, q, gain);
    // Update the filtered audio source with the coefficients
    highShelfFilteredSource.setCoefficients(highShelfCoefficients);
}

void DJAudioPlayer::start()
{
    transportSource.start();                    //  Begin playback
}

void DJAudioPlayer::pause()
{
    transportSource.stop();                     // Pause playback
}

void DJAudioPlayer::stop()
{
    transportSource.stop();                     // Pause playback
    transportSource.setNextReadPosition(0);     // Reset position to 0
}

// Returns the relative position into the track, or 0 if no track is loaded.
// Called continuously by the DeckGUI::timerCallback. 
double DJAudioPlayer::getPositionRelative()
{
    double position{};  // Default is 0

    // If a track is loaded and playing, get position
    if (transportSource.getLengthInSeconds() != 0)
    {
        // Calculate relative position as a proportion of total track length
        position = transportSource.getCurrentPosition() / 
                    transportSource.getLengthInSeconds();
    }
    return position;
}

std::string DJAudioPlayer::getTrackLength()
{
    // Get the total seconds of the track
    int lengthInSeconds = round(transportSource.getLengthInSeconds());

    // Convert to minutes and seconds
    int secondsLong = lengthInSeconds % 60;
    int minutesLong = (int)(lengthInSeconds / 60);

    // Return a formatted string
    return std::to_string(minutesLong) + "m " + std::to_string(secondsLong) + "s";
}