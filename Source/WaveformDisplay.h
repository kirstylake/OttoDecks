/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Aug 2022 5:01:53pm
    Author:  kirst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
						 public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
					juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    /** paint function returns nothing and takes in the juce::Graphics object reference*/
    void paint (juce::Graphics&) override;

    /** function to handle the resizing of the application window returns nothing*/
    void resized() override;

    /*change listener callback function takes in apointer to ChangeBroadcaster and returns nothing*/
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /*takes an input of a file location and loads it into the audio player to play return nothing*/
    void loadURL(juce::URL audioURL);

    /*set relative position of file takes the position in seconds as input returns nothing*/
    void setPositionRelative(double pos);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
