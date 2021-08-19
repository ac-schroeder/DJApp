/*
  ==============================================================================

    MusicLibrary.h
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <JuceHeader.h>
#include "DJAudioPlayer.h"

class MusicTrack
{
public:
    juce::String title;     // the track title
    juce::URL audioURL;     // the track file URL
    float songLength;       // the track length
};

class MusicLibrary 
{
public:
    MusicLibrary(juce::AudioFormatManager& _formatManager);
    ~MusicLibrary();

    // can this be a vector of pointers to the MusicTrack objects?
    std::vector<MusicTrack> getTracks();
    // can this be a pointer to a MusicTrack?
    MusicTrack getTrack(std::string keyword);

    void addTrack(juce::URL audioURL);
    void saveLibrary();
    void loadLibrary();

private:
    std::vector<MusicTrack> libraryTracks;

    // an audio player to read meta info about tracks
    DJAudioPlayer trackPlayer;
};


