#include "MainComponent.h"
#include <cmath>

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(pitchSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(loadButton);


    // add our own self, a Button::Listener object, to the play button
    // we are a listener object because we inherit from Button::Listener class
    // even though we are a component too
    // we have our own buttonClicked function
    // "this" refers to MainComponent
    playButton.addListener(this);
    stopButton.addListener(this);
    gainSlider.addListener(this);
    pitchSlider.addListener(this);
    speedSlider.addListener(this);
    loadButton.addListener(this);

    // restrict the gain slider range
    gainSlider.setRange(0.0, 1.0);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // set to not be playing on start
    //playing = false;

    // initialise sample settings
    phase = 0;
    dphase = 0;
    gain = 0.5;

    // register audio formats with the format manager
    formatManager.registerBasicFormats();

    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // clear buffer and return if not playing
    if (!playing)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    // else get next audio block from the transport source
    //transportSource.getNextAudioBlock(bufferToFill);
    resampleSource.getNextAudioBlock(bufferToFill);
}
//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//    // clear buffer and return if not playing
//    if (!playing)
//    {
//        bufferToFill.clearActiveBufferRegion();
//        return;
//    }
//
//    // get pointers to the buffer channel locations to write to
//    auto* leftChannel = bufferToFill.buffer->getWritePointer(0, 
//        bufferToFill.startSample);
//    auto* rightChannel = bufferToFill.buffer->getWritePointer(1,
//        bufferToFill.startSample);
//
//    // fill the buffer channels with generated samples
//    for (auto i = 0; i < bufferToFill.numSamples; ++i)
//    {
//        // set the sample value
//        // auto sample = rand.nextFloat() * 0.125f * gain;     // white noise
//         auto sample = fmod(phase, 1.0f) * 0.125f * gain;      // sawtooth wave        
//        // auto sample = sin(phase) * 0.125f * gain;           // sine wave
//
//        // assign value to both channels
//        leftChannel[i] = sample;
//        rightChannel[i] = sample;
//
//        // increment the phase according to the frequency
//        // phase += dphase;              // for a constant frequency/pitch, add a constant dphase
//
//        // increment the phase according to the frequency
//        phase += fmod(dphase, 0.01f);   // fmod the dphase to reset the pitch to a lower one
//        dphase += 0.0000005f;           // make the frequency/pitch ramp up
//    }    
//}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()

    transportSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    //DBG("Paint is called!");
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.

    DBG("MainComponent::resized");

    double rowHeight = getHeight() / 6;

    playButton.setBounds(0, 0, getWidth(), rowHeight);
    stopButton.setBounds(0, rowHeight, getWidth(), rowHeight);
    gainSlider.setBounds(0, rowHeight * 2, getWidth(), rowHeight);
    pitchSlider.setBounds(0, rowHeight * 3, getWidth(), rowHeight);
    speedSlider.setBounds(0, rowHeight * 4, getWidth(), rowHeight);
    loadButton.setBounds(0, rowHeight * 5, getWidth(), rowHeight);
}

/** Implement Button::Listener */
void MainComponent::buttonClicked(juce::Button* button)
{
    // process responses for each button
    if (button == &playButton)
    {
        DBG("MainComponent::buttonClicked: Play Button was clicked");
        // begin playing
        //playing = true;
        transportSource.start();
        // reset dphase
        dphase = 0;
    }
    if (button == &stopButton)
    {
        DBG("MainComponent::buttonClicked: Stop Button was clicked");
        // stop playing
        //playing = false;
        transportSource.stop();
    }
    if (button == &loadButton)
    {
        DBG("MainComponent::buttonClicked: Load Button was clicked");

        // create a file chooser GUI for the user to select a file
        juce::FileChooser chooser{ "Select file..." };
        // if the user selects a file to open, load the file
        if (chooser.browseForFileToOpen())
        {
            // convert the chosen file to a URL and load it
            loadURL(juce::URL{ chooser.getResult() });
        }
    }
}

/** Implement Slider::Listener */
void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &gainSlider)
    {
        DBG("MainComponent:sliderValueChanged: gainSlider " + std::to_string(slider->getValue()));
        //gain = gainSlider.getValue();
        transportSource.setGain(slider->getValue());
    }
    // implement pitch slider effects
    if (slider == &pitchSlider)
    {
        DBG("MainComponent::sliderValueChanged: pitchSlider moved " + std::to_string(slider->getValue()));
        // phase change for sawtooth waveform
        dphase = pitchSlider.getValue() * 0.001;
         
        // phase change for sine waveform
        //dphase = volumeSlider.getValue() * 0.01;
    }

    // implement gain slider effects
    if (slider == &speedSlider)
    {
        DBG("MainComponent:sliderValueChanged: gainSlider " + std::to_string(slider->getValue()));
        //gain = gainSlider.getValue();
        resampleSource.setResamplingRatio(slider->getValue());
    }
}

void MainComponent::loadURL(juce::URL audioURL)
{
    // get path to audio file
    //juce::URL audioURL{ "file:///C:/Users/alana/Documents/SchoolCode/OOP-JUCE/Tracks/tracks/aon_inspired.mp3" };

    // create the AudioFormatReader object from the audio file, converting the audio file to an input stream first
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    // check that the file converted correctly
    if (reader != nullptr) // good file!
    {
        // assign a new AudioFormatReaderSource object to a new smart pointer
        // pass it the AudioFormatReader object made from the audio file
        // set to true to delete the AudioFormatReader object when the AudioFormatReaderSource object is deleted
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
        (new juce::AudioFormatReaderSource(reader, true));

        // create the TransportSource object based on this AudioFormatReaderSource object
        transportSource.setSource(newSource.get(), 0, nullptr,
            reader->sampleRate);
        // pass the AudioFormatReaderSource object to the class readerSource pointer
        readerSource.reset(newSource.release());
        // start the transportSource
        //transportSource.start();
    }
    else
    {
        DBG("Something went wrong loading the file.");
    }
}