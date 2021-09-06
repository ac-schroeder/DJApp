/*
  ==============================================================================

    MainLookAndFeel.h
    Created: 1 Sep 2021 3:43:28pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    /** Constructor */
    MainLookAndFeel();
    /** Destructor */
    ~MainLookAndFeel();

    /** Overrides GroupComponent LookAndFeel: Draws an outline around the group. */
    void drawGroupComponentOutline(juce::Graphics& g, int w, int h, const juce::String& text, const juce::Justification&, juce::GroupComponent& groupComponent) override;

    /** Overrides TextEditor LookAndFeel: Draws background of text editor. */
    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

    /** Overrides TextEditor LookAndFeel: Draws text editor outline. */
    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
};