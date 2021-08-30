/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Aug 2021 1:41:53pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <JuceHeader.h>
#include "MusicLibrary.h"
#include "DeckGUI.h"
// #include "MusicTrack.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                      DeckGUI* _leftDeck, 
                      DeckGUI* _rightDeck);
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
    /** Draw custom components in cells */
    juce::Component* refreshComponentForCell(int rowNumber, 
                    int columnId, 
                    bool isRowSelected, 
                    juce::Component* existingComponentToUpdate) override;

    /** Implement Button Listener */
    void buttonClicked(juce::Button* button) override;

    /** Implement TextEditor Listener */
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

private:
    // the table component for the playlist track info and play buttons
    juce::TableListBox tableComponent;
    // the Add Track button to add files to the playlist
    juce::TextButton addTrackButton{ "Add Track" };
    // the Search button to search for tracks in the playlist
    juce::TextEditor searchBox;
    // a button to clear search results
    juce::TextButton clearSearchButton{ "Clear search" };
    // a message box to inform user of search results
    juce::Label playlistMessageBox;

    // the user's music library 
    MusicLibrary musicLibrary;
    // the playlist's shown tracks displayed in the table component
    std::vector<MusicTrack> shownTracks;

    // pointers to the deck GUI components, for loading tracks
    DeckGUI* rightDeck;
    DeckGUI* leftDeck;

    /** Clears the search box and any shown search results */
    void clearSearch();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
