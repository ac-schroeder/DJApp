/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Aug 2021 1:41:53pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <vector>
#include <string>
#include <JuceHeader.h>
#include "MusicLibrary.h"
#include "DeckGUI.h"
#include "MainLookAndFeel.h"


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    /** Constructor */
    PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                      DeckGUI* _leftDeck, 
                      DeckGUI* _rightDeck);
    /** Destructor */
    ~PlaylistComponent() override;

    /** Implements Component: Paints the Playlist component. */
    void paint (juce::Graphics&) override;
    /** Implements Component: Repaints the component on resize. */
    void resized() override;

    /** Implements TableListBoxModel: Gets the current number of rows. */
    int getNumRows() override;
    /** Implements TableListBoxModel: Draws background of rows in the table. */
    void paintRowBackground(juce::Graphics&, 
                    int rowNumber, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;
    /** Implements TableListBoxModel: Draws contents of individual cells. */
    void paintCell(juce::Graphics&, 
                    int rowNumber, 
                    int columnId, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;
    /** 
     * Implements TableListBoxModel: Draws custom components in cells. 
     * This is used to draw deck loading and track removal buttons.
     */
    juce::Component* refreshComponentForCell(int rowNumber, 
                    int columnId, 
                    bool isRowSelected, 
                    juce::Component* existingComponentToUpdate) override;

    /** Implements Button Listener: Processses button clicks. */
    void buttonClicked(juce::Button* button) override;

    /** Implements TextEditor Listener: Processes entered text in the search box. */
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

private:
    /** Clears the search box and any shown search results. */
    void clearSearch();
    /** Refreshes the playlist displayed in the tableComponent. */
    void refreshPlaylist();

    // The user's music library 
    MusicLibrary musicLibrary;
    // The playlist's shown tracks displayed in the table component
    std::vector<MusicTrack> shownTracks;

    // Pointers to the deck GUI components, for loading tracks
    DeckGUI* rightDeck;
    DeckGUI* leftDeck;

    // Custom look and feel
    MainLookAndFeel mainLookAndFeel;

    /*========== Child Components =========*/
    // Top bar components
    juce::TextButton addTrackButton{ "Add Track" };
    juce::TextButton clearPlaylistButton{ "Clear Playlist" };
    juce::Label searchBoxLabel;
    juce::TextEditor searchBox;
    juce::TextButton clearSearchButton{ "Clear Search" };
    // Message bar components
    juce::Label playlistMessageBox;
    // Table component
    juce::TableListBox tableComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
