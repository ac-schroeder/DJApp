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

    /** Returns the full music library of tracks */
    std::vector<MusicTrack> getTracks();
    /** Looks up a track in the library by trackID */
    MusicTrack getTrack(int _trackID);
    /** Adds a track to the music library */
    void addTrack(juce::URL audioURL);
    /** Removes a track from the music library */
    void removeTrack(int _trackID);
    /** Clears the music library of all tracks */
    void clearLibrary();
    /** Returns a pointer to the matching track if a match is found, or else nullptr */
    MusicTrack* searchLibrary(juce::String keyword);

private:
    // the music library
    std::vector<MusicTrack> libraryTracks;
    // a counter for incrementing track IDs
    int trackIDCount{ 0 };

    // local file object to store library CSV data
    juce::File tracksFile;

    // an audio player to read meta info about tracks
    DJAudioPlayer sourceReader;

    /** Saves the music library track list to CSV */
    void saveLibrary();
    /** Loads the music library track list from CSV */
    void loadLibrary();
};


