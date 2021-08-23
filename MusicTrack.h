/*
  ==============================================================================

    MusicTrack.h
    Created: 20 Aug 2021 8:58:44am
    Author:  alana

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class MusicTrack
{
public:
    MusicTrack(juce::String _fileName, juce::URL _audioURL, std::string _length);

    juce::String fileName;      // the track title
    juce::URL audioURL;         // the track file URL
    std::string length;         // the track length
};