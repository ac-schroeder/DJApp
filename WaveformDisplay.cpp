/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 5 Aug 2021 1:33:56pm
    Author:  Alana Schroeder

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"


WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : audioThumb {           
        1000,                   // image resolution
        formatManagerToUse,     // shared audio format manager 
        cacheToUse },           // shared AudioThumbnailCache 
      fileLoaded { false }    
{
    // Register change listener
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    // Clear the background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Draw an outline around the component
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);   

    // Set draw colour to orange
    g.setColour(juce::Colours::orange);

    // Draw the waveform only if file is loaded to the deck
    if (fileLoaded)
    {
        // Draw the waveform
        audioThumb.drawChannel(g, getLocalBounds(),
            0, audioThumb.getTotalLength(),
            0, 1.0f);

        // Draw the playhead indicator
        g.setColour(juce::Colours::lightgreen);
        juce::Rectangle<int> playheadBar(relativePosition * getWidth(), 
                                         0, 4, getHeight());
        g.fillRect(playheadBar);
    }
    else
    {
        // Draw placeholder text
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);
    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(const juce::URL& audioURL)
{
    // Clear previous drawings
    audioThumb.clear();

    // Set the audioURL as the input source for the audio thumbnail
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
}

void WaveformDisplay::setPositionRelative(double  _relativePosition)
{
    // Only change if there is change to show
    if (_relativePosition != relativePosition)
    {
        // Update the relative position
        relativePosition = _relativePosition;
        // Redraw the waveform
        repaint();
    }
}

// Called whenever the audio thumbnail broadcasts changes
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    repaint();
}