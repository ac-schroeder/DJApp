/*
  ==============================================================================

    MusicLibrary.cpp
    Created: 19 Aug 2021 10:40:26am
    Author:  alana

  ==============================================================================
*/

#include <algorithm>
#include "MusicLibrary.h"

MusicLibrary::MusicLibrary(juce::AudioFormatManager& _formatManager)
    : sourceReader{ _formatManager }
{
    // set the file path for the music library tracks CSV file
    juce::String filePath = juce::File::getCurrentWorkingDirectory().getFullPathName()
        + "\\libraryTracks.csv";
    tracksFile = filePath;

    // attempt to load a previously saved library CSV file
    loadLibrary();

    // initialise the trackID counter
    if (!libraryTracks.empty())
    {
        int lastID = libraryTracks[libraryTracks.size() - 1].trackID;
        trackIDCount = lastID;
    }
}

MusicLibrary::~MusicLibrary()
{
    saveLibrary();
}

// Returns the full music library of tracks
std::vector<MusicTrack> MusicLibrary::getTracks()
{
    return libraryTracks;
}

// Returns a pointer to the matching track if a match is found, or else nullptr
// Design based on https://stackoverflow.com/questions/2639255/return-a-null-object-if-search-result-not-found
MusicTrack* MusicLibrary::searchLibrary(juce::String keyword)
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

/** Looks up a track in the library by trackID */
MusicTrack MusicLibrary::getTrack(int _trackID)
{
    MusicTrack* matchedTrack{ nullptr };;
    for (MusicTrack& track : libraryTracks)
    {
        if (track.trackID == _trackID)
        {
            matchedTrack = &track;
        }
    }
    
    if (matchedTrack == nullptr)
    {
        throw std::invalid_argument{ "Track could not be found" };
    }

    return *matchedTrack;
}

// add a track to the track library
void MusicLibrary::addTrack(juce::URL audioURL)
{
    // get the next trackID number and increment the counter
    int trackID = ++trackIDCount;

    // get the filename from the URL
    juce::String fileName = audioURL.getFileName();

    // read the URL into the trackPlayer audio source reader and get its length
    sourceReader.loadURL(audioURL);
    std::string length{ sourceReader.getTrackLength() };

    // create a new track object and add to the libraryTracks list
    MusicTrack track{ trackID, fileName, audioURL, length };
    libraryTracks.push_back(track);
}

// Remove a track from the music library
// Technique from https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
void MusicLibrary::removeTrack(int _trackID)
{
    if (!libraryTracks.empty())
    {
        DBG("Attempting to remove track. " + std::to_string(_trackID) + " Library Tracks is not empty");
        for (int i = libraryTracks.size() - 1; i >= 0; i--)
        {
            if (libraryTracks.at(i).trackID == _trackID)
            {
                //DBG("I found trackID " + std::to_string(_trackID) 
                //    + " and the filename here is " + libraryTracks[i].fileName);
                libraryTracks.erase(libraryTracks.begin() + i);
                break;
            }
        }
    }
}

void MusicLibrary::saveLibrary()
{
    // check that there are tracks to save
    if (!libraryTracks.empty())
    {
        // create the CSV file if not already created
        tracksFile.create();

        // convert file to an output stream to write to
        juce::FileOutputStream output{ tracksFile };
        // check that it opened okay
        if (output.openedOk())
        {
            // set to beginning of the stream and clear the file
            output.setPosition(0);
            output.truncate();

            // save each track to the CSV file
            for (MusicTrack& track : libraryTracks)
            {
                // convert the trackID to a string
                juce::String trackID{ track.trackID };
                // convert the URL to a string
                juce::String audioURL = track.audioURL.getLocalFile().getFullPathName();
                // make a comma-delimited string for the track's properties
                juce::String line = trackID + "," + track.fileName + "," 
                                    + audioURL + "," + track.length + "\n";
                // write the line to the CSV file
                output.writeText(line, false, false, "\n");
            }
        }
    }
}

void MusicLibrary::loadLibrary()
{
    // check if the library tracks CSV file already exists
    if (tracksFile.exists())
    {
        // convert to an input stream
        juce::FileInputStream input{ tracksFile };
        // check that it opened okay
        if (input.openedOk())
        {
            // read each line
            while (!input.isExhausted())
            {
                // get a line and tokenise it
                juce::String line = input.readNextLine();
                juce::StringArray tokens = juce::StringArray::fromTokens(line, ",", "\"");

                // convert the URL string to a juce File object and verify it still exists
                juce::File audioFile{ tokens[2] };
                if (audioFile.exists())
                {
                    // convert tokens to Music Track data member data types
                    int trackID = tokens[0].getIntValue();
                    juce::String fileName{ tokens[1] };
                    juce::URL audioURL{ audioFile };
                    std::string length = tokens[3].toStdString();

                    // create the Music Track
                    MusicTrack track{ trackID, fileName, audioURL, length };

                    // add the track to the libraryTracks vector
                    libraryTracks.push_back(track);
                }
                else
                {
                    // alert that file no longer exists
                    DBG("File could not be loaded. It no longer exists at this location. File: " 
                        + tokens[1]);
                }

            }
        }
    }
}