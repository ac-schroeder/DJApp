#pragma once

#include <JuceHeader.h>

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /** Constructor */
    MainLookAndFeel();
    /** Destructor */
    ~MainLookAndFeel();

    /** 
     * Overrides GroupComponent LookAndFeel: Draws an outline around the group
     * component. 
     *
     * @param g              - The graphics context
     * @param w              - The width of the graphics context
     * @param h              - The height of the graphics context
     * @param text           - The text to display as a heading
     * @param j              - The justification of the text
     * @param groupComponent - The component that owns the graphics context
     */
    void drawGroupComponentOutline(juce::Graphics& g, 
                                   int w, int h, 
                                   const juce::String& text, 
                                   const juce::Justification& j, 
                                   juce::GroupComponent& groupComponent) override;

    /** 
     * Overrides TextEditor LookAndFeel: Draws background of text editor. 
     *
     * @param g          - The graphics context
     * @param width      - The width of the graphics context
     * @param height     - The height of the graphics context
     * @param textEditor - The textEditor component that owns the graphics context
     */
    void fillTextEditorBackground(juce::Graphics& g, 
                                  int width, 
                                  int height, 
                                  juce::TextEditor& textEditor) override;

    /** 
     * Overrides TextEditor LookAndFeel: Draws text editor outline. 
     *
     * @param g          - The graphics context
     * @param width      - The width of the graphics context
     * @param height     - The height of the graphics context
     * @param textEditor - The textEditor component that owns the graphics context
     */
    void drawTextEditorOutline(juce::Graphics& g, 
                               int width, 
                               int height, 
                               juce::TextEditor& textEditor) override;
};