/*
  ==============================================================================

    MainLookAndFeel.h
    Created: 1 Sep 2021 3:43:28pm
    Author:  alana

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class MainLookAndFeel : public juce::LookAndFeel_V4
{
public:
    MainLookAndFeel();
    ~MainLookAndFeel();

    void drawGroupComponentOutline(juce::Graphics& g, int w, int h, const juce::String& text, const juce::Justification&, juce::GroupComponent& groupComponent) override;

    void fillTextEditorBackground(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;

    void drawTextEditorOutline(juce::Graphics& g, int width, int height, juce::TextEditor& textEditor) override;
};