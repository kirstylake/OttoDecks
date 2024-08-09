/*
  ==============================================================================

    TrackHeader.h
    Created: 4 Sep 2022 12:57:28pm
    Author:  kirst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/* This class is here to display the title of tracks and store data that needs to be passed from the
 * PlaylistComponent class to the DeckGUI class
*/
class TrackHeader  : public juce::Component
{
public:
    TrackHeader();
    ~TrackHeader() override;

    /** paint function returns nothing and takes in the juce::Graphics object reference*/
    void paint (juce::Graphics&) override;

    /** function to handle the resizing of the application window returns nothing*/
    void resized() override;

    /** get Row index of loaded track and stores in this class returns an integer*/
    int getRowIndex();

    /** sets the title of the track on the top of the screen along with its length, takes in title string,
     *a song length string and a default row index if none is given, returns nothing*/
    void setHeader(juce::String _title, juce::String _length, int _rowIndex = -2);

    /** sets the vector data to be stored here, takes in a vector of URLs and returns nothing*/
    void setTrackData(std::vector<juce::URL> _trackURLs);

    /** returns the URL of the next track takes in nothing*/
    juce::URL getNextTrack();

    /** returns the URL of the previous track takes in nothing*/
    juce::URL getPrevTrack();

private:
    //A DrawableText variable
    juce::DrawableText title;

    //A rectangle and a parallelogram that are needed to draw the bounds of the title text
    juce::Rectangle<float> displayTitle{ 0,0,0,0 };
    juce::Parallelogram<float> titleBox;

    //the rowIndex that is stored here
    int rowIndex = 0;

    //the track data that is stored in this class
    std::vector<juce::URL> trackURLs;
    juce::URL nextTrack;
    juce::URL prevTrack;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackHeader)
};
