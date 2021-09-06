/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Aug 2021 1:41:53pm
    Author:  Alana Schroeder

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
    // set look and feel
    setLookAndFeel(&mainLookAndFeel);

    // get playlist from the music library
    shownTracks = musicLibrary.getTracks();

    // set the table component's data model
    tableComponent.setModel(this);
    // create headers for the table
    tableComponent.getHeader().addColumn("File Name", 1, 330);
    tableComponent.getHeader().addColumn("Track Length", 2, 160);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);
    tableComponent.getHeader().addColumn("", 5, 110);

    // make components visible
    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(clearPlaylistButton);
    addAndMakeVisible(searchBoxLabel);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(clearSearchButton);
    addAndMakeVisible(playlistMessageBox);
    addAndMakeVisible(tableComponent);

    // set search box properties
    searchBoxLabel.attachToComponent(&searchBox, true);
    searchBoxLabel.setFont(juce::Font{ 16.0f });
    searchBoxLabel.setText("Search", juce::dontSendNotification);
    searchBox.setJustification(juce::Justification::centredLeft);
    searchBox.setTextToShowWhenEmpty("Search tracks...", juce::Colours::lightgrey);
    // searchBox.setIndents(5,5);

    // set playlist message box properties
    playlistMessageBox.setText("Displaying all tracks in your library.",
                                juce::dontSendNotification);
    //playlistMessageBox.setFont(juce::Font{ 13.0f });

    // add listeners
    addTrackButton.addListener(this);
    clearPlaylistButton.addListener(this);
    searchBox.addListener(this);
    clearSearchButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    setLookAndFeel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    // draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    // set text style
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds();

    // dimensions
    double topBarHeight = area.getHeight() / 9;
    double leftButtonWidth = area.getWidth() / 5;
    double searchBoxWidth = area.getWidth() / 5;
    double clearSearchButtonWidth = area.getWidth() / 6;
    double messageBarHeight = area.getHeight() / 11;

    // top bar area
    auto topBar = area.removeFromTop(topBarHeight);
    
    // top bar components
    addTrackButton.setBounds(topBar.removeFromLeft(leftButtonWidth));
    clearPlaylistButton.setBounds(topBar.removeFromLeft(leftButtonWidth));
    clearSearchButton.setBounds(topBar.removeFromRight(clearSearchButtonWidth));
    searchBox.setBounds(topBar.removeFromRight(searchBoxWidth).reduced(1));

    // message bar component
    playlistMessageBox.setBounds(area.removeFromTop(messageBarHeight));

    // table component
    tableComponent.setBounds(area);
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
        // make 'left deck' load button in rows with content
        if (existingComponentToUpdate == nullptr)
        {
            // create a text button component for the left deck
            juce::TextButton* leftDeckButton = new juce::TextButton{ "Left Deck" };
            juce::String id { shownTracks[rowNumber].trackID };
            juce::String name{ "leftDeck" };
            leftDeckButton->setComponentID(id);
            leftDeckButton->setName(name);

            // assign it the button listener
            leftDeckButton->addListener(this);

            // save it as the existing component to update, to return below
            existingComponentToUpdate = leftDeckButton;
        }
        // refresh the component ID for existing buttons
        // this is necessary because which track is in the row can change
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].trackID };
            existingComponentToUpdate->setComponentID(id);
        }
        //else
        //{
        //    // slack comment that this helps with scrolling the table?? he said "i update its properties such as lambda callback so it is redrawn for hte rows"
        //    tableComponent.updateContent();
        //}
    }
    if (columnId == 4)
    {
        // make 'right deck' load button in rows with content
        if (existingComponentToUpdate == nullptr)
        {
            juce::TextButton* rightDeckButton = new juce::TextButton{ "Right Deck" };
            juce::String id{ shownTracks[rowNumber].trackID };
            juce::String name{ "rightDeck" };
            rightDeckButton->setComponentID(id);
            rightDeckButton->setName(name);

            // assign it the button listener
            rightDeckButton->addListener(this);

            existingComponentToUpdate = rightDeckButton;
        }
        // refresh the component ID for existing buttons
        // this is necessary because which track is in the row can change
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].trackID };
            existingComponentToUpdate->setComponentID(id);
        }
    }
    if (columnId == 5)
    {
        // make 'remove track' button in rows with content
        if (existingComponentToUpdate == nullptr)
        {
            // create a button to remove the track
            juce::TextButton* removeTrackButton = new juce::TextButton{ "Remove Track" };
            juce::String id{ shownTracks[rowNumber].trackID };
            juce::String name{ "removeTrack" };
            removeTrackButton->setComponentID(id);
            removeTrackButton->setName(name);

            // assign it the button listener
            removeTrackButton->addListener(this);

            existingComponentToUpdate = removeTrackButton;
        }
        // refresh the component ID for existing buttons
        // this is necessary because which track is in the row can change
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].trackID };
            existingComponentToUpdate->setComponentID(id);
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
        // create a file chooser GUI for the user to select a file
        juce::FileChooser chooser{ "Select a track to load..." };
        // if the user selects a file to open, load the file
        if (chooser.browseForFileToOpen())
        {
            // clear any active search, so full library can be seen
            clearSearch();

            // convert the chosen file to a URL and load it
            juce::URL audioURL { chooser.getResult() };
            musicLibrary.addTrack(audioURL);

            // update playlist to show the new track
            shownTracks = musicLibrary.getTracks();
            tableComponent.updateContent();
        }
    }
    // clear playlist button
    else if (button == &clearPlaylistButton)
    {
        if (!shownTracks.empty())
        {
            // set alert messages
            juce::String alertTitle{ "Clear Playlist" };
            juce::String alert{ "Are you sure you want to clear the playlist?" };  
            // alert the user with a confirmation dialog box
            juce::AlertWindow::showOkCancelBox(juce::AlertWindow::AlertIconType::QuestionIcon, 
                alertTitle, alert, "", "", this->getParentComponent(),
                juce::ModalCallbackFunction::create([this](int response) {
                    // only clear the library if the user confirms
                    if (response == 1)
                    {
                        musicLibrary.clearLibrary();
                        refreshPlaylist();
                    }
                })
            );
        }
    }
    // clear search button
    else if (button == &clearSearchButton)
    {
        // clear the search and re-display whole library
        clearSearch();
    }
    // track removal buttons in the tableComponent
    else if (button->getName() == "removeTrack")
    {
        // get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();
        musicLibrary.removeTrack(trackID);

        // refresh the table
        refreshPlaylist();

    }
    // left deck loading buttons in the tableComponent
    else if (button->getName() == "leftDeck")
    {
        // get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();

        // get the relevant track from the music library to load
        try {
            MusicTrack track{ musicLibrary.getTrack(trackID) };
            DBG("Track found with name " + track.fileName);

            // load the file to the correct deck
            leftDeck->loadURL(track.audioURL, track.fileName);
        }        
        catch(const std::exception& e) // there was an error looking up the track
        {
            DBG(e.what());
        }
    }
    // right deck loading buttons in the tableComponent
    else if (button->getName() == "rightDeck")
    {
        // get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();

        // get the relevant track from the music library to load
        try {
            MusicTrack track{ musicLibrary.getTrack(trackID) };
            DBG("Track found with name " + track.fileName);

            // load the file to the correct deck
            rightDeck->loadURL(track.audioURL, track.fileName);
        }
        catch (const std::exception& e) // there was an error looking up the track
        {
            DBG(e.what());
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
        std::vector<MusicTrack> matchedTracks = musicLibrary.searchLibrary(searchText);
        if (!matchedTracks.empty())
        {
            // update the playlist to show the search results
            shownTracks.clear(); 
            shownTracks = matchedTracks;
            tableComponent.updateContent();

            // repaint to update row data in case of consecutive searches, which doesn't change the number of rows and therefore will not be updated by updateContent()
            repaint();

            // display success message
            playlistMessageBox.setText("Displaying search results...",
                juce::dontSendNotification);
        }
        // if no results were found, show message
        else
        {
            DBG("There was no match for your search: " + searchText);

            // display search failure message
            playlistMessageBox.setText("No results for your search were found. Displaying all tracks in your library.", juce::dontSendNotification);

            // show full music library 
            refreshPlaylist();
        }
    }
}

void PlaylistComponent::clearSearch()
{
    // clear the search results and revert to showing all tracks
    refreshPlaylist();

    // update the playlist message
    playlistMessageBox.setText("Displaying all tracks in your library.",
        juce::NotificationType::dontSendNotification);

    // clear the search term from the search box
    searchBox.setText("");
}


void PlaylistComponent::refreshPlaylist()
{
    shownTracks.clear();                        // clear the tracks
    shownTracks = musicLibrary.getTracks();     // get fresh set from the library
    tableComponent.updateContent();             // update the table
}


