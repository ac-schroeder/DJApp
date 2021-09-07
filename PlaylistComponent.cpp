/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Aug 2021 1:41:53pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"


PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                                     DeckGUI* _leftDeck,
                                     DeckGUI* _rightDeck)
    : musicLibrary { _formatManager },
      leftDeck { _leftDeck },
      rightDeck { _rightDeck }
{
    // Set custom look and feel
    setLookAndFeel(&mainLookAndFeel);

    // Get the playlist from the music library
    shownTracks = musicLibrary.getTracks();

    // Set the table component's data model
    tableComponent.setModel(this);

    // Create headers for the table
    tableComponent.getHeader().addColumn("File Name", 1, 330);
    tableComponent.getHeader().addColumn("Track Length", 2, 160);
    tableComponent.getHeader().addColumn("", 3, 150);
    tableComponent.getHeader().addColumn("", 4, 150);
    tableComponent.getHeader().addColumn("", 5, 110);

    // Add components
    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(clearPlaylistButton);
    addAndMakeVisible(searchBoxLabel);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(clearSearchButton);
    addAndMakeVisible(playlistMessageBox);
    addAndMakeVisible(tableComponent);

    // Set search box properties
    searchBoxLabel.attachToComponent(&searchBox, true);
    searchBoxLabel.setFont(juce::Font{ 16.0f });
    searchBoxLabel.setText("Search", juce::dontSendNotification);
    searchBox.setJustification(juce::Justification::centredLeft);
    searchBox.setTextToShowWhenEmpty("Search tracks...", juce::Colours::lightgrey);

    // Set playlist message box properties
    playlistMessageBox.setText("Displaying all tracks in your library.",
                                juce::dontSendNotification);

    // Add listeners
    addTrackButton.addListener(this);
    clearPlaylistButton.addListener(this);
    searchBox.addListener(this);
    clearSearchButton.addListener(this);
}

PlaylistComponent::~PlaylistComponent()
{
    // Remove this component's look and feel
    setLookAndFeel(nullptr);
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    // Clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // Draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    // Set text style
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
}

void PlaylistComponent::resized()
{
    auto area = getLocalBounds();
    // Dimensions
    double topBarHeight = area.getHeight() / 9;
    double leftButtonWidth = area.getWidth() / 5;
    double searchBoxWidth = area.getWidth() / 5;
    double clearSearchButtonWidth = area.getWidth() / 6;
    double messageBarHeight = area.getHeight() / 11;
    // Top bar area
    auto topBar = area.removeFromTop(topBarHeight);
    // Top bar components
    addTrackButton.setBounds(topBar.removeFromLeft(leftButtonWidth));
    clearPlaylistButton.setBounds(topBar.removeFromLeft(leftButtonWidth));
    clearSearchButton.setBounds(topBar.removeFromRight(clearSearchButtonWidth));
    searchBox.setBounds(topBar.removeFromRight(searchBoxWidth).reduced(1));
    // Message bar component
    playlistMessageBox.setBounds(area.removeFromTop(messageBarHeight));
    // Table component
    tableComponent.setBounds(area);
}

int PlaylistComponent::getNumRows()
{
    // Use the size of the trackTitles vector to determine number of rows
    return shownTracks.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected) 
{
    // Highlight selected row as orange
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

// Draws cell contents for non-custom component columns
void PlaylistComponent::paintCell(juce::Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    // Draw the track titles down the first column
    if(columnId == 1)
    {
        g.drawText(shownTracks[rowNumber].getFileName(),
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
    // Draw the track lengths down the second column
    if (columnId == 2)
    {
        g.drawText(shownTracks[rowNumber].getLength(),
            2, 0,
            width - 4, height,
            juce::Justification::centredLeft,
            true);
    }
}

// Draws cell contents that contain custom components
juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    juce::Component* existingComponentToUpdate)
{
    // Create custom component deck loading buttons in the 3rd and 4th columns
    if (columnId == 3)
    {
        // Make 'left deck' load buttons
        if (existingComponentToUpdate == nullptr)
        {
            // Create a text button component for the left deck
            juce::TextButton* leftDeckButton = new juce::TextButton{ "Left Deck" };
            juce::String id { shownTracks[rowNumber].getTrackID() };
            juce::String name{ "leftDeck" };
            leftDeckButton->setComponentID(id);
            leftDeckButton->setName(name);

            // Assign it the button listener
            leftDeckButton->addListener(this);

            // Save it as the existing component to update, to return below
            existingComponentToUpdate = leftDeckButton;
        }
        // Refresh the component ID for existing buttons.
        // This is necessary because which track is in the row can change.
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].getTrackID() };
            existingComponentToUpdate->setComponentID(id);
        }
    }
    if (columnId == 4)
    {
        // Make 'right deck' load buttons
        if (existingComponentToUpdate == nullptr)
        {
            // Create a text button component for the right deck
            juce::TextButton* rightDeckButton = new juce::TextButton{ "Right Deck" };
            juce::String id{ shownTracks[rowNumber].getTrackID() };
            juce::String name{ "rightDeck" };
            rightDeckButton->setComponentID(id);
            rightDeckButton->setName(name);

            // Assign it the button listener
            rightDeckButton->addListener(this);

            existingComponentToUpdate = rightDeckButton;
        }
        // Refresh the component ID for existing buttons
        // This is necessary because which track is in the row can change
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].getTrackID() };
            existingComponentToUpdate->setComponentID(id);
        }
    }
    // Create custom component 'remove track' buttons in the 5th column
    if (columnId == 5)
    {
        if (existingComponentToUpdate == nullptr)
        {
            // Create a button to remove the track
            juce::TextButton* removeTrackButton = new juce::TextButton{ "Remove Track" };
            juce::String id{ shownTracks[rowNumber].getTrackID() };
            juce::String name{ "removeTrack" };
            removeTrackButton->setComponentID(id);
            removeTrackButton->setName(name);

            // Assign it the button listener
            removeTrackButton->addListener(this);

            existingComponentToUpdate = removeTrackButton;
        }
        // Refresh the component ID for existing buttons
        // This is necessary because which track is in the row can change
        if (existingComponentToUpdate->isVisible())
        {
            juce::String id{ shownTracks[rowNumber].getTrackID() };
            existingComponentToUpdate->setComponentID(id);
        }
    }
    // Return the custom component, or nullptr if irrelevant column
    return existingComponentToUpdate;
}

// Processes all buttons in the playlist component, both in the
// top bar, and in the table columns.
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // 'Add Track' button
    if (button == &addTrackButton)
    {
        // Create a file chooser GUI for the user to select a file
        juce::FileChooser chooser{ "Select a track to load..." };
        // If the user selects a file to open, load the file
        if (chooser.browseForFileToOpen())
        {
            // Clear any active search, so full library can be seen
            clearSearch();

            // Convert the chosen file to a URL and load it
            juce::URL audioURL { chooser.getResult() };
            musicLibrary.addTrack(audioURL);

            // Update playlist to show the new track
            shownTracks = musicLibrary.getTracks();
            tableComponent.updateContent();
        }
    }
    // 'Clear Playlist' button
    else if (button == &clearPlaylistButton)
    {
        // Only clear if not already empty
        if (!shownTracks.empty())
        {
            // Set alert messages
            juce::String alertTitle{ "Clear Playlist" };
            juce::String alert{ "Are you sure you want to clear the playlist?" };  
            // Alert the user with a confirmation dialog box
            juce::AlertWindow::showOkCancelBox(juce::AlertWindow::AlertIconType::QuestionIcon, 
                alertTitle, alert, "", "", this->getParentComponent(),
                juce::ModalCallbackFunction::create([this](int response) {
                    // Only clear the library if the user confirms
                    if (response == 1)
                    {
                        musicLibrary.clearLibrary();
                        refreshPlaylist();
                    }
                })
            );
        }
    }
    // 'Clear Search' button
    else if (button == &clearSearchButton)
    {
        // Clear the search and re-display whole library
        clearSearch();
    }
    // 'Remove Track' buttons in the tableComponent
    else if (button->getName() == "removeTrack")
    {
        // Get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();

        // Remove the track from the music library
        musicLibrary.removeTrack(trackID);

        // Refresh the table
        refreshPlaylist();
    }
    // 'Left Deck' loading buttons in the tableComponent
    else if (button->getName() == "leftDeck")
    {
        // Get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();

        // Load the relevant track from the music library
        try {
            // Get the track from the library
            MusicTrack track{ musicLibrary.getTrack(trackID) };

            // Load the file to the correct deck
            leftDeck->loadURL(track.getAudioURL(), track.getFileName());
        }        
        catch(const std::exception& e) // there was an error looking up the track
        {
            DBG(e.what());
        }
    }
    // 'Right Deck' loading buttons in the tableComponent
    else if (button->getName() == "rightDeck")
    {
        // Get the track id from the component id on the button
        int trackID = button->getComponentID().getIntValue();

        // Load the relevant track from the music library
        try {
            // Get the track from the library
            MusicTrack track{ musicLibrary.getTrack(trackID) };

            // Load the file to the correct deck
            rightDeck->loadURL(track.getAudioURL(), track.getFileName());
        }
        catch (const std::exception& e) // there was an error looking up the track
        {
            DBG(e.what());
        }
    }
}

// Called when a search term is entered to search the playlist for tracks.
void PlaylistComponent::textEditorReturnKeyPressed(juce::TextEditor& textEditor)
{
    // Get the search term
    juce::String searchText = textEditor.getText();

    // If search box text is deleted and entered, clear search results
    if (textEditor.isEmpty())
    {
        // Clear the search and revert to displaying whole library
        clearSearch();
    }
    // Otherwise, search for the entered term
    else
    {   
        // Check the library for matching tracks
        std::vector<MusicTrack> matchedTracks = musicLibrary.searchLibrary(searchText);

        // Display results
        if (!matchedTracks.empty())
        {
            // Update the playlist to show the search results
            shownTracks.clear(); 
            shownTracks = matchedTracks;
            tableComponent.updateContent();

            // Repaint to update row data in case of consecutive searches, 
            // ... which doesn't change the number of rows and therefore will not 
            // ... be updated by updateContent()
            repaint();

            // Display success message
            playlistMessageBox.setText("Displaying search results...",
                juce::dontSendNotification);
        }
        // If no results were found, show message
        else
        {
            playlistMessageBox.setText("No results for your search were found. Displaying all tracks in your library.", juce::dontSendNotification);

            // Show full music library 
            refreshPlaylist();
        }
    }
}

void PlaylistComponent::clearSearch()
{
    // Clear the search results and revert to showing all tracks
    refreshPlaylist();

    // Update the playlist message
    playlistMessageBox.setText("Displaying all tracks in your library.",
        juce::dontSendNotification);

    // Clear the search term from the search box
    searchBox.setText("");
}

void PlaylistComponent::refreshPlaylist()
{
    shownTracks.clear();                        // Clear the tracks
    shownTracks = musicLibrary.getTracks();     // Get fresh set from the library
    tableComponent.updateContent();             // Update the table
}


