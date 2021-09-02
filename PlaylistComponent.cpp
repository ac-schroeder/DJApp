/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Aug 2021 1:41:53pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                                     DeckGUI* _leftDeck,
                                     DeckGUI* _rightDeck)
    : musicLibrary { _formatManager },
      leftDeck { _leftDeck },
      rightDeck { _rightDeck }
{
    // set the table component's data model
    tableComponent.setModel(this);

    // populate the shownTracks with the Music Library tracks
    shownTracks = musicLibrary.getTracks();

    // create headers for the table
    tableComponent.getHeader().addColumn("File Name", 1, 400);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 100);
    tableComponent.getHeader().addColumn("", 5, 100);

    // make components visible
    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(playlistMessageBox);
    addAndMakeVisible(tableComponent);

    // add clear search button, but do not make visible yet
    addChildComponent(clearSearchButton);

    // set search box properties
    searchBox.setJustification(juce::Justification::centred);
    searchBox.setTextToShowWhenEmpty("SEARCH TRACKS...", juce::Colours::white);

    // set playlist message box properties
    playlistMessageBox.setText("Displaying all tracks in your library.",
                                juce::NotificationType::dontSendNotification);

    // add listeners
    addTrackButton.addListener(this);
    searchBox.addListener(this);
    clearSearchButton.addListener(this);

}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized()
{
    double topBarHeight = getHeight() / 10;
    double addTrackButtonWidth = getWidth() / 3;
    double searchBoxWidth = getWidth() / 3;
    double clearSearchButtonWidth = getWidth() / 3;

    addTrackButton.setBounds(0, 0, 
                        addTrackButtonWidth, topBarHeight);
    searchBox.setBounds(getWidth() - searchBoxWidth, 0, 
                        searchBoxWidth, topBarHeight);
    playlistMessageBox.setBounds(0, topBarHeight,
                        getWidth() - 80, topBarHeight);
    clearSearchButton.setBounds(getWidth() - clearSearchButtonWidth, topBarHeight, 
                        clearSearchButtonWidth, topBarHeight);
    tableComponent.setBounds(0, topBarHeight * 2, 
                        getWidth(), getHeight() - (topBarHeight*2));
}

int PlaylistComponent::getNumRows()
{
    // use the size of the trackTitles vector to determine number of rows
    return shownTracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected) 
{
    // highlight selected row as orange
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    // draw the track titles down the first column
    if(columnId == 1)
    {
        // draw the track title in the row that matches the index of the track in the shownTracks vector
        g.drawText(shownTracks[rowNumber].fileName,
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
    // draw the track lengths down the second column
    if (columnId == 2)
    {
        // draw the track length
        g.drawText(shownTracks[rowNumber].length,
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    // create custom component play buttons in the 3rd and 4th columns
    if (columnId == 3)
    {
        // check that a custom component does not already exist
        if (existingComponentToUpdate == nullptr)
        {
            // create a text button component for the left deck
            juce::TextButton* leftDeckButton = new juce::TextButton{ "Left Deck" };

            // give it an ID matching the track file name
            juce::String id { shownTracks[rowNumber].fileName };
            leftDeckButton->setComponentID(id);

            // assign it the button listener
            leftDeckButton->addListener(this);

            // save it as the existing component to update, to return below
            existingComponentToUpdate = leftDeckButton;
        }
        //else
        //{
        //    // slack comment that this helps with scrolling the table?? he said "i update its properties such as lambda callback so it is redrawn for hte rows"
        //    tableComponent.updateContent();
        //}
    }
    if (columnId == 4)
    {
        // check that a custom component does not already exist
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* rightDeckButton = new juce::TextButton{ "Right Deck" };
            juce::String id{ shownTracks[rowNumber].fileName };
            rightDeckButton->setComponentID(id);

            // assign it the button listener
            rightDeckButton->addListener(this);

            existingComponentToUpdate = rightDeckButton;
        }
    }
    if (columnId == 5)
    {
        // check that a custom component does not already exist
        if (existingComponentToUpdate == nullptr)
        {
            // create a button to remove the track
            juce::TextButton* removeTrackButton = new juce::TextButton{ "Remove Track" };
            juce::String id{ shownTracks[rowNumber].fileName };
            removeTrackButton->setComponentID(id);

            // assign it the button listener
            removeTrackButton->addListener(this);

            existingComponentToUpdate = removeTrackButton;
        }
    }
    // return the custom component, or nullptr if the second column
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // add track button
    if (button == &addTrackButton)
    {
        DBG("add track button was clicked");

        // create a file chooser GUI for the user to select a file
        juce::FileChooser chooser{ "Select a track to load..." };
        // if the user selects a file to open, load the file
        if (chooser.browseForFileToOpen())
        {
            // clear any active search, so full library can be seen
            clearSearch();

            // convert the chosen file to a URL and load it
            /*auto audioURL = juce::URL{ chooser.getResult() };*/
            juce::URL audioURL { chooser.getResult() };
            musicLibrary.addTrack(audioURL);

            // update playlist to show the new track
            shownTracks = musicLibrary.getTracks();
            tableComponent.updateContent();
        }
    }
    // clear search button
    else if (button == &clearSearchButton)
    {
        // clear the search and re-display whole library
        clearSearch();
    }
    // track removal buttons in the tableComponent
    else if (button->getButtonText() == "Remove Track")
    {
        DBG("I'm a track removal button!");

        // get the track number from the component id on the button
        juce::String fileName = button->getComponentID();
        musicLibrary.removeTrack(fileName);

        // refresh the table
        shownTracks.clear();
        shownTracks = musicLibrary.getTracks();
        tableComponent.updateContent();

    }
    // deck loading buttons in the tableComponent
    else
    {
        // get the track number from the component id on the button
        juce::String fileName = button->getComponentID();

        // get the button text to see which deck it is for
        DeckGUI* deckToUse{ nullptr };
        if (button->getButtonText() == "Left Deck")
        {
            deckToUse = leftDeck;
        }
        else if (button->getButtonText() == "Right Deck")
        {
            deckToUse = rightDeck;
        }

        // get the relevant track from the music library to load
        MusicTrack* track { musicLibrary.getTrack(fileName) };
        if (track != nullptr) // track was found
        {
            DBG("Pointer points to track with name " + track->fileName);

            // load the file to the correct deck
            deckToUse->loadURL(track->audioURL, track->fileName);
        }
        else // there was an error looking up the track
        {
            DBG("Track could not be found");
            // TODO: throw exception
        }
    }
}

/** Implement TextEditor Listener */
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    juce::String searchText = textEditor.getText();

    // if search box text is deleted and entered, clear search results
    if (textEditor.isEmpty())
    {
        // clear the search and revert to displaying whole library
        clearSearch();
    }
    // search for the entered term
    else
    {
        // try to find a track in the library to match the search input
        MusicTrack* track = musicLibrary.getTrack(searchText);  
        // if a matching track was found, display results
        if (track != nullptr)
        {
            DBG("A track was found! " + track->fileName);

            // update the playlist to show the search results
            shownTracks.clear(); 
            shownTracks.push_back(*track);
            tableComponent.updateContent();

            // repaint to update row data in case of consecutive searches, which doesn't change the number of rows and therefore will not be updated by updateContent()
            repaint();

            // display success message
            playlistMessageBox.setText("Displaying search results...",
                juce::NotificationType::dontSendNotification);

            // show the 'clear search' button
            clearSearchButton.setVisible(true);
        }
        // if no results were found, show message
        else
        {
            DBG("There was no match for your search: " + searchText);

            // display search failure message
            playlistMessageBox.setText("No results for your search were found. Displaying all tracks in your library.", juce::NotificationType::dontSendNotification);

            // show full music library 
            shownTracks.clear();
            shownTracks = musicLibrary.getTracks();
            tableComponent.updateContent();
        }
    }
}

void PlaylistComponent::clearSearch()
{
    // clear the search results and revert to showing all tracks
    shownTracks.clear();
    shownTracks = musicLibrary.getTracks();
    tableComponent.updateContent();

    // update the playlist message
    playlistMessageBox.setText("Displaying all tracks in your library.",
        juce::NotificationType::dontSendNotification);

    // clear the search term from the search box
    searchBox.setText("");

    // hide the 'clear search' button again
    clearSearchButton.setVisible(false);
}