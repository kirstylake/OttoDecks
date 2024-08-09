#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "TrackHeader.h"
#include "LogoHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::AudioAppComponent,
					  public juce::Button::Listener,
					  public juce::Slider::Listener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================

	/*standard virtual functions for AudioSource*/
    /*prepares the audioblock to be played takes in an integer of samplesPerBlockExpected and a double of sampleRate*/
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;

    /*get the next audio block and takes in a reference to AudioSourceChannelInfo*/
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;

    /*releases resources once the file is finished with the audio block returns nothing*/
    void releaseResources() override;

    //==============================================================================
    /*Takes in a reference to the juce Graphics and creates the visual graphics returns nothing*/
    void paint (juce::Graphics& g) override;

    /** function to handle the resizing of the application window returns nothing*/
    void resized() override;

    /** implement button listener takes in a pointer to Button and returns nothing*/
    void buttonClicked(juce::Button* button) override;

    /** implement slider listener takes in a pointer to slider and returns nothing*/
    void sliderValueChanged(juce::Slider *slider) override;

private:

    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{formatManager};
    DJAudioPlayer player2{formatManager};
    DJAudioPlayer tempPlayer{ formatManager };

    DeckGUI deckGUI1{&player1, formatManager, thumbCache, &waveformDisplay1, &header1};
    DeckGUI deckGUI2{&player2, formatManager, thumbCache, &waveformDisplay2, &header2};

    juce::MixerAudioSource mixerSource;

    WaveformDisplay waveformDisplay1{ formatManager, thumbCache };
    WaveformDisplay waveformDisplay2{ formatManager, thumbCache };

    TrackHeader header1;
    TrackHeader header2;

    LogoHeader logoHeader;

    PlaylistComponent playlistComponent{ &player1, &player2, &tempPlayer, formatManager, &waveformDisplay1, &waveformDisplay2, &header1, &header2 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
