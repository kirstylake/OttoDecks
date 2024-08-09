/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Aug 2022 5:01:53pm
    Author:  kirst

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
								 juce::AudioThumbnailCache& cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse),
																		  fileLoaded(false),
																		  position(0)
{
    audioThumb.addChangeListener(this);

}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{
    // clear the background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    // draw an outline around the component
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    //change the waveform colour to a light blue
    g.setColour (juce::Colour(102, 178, 255));
    if(fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(),
            0,
            audioThumb.getTotalLength(),
            0,
            1.0f);

        g.setColour(juce::Colour(102, 178, 255));
        g.drawRect(position * getWidth(), 0, getWidth() / 50, getHeight());
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
					juce::Justification::centred, true);   // draw some placeholder text 
    }

}

void WaveformDisplay::resized()
{
   

}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    DBG("Waveform display load URL");
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));
    if (fileLoaded)
    {
        DBG("wfd: loaded");
        repaint();
    }else
    {
        DBG("wfd: not loaded");
    }
}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    DBG("wfd: change recieved");
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position)
    {
        position = pos;
        repaint();
    }
    position = pos;

}

