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
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager& _formatManager)
    : musicLibrary { _formatManager }
{
    // set the table component's data model
    // this tells the component to call the TableListBoxModel functions
    tableComponent.setModel(this);

    // add some example track titles to make up a playlist
    //trackTitles.push_back("Track 1");
    //trackTitles.push_back("Track 2");

    // populate the shownTracks with the Music Library tracks
    shownTracks = musicLibrary.getTracks();

    // create headers for the table
    tableComponent.getHeader().addColumn("File Name", 1, 400);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 100);

    // make the Add Track button visible
    addAndMakeVisible(addTrackButton);
    // make the table component visible
    addAndMakeVisible(tableComponent);

    // add main controls button listeners
    addTrackButton.addListener(this);
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
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    double controlsHeight = getHeight() / 10;
    double controlButtonWidth = getWidth() / 4;
    addTrackButton.setBounds(controlButtonWidth * 3, 0, controlButtonWidth, controlsHeight);
    tableComponent.setBounds(0, controlsHeight, getWidth(), getHeight() - controlsHeight);
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
            // create a text button component with the row number as its ID
            juce::TextButton* leftDeckButton = new juce::TextButton{ "Left Deck" };
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

        DBG("Button clicked! Button Component ID: " + fileName);
        DBG("Button clicked! Track File Name: " + fileName);

        // get the button component name: that is the deck GUI to call
            // call that deckGUI pointer's loadURL function


        // get the button component ID: that is the track title
            // use it to look up the audioURL of that track 
                // use getTrack(keyword) and then you have the track object
            // pass it to the deckGUI pointer's loadURL function

        // QUESTION: Shouldn't this work? it's a pointer to the real track, but the pointer is in scope of this function, so the pointer should be deleted, but the track would persist, to be found again next time the button is clicked. No???
        
        MusicTrack* track { musicLibrary.getTrack(fileName) };
        if (track != nullptr)
        {
            DBG("Pointer points to track with name " + track->fileName);
        }
        else
        {
            DBG("I'm a null pointer");
        }
       

        // QUESTION TO GOOGLE: do smart pointers delete the referenced object when the POINTER goes out of scope?
        // this makes a smart pointer pointing to the actual track. it should destroy the track after this scope ends?
        /*std::unique_ptr<MusicTrack> track{ musicLibrary.getTrack(fileName) };
        if (track != nullptr)
        {
            DBG("Pointer points to track with name " + track->fileName);
        }*/
    }
}