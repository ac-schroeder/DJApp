/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 5 Aug 2021 1:33:56pm
    Author:  alana

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
                                 juce::AudioThumbnailCache& cacheToUse)
    : audioThumb{               // audioThumbnail object
        1000,                       // sourceSamplesPerThumbnailSample: image resolution
        formatManagerToUse,         // AudioFormatManager object: to read file to the audioThumbnail
        cacheToUse},                // AudioThumbnailCache object: to store/share data for all thumbs
      fileLoaded{ false },      // file is not loaded yet
      position{0}               // initial playhead position is 0
{
    // register change listener
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);

    if (fileLoaded)
    {
        // draw waveform
        audioThumb.drawChannel(g, getLocalBounds(), 
                               0, audioThumb.getTotalLength(),
                               0, 1.0f);
        // draw playhead indicator
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 6, getHeight());
    }
    else
    {
        // draw some placeholder text
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
            juce::Justification::centred, true);   
    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

/** Implement ChangeListener */
void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    DBG("Change received");
    repaint();
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    // clear previous drawings
    audioThumb.clear();
    // set the audioURL as the input source for the audio thumbnail
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("WaveformDisplay::loadURL: File is loaded.");
    }
    else
    {
        DBG("WaveformDisplay::loadURL: File was not loaded.");
    }
}

void WaveformDisplay::setPositionRelative(double _position)
{
    // only change position if it's changed
    if (_position != position)
    {
        position = _position;
        repaint();
    }
    
}