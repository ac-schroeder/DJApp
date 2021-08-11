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
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // set the table component's data model
    // this tells the component to call the TableListBoxModel functions
    tableComponent.setModel(this);

    // add some example track titles to make up a playlist
    trackTitles.push_back("Track 1");
    trackTitles.push_back("Track 2");

    // create headers for the table
    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("", 2, 200);

    // make the table component visible
    addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

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
    // This method is where you should set the bounds of any child
    // components that your component contains..

    tableComponent.setBounds(0, 0, getWidth(), getHeight());

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
        }
    }
    // return the custom component, or nullptr if the second column
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    // get the track number from the component id on the button
    // convert the juce string to a standard string, then to an integer
    int trackID = std::stoi(button->getComponentID().toStdString());

    DBG("Button clicked! Button Component ID: " + button->getComponentID());
    DBG("Button clicked! Track ID: " + std::to_string(trackID));
    DBG("Button clicked! Track Title: " + trackTitles[trackID]);
}