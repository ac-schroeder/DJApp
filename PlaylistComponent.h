/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Aug 2021 1:41:53pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** Implement TableListBoxModel */
    int getNumRows() override;
    /** Draw background rows in the table */
    void paintRowBackground(juce::Graphics&, 
                    int rowNumber, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;
    /** Draw contents of individual cells */
    void paintCell(juce::Graphics&, 
                    int rowNumber, 
                    int columnId, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;
    /**  */
    juce::Component* refreshComponentForCell(int rowNumber, 
                    int columnId, 
                    bool isRowSelected, 
                    juce::Component* existingComponentToUpdate) override;

    /** Implement Button Listener */
    void buttonClicked(juce::Button* button) override;

private:
    // the table component for the playlist track info and play buttons
    juce::TableListBox tableComponent;

    // a vector of track title strings
    std::vector<std::string> trackTitles;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
