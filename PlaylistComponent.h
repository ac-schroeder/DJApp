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


class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    /** 
     * Constructor 
     *
     * @param _formatManager - Reference to the shared audio format manager
     *                         for the app. Used to create a source reader 
     *                         for playlist tracks.
     * @param _leftDeck      - Pointer to the left deck GUI. Used to load
     *                         tracks from the playlist to the left deck.
     * @param _rightDeck     - Pointer to the right deck GUI. Used to load
     *                         tracks from the playlist to the right deck.
     */
    PlaylistComponent(juce::AudioFormatManager& _formatManager, 
                      DeckGUI* _leftDeck, 
                      DeckGUI* _rightDeck);

    /** 
     * Destructor 
     */
    ~PlaylistComponent() override;

    /**
     * Implements Component: Draws the component's content, as well as any child
     * components' content.
     *
     * @param g - The graphics context of the component.
     */
    void paint (juce::Graphics&) override;

    /**
     * Implements Component: Called when the component is resized. Used to
     * control layout of child components.
     */
    void resized() override;


private:
    /** 
     * Implements TableListBoxModel: Gets the current number of rows. 
     *
     * @return - The number of rows currently in the table.
     */
    int getNumRows() override;

    /** 
     * Implements TableListBoxModel: Draws background of a row in the table. 
     *
     * @param g             - The graphics context of the row
     * @param rowNumber     - The number of the row 
     * @param width         - The width of the graphics context
     * @param height        - The height of the graphics context
     * @param rowIsSelected - Whether the row is selected
     */
    void paintRowBackground(juce::Graphics& g, 
                    int rowNumber, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;

    /** 
     * Implements TableListBoxModel: Draws contents of individual cells. 
     *
     * @param g             - The graphics context
     * @param rowNumber     - The number of the row
     * @param columnId      - The ID of the column
     * @param width         - The width of the graphics context
     * @param height        - The height of the graphics context
     * @param rowIsSelected - Whether the row is selected
     */
    void paintCell(juce::Graphics& g, 
                    int rowNumber, 
                    int columnId, 
                    int width, 
                    int height, 
                    bool rowIsSelected) override;

    /** 
     * Implements TableListBoxModel: Draws custom components in cells. 
     * This is used to draw deck loading and track removal buttons.
     * 
     * @param rowNumber                 - The number of the row
     * @param columnId                  - The ID of the column
     * @param isRowSelected             - Whether the row is selected
     * @param existingComponentToUpdate - A pointer to the custom component to 
     *     draw in the cell, or else a nullptr if no custom component is desired
     */
    juce::Component* refreshComponentForCell(int rowNumber, 
                    int columnId, 
                    bool isRowSelected, 
                    juce::Component* existingComponentToUpdate) override;

    /**
     * Implements Button::Listener: Processes button clicks.
     *
     * @param button - Pointer to the button that was clicked.
     */
    void buttonClicked(juce::Button* button) override;

    /** 
     * Implements TextEditor Listener: Processes entered text in the search box. 
     *
     * @param textEditor - The text editor component that triggered the event.
     */
    void textEditorReturnKeyPressed(juce::TextEditor& textEditor) override;

    /** 
     * Clears the search box and any shown search results. 
     */
    void clearSearch();

    /** 
     * Refreshes the playlist displayed in the tableComponent. 
     */
    void refreshPlaylist();

    // The user's music library 
    MusicLibrary musicLibrary;
    // The playlist's shown tracks displayed in the table component
    std::vector<MusicTrack> shownTracks;
    // Pointers to the deck GUI components, for loading tracks
    DeckGUI* rightDeck;
    DeckGUI* leftDeck;
    // Pointer for a file chooser to add tracks
    std::unique_ptr<juce::FileChooser> chooser;
    // Path to home directory for selecting tracks to add
    juce::File homeDirectory { 
        juce::File::getSpecialLocation(juce::File::userHomeDirectory) };
    // Custom look and feel
    MainLookAndFeel mainLookAndFeel;

    /*------------- Child Components ------------*/
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
