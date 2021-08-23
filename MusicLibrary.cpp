/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#include "MusicLibrary.h"

MusicLibrary::MusicLibrary(juce::AudioFormatManager& _formatManager)
    : sourceReader{ _formatManager }
{

}

MusicLibrary::~MusicLibrary()
{

}

// Returns the full music library of tracks
std::vector<MusicTrack> MusicLibrary::getTracks()
{
    return libraryTracks;
}

// Returns a pointer to the matching track if a match is found, or else nullptr
// Design based on https://stackoverflow.com/questions/2639255/return-a-null-object-if-search-result-not-found
MusicTrack* MusicLibrary::getTrack(juce::String keyword)
{
    // create a null pointer to return as default, for if no tracks found
    MusicTrack* matchedTrack { nullptr };
    // loop over library tracks to find matching track
    for (MusicTrack& track : libraryTracks)
    {
        if (track.fileName == keyword)
        {
            matchedTrack = &track; // reassign the pointer to the track
            DBG("The matched track exists! track.filename is " + track.fileName);
        }
    }
    return matchedTrack;
}

// add a track to the track library
void MusicLibrary::addTrack(juce::URL audioURL)
{
    // get the filename from the URL
    juce::String fileName = audioURL.getFileName();

    // read the URL into the trackPlayer audio source reader and get its length
    sourceReader.loadURL(audioURL);
    std::string length{ sourceReader.getTrackLength() };

    // create a new track object and add to the libraryTracks list
    MusicTrack track{ fileName, audioURL, length };
    libraryTracks.push_back(track);
}

void MusicLibrary::saveLibrary()
{

}

void MusicLibrary::loadLibrary()
{

}