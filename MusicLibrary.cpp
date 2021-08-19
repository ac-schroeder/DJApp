/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#include "MusicLibrary.h"

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
    std::vector<MusicTrack> tracks;

    return tracks;
}
// can this be a pointer to a MusicTrack?
MusicTrack MusicLibrary::getTrack(std::string keyword)
{
    MusicTrack track;
    return track;
}

void MusicLibrary::addTrack(juce::URL audioURL)
{
    // audioURL.toString(false) outputs:
    // file:///C%3A/Users/alana/Documents/SchoolCode/OOP-JUCE/Tracks/tracks/bad_frog.mp3

    // get the filename from the URL
    juce::String fileName = audioURL.getFileName();
    DBG("I was called!" + fileName);

    // get the length of the file
    // read the URL into the trackPlayer audio source reader
    trackPlayer.loadURL(audioURL);
    double length{ trackPlayer.getTrackLength() };
    DBG("The track length is " + std::to_string(length));

}
void MusicLibrary::saveLibrary()
{

}
void MusicLibrary::loadLibrary()
{

}