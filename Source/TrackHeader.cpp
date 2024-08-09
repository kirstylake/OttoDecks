/*
  ==============================================================================

    TrackHeader.cpp
    Created: 4 Sep 2022 12:57:28pm
    Author:  kirst

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackHeader.h"

//==============================================================================
TrackHeader::TrackHeader()
{

    //make title visible
    addAndMakeVisible(title);

    //set parameters for title
    title.setJustification(juce::Justification::centred);
    title.setFontHeight(20.0f);
    title.setText("Choose a File to Play");
    title.setColour(juce::Colour(0, 51, 102));
}

TrackHeader::~TrackHeader()
{
}

void TrackHeader::paint (juce::Graphics& g)
{
    //formating for title text
    titleBox = displayTitle;
    title.setBoundingBox(titleBox);

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background

    //Filling the background with a dark blue and a light blue inner box
    g.fillAll(juce::Colour(204, 229, 255));
    g.setColour(juce::Colour(0, 51, 102));

    // draw an outline around the component   
    g.drawRect(getLocalBounds(), 3);   
}

void TrackHeader::resized()
{
  
    displayTitle.setWidth(getLocalBounds().toFloat().getWidth());
    displayTitle.setHeight(getLocalBounds().toFloat().getHeight());
}

//This function displays the name of a track once it is loaded along with its song length
void TrackHeader::setHeader(juce::String _title, juce::String _length, int _rowIndex)
{
    title.setText(_title + "\n" + _length);
    if (rowIndex >-1)
    {
        rowIndex = _rowIndex;
    }
    
    
}

//saves the rowIndex of the loaded track to a variable in this class that can be accessed in the DeckGUI class
int TrackHeader::getRowIndex()
{
    return rowIndex;
}

//updates the trackURLs vector in this class so that it can be accessed from the DeckGUI class
void TrackHeader::setTrackData(std::vector<juce::URL> _trackURLs)
{
    trackURLs.clear();
    for (int i = 0; i < _trackURLs.size(); i++) {
        trackURLs.push_back(_trackURLs[i]);
        trackURLs.shrink_to_fit();
    }

}

//calculates the next logical track
juce::URL TrackHeader::getNextTrack()
{
    rowIndex = rowIndex + 1;
    const int last_element = trackURLs.size() - 1; //find the last element of the vector

    if (rowIndex > last_element)
    {
        rowIndex = 0; //if we go past the last element rollback to the first element
    }

    //if the playlist has been emptied then return a nul URL so suitable error handling can happen
    //this stops the program from crashing
    if (trackURLs.empty() == false)
    {
        return trackURLs[rowIndex];
    }
    else
    {
        juce::URL juceURL = "";
        return juceURL;
    }
    
}
juce::URL TrackHeader::getPrevTrack()
{
    rowIndex = rowIndex - 1;

    const int last_element = trackURLs.size() - 1; //find the last element of the vector

    if (rowIndex == -1)
    {
        rowIndex = last_element; //if go back past first element then restart from beginning
    }

    if (trackURLs.empty() == false)
    {
        return trackURLs[rowIndex];
    }
    else
    {
    	juce::URL juceURL = "";
        return juceURL;
    }
}
