/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Aug 2022 7:03:48pm
    Author:  kirst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TrackHeader.h"


//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
				 public juce::Button::Listener,
				 public juce::Slider::Listener,
				 public juce::FileDragAndDropTarget,
				 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
            juce::AudioFormatManager & formatManagerToUse,
            juce::AudioThumbnailCache & cacheToUse,
			WaveformDisplay* _waveformDisplay,
            TrackHeader* _header);

    ~DeckGUI() override;

    /** paint function returns nothing and takes in the juce::Graphics object reference*/
    void paint (juce::Graphics&) override;

    /** function to handle the resizing of the application window returns nothing*/
    void resized() override;

    /** implement button listener with reference the Button object as an input, returns nothing*/
    void buttonClicked(juce::Button* button) override;

    /** file drop toggle to decide if the application will accept files returns a boolean*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /**file drop listeners with an input of file arrays*/
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** implement slider listener*/
    void sliderValueChanged(juce::Slider* slider) override;

    /** timer call back which returns nothing*/
    void timerCallback() override;

private:

    //Buttons
    juce::TextButton playButton{ "PLAY" };
    juce::ToggleButton loopButton;
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton nextButton{ "Next >>" };
    juce::TextButton prevButton{ " << Previous" };

    //Sliders
    juce::Slider volSlider;
    juce::Slider gainSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;

    //Labels
    juce::Label volLabel;
    juce::Label loopLabel;
    juce::Label speedLabel;
    juce::Label posLabel;

    //FileChoose
    juce::FileChooser fChooser{ "Select a file..." };

    //Pointers

    TrackHeader* header;
    DJAudioPlayer* player;
    WaveformDisplay* waveformDisplay;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
