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

    // make components visible
    addAndMakeVisible(addTrackButton);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(searchResultsMessageBox);
    addAndMakeVisible(tableComponent);

    // set search box properties
    searchBox.setJustification(juce::Justification::centred);
    searchBox.setTextToShowWhenEmpty("SEARCH TRACKS...", juce::Colours::white);

    // set search results message box properties
    searchResultsMessageBox.setText("Displaying all tracks.",      
                                    juce::NotificationType::dontSendNotification);
    // TODO: get this stuff into LookandFeel?
    // TODO: how do you make a border?!?!?
    //juce::BorderSize<int> border{ 12 };
    //searchResultsMessageBox.setBorderSize(border);

    // add listeners
    addTrackButton.addListener(this);
    searchBox.addListener(this);

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
    //g.drawText ("PlaylistComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    double topBarHeight = getHeight() / 10;
    double topBarComponentWidth = getWidth() / 3;
    addTrackButton.setBounds(0, 0, topBarComponentWidth, topBarHeight);
    searchBox.setBounds(topBarComponentWidth * 2, 0, topBarComponentWidth, topBarHeight);
    searchResultsMessageBox.setBounds(0, topBarHeight, getWidth(), topBarHeight);
    tableComponent.setBounds(0, topBarHeight * 2, getWidth(), getHeight() - (topBarHeight*2));
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
    // return the custom component, or nullptr if the second column
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addTrackButton)
    {
        DBG("add track button was clicked");

        // create a file chooser GUI for the user to select a file
        juce::FileChooser chooser{ "Select a track to load..." };
        // if the user selects a file to open, load the file
        if (chooser.browseForFileToOpen())
        {
            // convert the chosen file to a URL and load it
            auto audioURL = juce::URL{ chooser.getResult() };
            musicLibrary.addTrack(audioURL);
        }

        // update table
        shownTracks = musicLibrary.getTracks();
        tableComponent.updateContent();
    }
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

        DBG("Button clicked! Button Component ID: " + fileName);
        DBG("Button clicked! Track File Name: " + fileName);
        DBG("Button clicked! Deck to use is " + button->getButtonText());
       
        // get the relevant track from the music library to load
        MusicTrack* track { musicLibrary.getTrack(fileName) };
        if (track != nullptr) // track was found
        {
            DBG("Pointer points to track with name " + track->fileName);

            // load the file to the correct deck
            deckToUse->loadURL(track->audioURL);

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

    // if search box is cleared, display default message and all library tracks
    if (searchText.isEmpty())
    {
        // display search results message
        searchResultsMessageBox.setText("Displaying all tracks.", 
                                        juce::NotificationType::dontSendNotification);

        // update the playlist to show all tracks
        shownTracks.clear();
        shownTracks = musicLibrary.getTracks();
        tableComponent.updateContent();
    }
    else
    {
        // Search for a track matching the search input
        MusicTrack* track = musicLibrary.getTrack(searchText);
        if (track != nullptr)
        {
            DBG("A track was found! " + track->fileName);

            // display search results 
            searchResultsMessageBox.setText("Displaying search results...",
                juce::NotificationType::dontSendNotification);
            shownTracks.clear();
            shownTracks.push_back(*track);
            tableComponent.updateContent();

            // TODO: display clear results button

        }
        else
        {
            DBG("There was no match for your search: " + searchText);

            // display search failure message
            searchResultsMessageBox.setText("No results for your search were found. Displaying all tracks.", juce::NotificationType::dontSendNotification);

            // show all tracks in the playlist
            shownTracks.clear();
            shownTracks = musicLibrary.getTracks();
            tableComponent.updateContent();
        }
    }
}