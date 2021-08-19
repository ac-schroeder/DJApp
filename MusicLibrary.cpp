/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#include "MusicLibrary.h"

MusicTrack::MusicTrack(juce::String _fileName, juce::URL _audioURL, std::string _length)
    : fileName { _fileName }, audioURL { _audioURL }, length { _length }
{

}

MusicLibrary::MusicLibrary(juce::AudioFormatManager& _formatManager)
    : trackPlayer { _formatManager }
{

}

MusicLibrary::~MusicLibrary()
{

}

// can this be a vector of pointers to the MusicTrack objects?
std::vector<MusicTrack> MusicLibrary::getTracks()
{
    return libraryTracks;
}
// can this be a pointer to a MusicTrack?
MusicTrack* MusicLibrary::getTrack(std::string keyword)
{
    MusicTrack* track{ nullptr };
    return track;
}

void MusicLibrary::addTrack(juce::URL audioURL)
{
    // audioURL.toString(false) outputs:
    // file:///C%3A/Users/alana/Documents/SchoolCode/OOP-JUCE/Tracks/tracks/bad_frog.mp3

    // get the filename from the URL
    juce::String fileName = audioURL.getFileName();
    DBG("I was called!" + fileName);

    // read the URL into the trackPlayer audio source reader and get its length
    trackPlayer.loadURL(audioURL);
    std::string length{ trackPlayer.getTrackLength() };
    DBG("The track length is " + length);

    // create a new track object and add a pointer to it to the libraryTracks list
    // NOTE: if you want to use pointers, use smart pointers, or else delete!!
    // MusicTrack* track{ new MusicTrack{fileName, audioURL, length} };
    MusicTrack track{ fileName, audioURL, length };
    libraryTracks.push_back(track);
}
void MusicLibrary::saveLibrary()
{

}
void MusicLibrary::loadLibrary()
{

}