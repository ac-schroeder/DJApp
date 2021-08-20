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

// can this be a vector of pointers to the MusicTrack objects?
std::vector<MusicTrack> MusicLibrary::getTracks()
{
    return libraryTracks;
}

// can this be a pointer to a MusicTrack?
// TODO: switch this to a smart pointer
MusicTrack* MusicLibrary::getTrack(juce::String keyword)
{

    MusicTrack* matchedTrack { nullptr };
    // loop over library tracks to find matching track
    for (MusicTrack& track : libraryTracks)
    {
        if (track.fileName == keyword)
        {
            matchedTrack = &track; // reassign the pointer to the track
            DBG("The matched track exists! track.filename is " + track.fileName);
            //DBG("The pointer points to " + matchedTrack->fileName);
            //DBG("The pointer track has length " + matchedTrack->length);
        }
    }
    return matchedTrack;
}
//std::unique_ptr<MusicTrack> MusicLibrary::getTrack(juce::String keyword)
//{
//    
//    std::unique_ptr<MusicTrack> matchedTrack{ nullptr };
//    DBG("I made it here. do I still have tracks? " + std::to_string(libraryTracks.size()));
//    // loop over library tracks to find matching track
//    for (MusicTrack track : libraryTracks)
//    {
//        DBG("Here is a library track. it's name is " + track.fileName);
//        if (track.fileName == keyword)
//        {
//            DBG("The matched track exists! track.filename is " + track.fileName);
//            // update the smart pointer to own a pointer to the matching track
//            matchedTrack.reset(&track); // reassign the pointer to the track
//            
//            DBG("The pointer points to " + matchedTrack->fileName);
//            DBG("The pointer track has length " + matchedTrack->length);
//        }
//    }
//    return matchedTrack;
//}

void MusicLibrary::addTrack(juce::URL audioURL)
{
    // get the filename from the URL
    juce::String fileName = audioURL.getFileName();

    // read the URL into the trackPlayer audio source reader and get its length
    sourceReader.loadURL(audioURL);
    std::string length{ sourceReader.getTrackLength() };

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