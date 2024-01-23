#include <algorithm>
#include "MusicLibrary.h"


MusicLibrary::MusicLibrary(juce::AudioFormatManager& _formatManager)
    : sourceReader{ _formatManager }
{
    // Load any previously saved library CSV file
    loadLibrary();

    // Initialise the trackID counter
    // If tracks loaded from the saved library, it starts at the last known ID
    if (!libraryTracks.empty())
    {
        int lastID = libraryTracks[libraryTracks.size() - 1].getTrackID();
        trackIDCount = lastID;
    }
}

MusicLibrary::~MusicLibrary()
{
    // Save the library playlist to CSV
    saveLibrary();
}


std::vector<MusicTrack> MusicLibrary::getTracks()
{
    // Return the full library of tracks
    return libraryTracks;
}

// Called when a track is being loaded to a deck from the playlist. 
// If the track is not found for some reason, throws an exception.
MusicTrack MusicLibrary::getTrack(int _trackID)
{
    // Initialize a MusicTrack pointer to nullptr, in the event the track is not found
    const MusicTrack* matchedTrack{ nullptr };
    
    // Search the music library for the track with the right ID
    for (const MusicTrack& track : libraryTracks)
    {
        if (track.getTrackID() == _trackID)
        {
            // assign the pointer to the track
            matchedTrack = &track;      
        }
    }
    
    // If still nullptr, no match was found
    if (matchedTrack == nullptr)
    {
        throw std::invalid_argument{ "Track could not be found" };
    }

    // Return the matched track
    return *matchedTrack;
}

void MusicLibrary::addTrack(const juce::URL& audioURL)
{
    // Get the next trackID number and increment the counter
    int trackID = ++trackIDCount;

    // Get the filename from the URL
    juce::String fileName = audioURL.getFileName();

    // Read the URL into the audio source reader and get its length
    sourceReader.loadURL(audioURL);
    std::string length{ sourceReader.getTrackLength() };

    // Create a new track object and add to the libraryTracks vector
    MusicTrack track{ trackID, fileName, audioURL, length };
    libraryTracks.push_back(track);
}

// Removes a track from the music library.
// Method from https://stackoverflow.com/questions/8628951/remove-elements-of-a-vector-inside-the-loop
void MusicLibrary::removeTrack(int _trackID)
{
    if (!libraryTracks.empty())
    {
        // loop over the libraryTracks vector and check if the trackID matches
        for (int i = libraryTracks.size() - 1; i >= 0; i--)
        {
            if (libraryTracks.at(i).getTrackID() == _trackID)
            {
                // erase the track using an iterator to this position
                libraryTracks.erase(libraryTracks.begin() + i);
                break;
            }
        }
    }
}

void MusicLibrary::clearLibrary()
{
    // Remove all tracks from the library
    libraryTracks.clear();
}

// Returns a vector of tracks which contain the keyword in their filename
// The keyword is treated as a wildcard pattern
std::vector<MusicTrack> MusicLibrary::searchLibrary(juce::String& keyword)
{
    // Convert keyword to a wildcard pattern
    keyword = "*" + keyword + "*";
    juce::StringRef pattern{ keyword };

    // Set up an empty vector of matching tracks to return
    std::vector<MusicTrack> matchedTracks;

    // Add any tracks that contain the keyword pattern
    for (const MusicTrack& track : libraryTracks)
    {
        if (track.getFileName().matchesWildcard(pattern, true))
        {
            matchedTracks.push_back(track);
        }
    }
    return matchedTracks;
}

void MusicLibrary::saveLibrary()
{
    // Create the CSV file if not already done
    tracksFile.create();

    // Convert file to an output stream to write to
    juce::FileOutputStream output{ tracksFile };
    if (output.openedOk()) // File opened successfully
    {
        // Set to beginning of the stream and clear the file
        output.setPosition(0);
        output.truncate();

        // Save each track to the CSV file
        for (const MusicTrack& track : libraryTracks)
        {
            // Convert the trackID to a string
            juce::String trackID{ track.getTrackID() };
            // Convert the URL to a string
            juce::String audioURL = track.getAudioURL().getLocalFile().getFullPathName();
            // Make a comma-delimited string for the track's properties
            juce::String line = trackID + "," + track.getFileName() + "," 
                                + audioURL + "," + track.getLength() + "\n";
            // Write the line to the CSV file
            output.writeText(line, false, false, "\n");
        }
    }
}

// Loads the music library from previously saved CSV file. 
// Checks that the audio files still exist before adding them back to the library.
void MusicLibrary::loadLibrary()
{
    // If a library CSV file exists..
    if (tracksFile.exists())
    {
        // Convert to an input stream
        juce::FileInputStream input{ tracksFile };
        if (input.openedOk()) // File opened successfully
        {
            // Read line by line to end
            while (!input.isExhausted())
            {
                // Tokenise the line
                juce::String line = input.readNextLine();
                juce::StringArray tokens = juce::StringArray::fromTokens(line, ",", "\"");

                // Convert the URL string to a JUCE File object and verify it exists
                juce::File audioFile{ tokens[2] };
                if (audioFile.exists())     // File still exists
                {
                    // Convert tokens to Music Track data member data types
                    int trackID = tokens[0].getIntValue();
                    juce::String fileName{ tokens[1] };
                    juce::URL audioURL{ audioFile };
                    std::string length = tokens[3].toStdString();

                    // Create the Music Track
                    MusicTrack track{ trackID, fileName, audioURL, length };

                    // Add the track to the libraryTracks vector
                    libraryTracks.push_back(track);
                }
                else    // File has been moved or deleted
                {
                    DBG("File could not be loaded. It no longer exists at this location. File: " + tokens[1]);
                }

            }
        }
    }
}