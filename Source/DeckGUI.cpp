/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Aug 2022 7:03:48pm
    Author:  kirst

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"


//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, 
				juce::AudioFormatManager& formatManagerToUse,
				juce::AudioThumbnailCache& cacheToUse,
                WaveformDisplay* _waveformDisplay,
                TrackHeader* _header
                ) : player(_player),
                    waveformDisplay(_waveformDisplay),
                    header(_header)
										
{
    //Make components visible
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(loopButton);
    addAndMakeVisible(loopLabel);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(prevButton);

    //Adjust Volume Slider
    volSlider.setRange(0, 1.0);
    volSlider.setValue(0.5);
    volSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    volLabel.setText("Volume", juce::dontSendNotification);
    volSlider.setNumDecimalPlacesToDisplay(3);

    //Adjust Position Slider
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);
    posSlider.setRange(0, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(3);

    //Adjust Speed Slider
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setRange(0, 20);
    speedSlider.setValue(1);
    speedSlider.setNumDecimalPlacesToDisplay(3);

    //Adjust Loop Toggle
    loopLabel.setText("Loop", juce::dontSendNotification);
    loopLabel.attachToComponent(&loopButton, true);
    loopButton.addListener(this);

    //Add Listeners
    playButton.addListener(this);
    stopButton.addListener(this);
    nextButton.addListener(this);
    prevButton.addListener(this);
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    startTimer(200);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    //grey background
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    // border color
    g.setColour (juce::Colours::grey);

    // draw an outline around the component
    g.drawRect (getLocalBounds(), 1);   

    //text color
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    //Change slider knob color
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour(0, 102, 204));

    //Button hover color change
    if (playButton.isOver())
    {
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0,102,204));
    }
    else
    {
        playButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }

    if (prevButton.isOver())
    {
        prevButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        prevButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }
    if (nextButton.isOver())
    {
        nextButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        nextButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }

    if (stopButton.isOver())
    {
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        stopButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }
}

void DeckGUI::resized()
{
    //variables to make resizing easier
    const double rowH = getHeight() / 8;
    const double colW = getWidth() / 5;

    prevButton.setBounds(0, 0, colW, rowH);
    playButton.setBounds(colW, 0, colW, rowH);
    stopButton.setBounds(colW * 2, 0, colW, rowH);
    nextButton.setBounds(colW * 3, 0, colW, rowH);
    loopButton.setBounds(colW * 4 + 55, 0, colW / 2, rowH);

    volLabel.setBounds(100, rowH, getWidth() / 2 + 100, rowH);
    volSlider.setBounds(0, rowH * 2, getWidth() /2, rowH * 2);

    speedLabel.setBounds(getWidth() / 2 + 100, rowH , getWidth() / 2 + 100, rowH);
    speedSlider.setBounds(getWidth() / 2, rowH * 2, getWidth() / 2, rowH * 2);

    posLabel.setBounds(0, rowH * 4, getWidth(), rowH);
    posSlider.setBounds(100, rowH * 4, getWidth() - 100, rowH);

}

void DeckGUI::buttonClicked(juce::Button* button)
{
    //events for buttons
    if(button == &playButton)
    {
        player->play();
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &nextButton)
    {
        //track vector stored in header
        const juce::URL nextTrack = header->getNextTrack();

        if (nextTrack.isEmpty() == false)
        {
            //load track
            player->loadURL(juce::URL{ nextTrack });
            waveformDisplay->loadURL(juce::URL{ nextTrack });
            //change title text
            header->setHeader(nextTrack.getFileName().toStdString(), player->getSongLength(), header->getRowIndex());
        }
    }

    if (button == &prevButton)
    {
        const juce::URL prevTrack = header->getPrevTrack();
        if ( prevTrack.isEmpty() == false)
        {
            player->loadURL(juce::URL{ prevTrack });
            waveformDisplay->loadURL(juce::URL{ prevTrack });
            header->setHeader(prevTrack.getFileName().toStdString(), player->getSongLength(), header->getRowIndex());
        }

    }

    if (button == &loopButton)
    {

        if (loopButton.getToggleState() == true)
        {
            player->setLoop();
        }
        else
        {
            player->unsetLoop();
        }
    }

}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
 
    return true;
}
void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{

    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
        waveformDisplay->loadURL(juce::URL{ juce::File{files[0]} });
        header->setHeader(juce::File{ files[0] } .getFileName().toStdString(), player->getSongLength(), header->getRowIndex());
    }
}

void DeckGUI::timerCallback()
{
       waveformDisplay->setPositionRelative(
            player->getPositionRelative());
    if (player->getPositionRelative() >= 0.0 && player->getPositionRelative() <= 1.0) {
        posSlider.setValue(player->getPositionRelative());
    }
}


