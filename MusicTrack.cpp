/*
  ==============================================================================

    MusicTrack.cpp
    Created: 20 Aug 2021 8:58:44am
    Author:  alana

  ==============================================================================
*/

#include "MusicTrack.h"

MusicTrack::MusicTrack(juce::String _fileName, juce::URL _audioURL, std::string _length)
    : fileName { _fileName },
      audioURL{ _audioURL }, 
      length{ _length }
{

    trackID = countID++;
}