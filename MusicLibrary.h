/*
  ==============================================================================

    MusicLibrary.h
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MusicTrack.h"
#include "DJAudioPlayer.h"

class MusicLibrary 
{
public:
    MusicLibrary(juce::AudioFormatManager& _formatManager);
    ~MusicLibrary();

    // can this be a vector of pointers to the MusicTrack objects?
    std::vector<MusicTrack> getTracks();
    // can this be a pointer to a MusicTrack?
    MusicTrack* getTrack(juce::String keyword);
    //std::unique_ptr<MusicTrack> getTrack(juce::String keyword);

    void addTrack(juce::URL audioURL);
    void saveLibrary();
    void loadLibrary();

private:
    //std::vector<MusicTrack*> libraryTracks;
    std::vector<MusicTrack> libraryTracks;

    // an audio player to read meta info about tracks
    DJAudioPlayer sourceReader;
};


