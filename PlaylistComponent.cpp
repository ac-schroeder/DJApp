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
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");

    // create headers for the table
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);

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
    return trackTitles.size();
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
        g.drawText(trackTitles[rowNumber],
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
    // create custom component play buttons in the second column
    if (columnId == 2)
    {
        // check that a custom component does not already exist
        if (existingComponentToUpdate == nullptr)
        {
            // create a text button component with the row number as its ID
            juce::TextButton* button = new juce::TextButton{ "play" };
            juce::String id{ std::to_string(rowNumber) };
            button->setComponentID(id);

            // assign it the button listener
            button->addListener(this);

            // save it as the existing component to update, to return below
            existingComponentToUpdate = button;

            /*
                Instead of the above, create two buttons, one with a name for the left deckGUI, one with a name for the right deckGUI. 

                Then give each button the track title as its ID. 
                    This does mean same ID on two buttons, but I don't care as long as it works.
            */
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
    }
    else
    {
        // get the track number from the component id on the button
        // convert the juce string to a standard string, then to an integer
        int trackID = std::stoi(button->getComponentID().toStdString());

        DBG("Button clicked! Button Component ID: " + button->getComponentID());
        DBG("Button clicked! Track ID: " + std::to_string(trackID));
        DBG("Button clicked! Track Title: " + trackTitles[trackID]);

        // get the button component name: that is the deck GUI to call
            // call that deckGUI pointer's loadURL function

        // get the button component ID: that is the track title
            // use it to look up the audioURL of that track 
                // use getTrack(keyword) and then you have the track object
            // pass it to the deckGUI pointer's loadURL function
    }
}