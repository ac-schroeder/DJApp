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
    /** Returns a pointer to the matching track if a match is found, or else nullptr */
    MusicTrack* getTrack(juce::String keyword);
    /** Adds a track to the music library */
    void addTrack(juce::URL audioURL);

private:
    // the music library
    std::vector<MusicTrack> libraryTracks;

    // local file object to store library CSV data
    juce::File tracksFile;

    // an audio player to read meta info about tracks
    DJAudioPlayer sourceReader;

    /** Saves the music library track list to CSV */
    void saveLibrary();
    /** Loads the music library track list from CSV */
    void loadLibrary();
};


