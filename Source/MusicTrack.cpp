/*
  ==============================================================================

    MusicTrack.cpp
    Created: 20 Aug 2021 8:58:44am
    Author:  Alana Schroeder

  ==============================================================================
*/

#include "MusicTrack.h"


MusicTrack::MusicTrack(int _trackID, juce::String _fileName, juce::URL _audioURL, std::string _length)
    : trackID{ _trackID },
      fileName { _fileName },
      audioURL{ _audioURL }, 
      length{ _length }
{
}

int MusicTrack::getTrackID() const
{
    return trackID;
}

juce::String MusicTrack::getFileName() const
{
    return fileName;
}

juce::URL MusicTrack::getAudioURL() const
{
    return audioURL;
}

std::string MusicTrack::getLength() const
{
    return length;
}