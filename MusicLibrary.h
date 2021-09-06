/*
  ==============================================================================

    MusicLibrary.h
    Created: 19 Aug 2021 10:40:26am
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MusicTrack.h"
#include "DJAudioPlayer.h"

class MusicLibrary 
{
public:
    /** Constructor */
    MusicLibrary(juce::AudioFormatManager& _formatManager);
    /** Destructor */
    ~MusicLibrary();

    /** Returns the full music library of tracks. */
    std::vector<MusicTrack> getTracks();

    /** 
     * Looks up a track in the library by trackID. 
     *
     * @param _trackID - The unique ID of the track in the music library
     * @return A music track object
     */
    MusicTrack getTrack(int _trackID);

    /** 
     * Adds a track to the music library.
     *
     * @param audioURL - The URL of the track to add to the library
     */
    void addTrack(juce::URL audioURL);

    /** 
     * Removes a track from the music library.
     *
     * @param _trackID - The unique ID of the track in the music library
     */
    void removeTrack(int _trackID);

    /** Clears the music library of all tracks */
    void clearLibrary();

    /** 
     * Returns a vector of tracks matching the search keyword. 
     *
     * @param keyword - The search term to search for tracks
     * @return A vector of music track objects
     */
    std::vector<MusicTrack> searchLibrary(juce::String& keyword);

private:
    /** Saves the music library track list to CSV. */
    void saveLibrary();
    /** Loads the music library track list from CSV. */
    void loadLibrary();

    // The music library
    std::vector<MusicTrack> libraryTracks;
    // A counter for incrementing track IDs in the library
    int trackIDCount{ 0 };

    // Local file object to store library CSV data
    juce::File tracksFile{ juce::File::getCurrentWorkingDirectory().getFullPathName() 
        + "\\libraryTracks.csv" };

    // An audio player to read meta info about tracks
    DJAudioPlayer sourceReader;
};


