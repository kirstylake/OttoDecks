/*
  ==============================================================================

    LogoHeader.cpp
    Created: 5 Sep 2022 12:59:31am
    Author:  kirst

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LogoHeader.h"

//==============================================================================
LogoHeader::LogoHeader()
{

}

LogoHeader::~LogoHeader()
{
}

void LogoHeader::paint (juce::Graphics& g)
{
    //background dark blue
    g.fillAll (juce::Colour(204, 229, 255));   

    //outline dark blue
    g.setColour (juce::Colour(0, 51, 102));
    g.drawRect (getLocalBounds(), 3);   

    //colour of box with logo light blue
    g.setColour (juce::Colour(294,229,255));
    g.setFont (14.0f);
 
    //get Logo from local file

    //image was sourced from
    ////https://www.spreadshirt.ie/shop/design/otter+with+headphones+gift+funny+birthday+sticker-D5d4187b1b264a1173d740552?sellable=R43wBOoEzZSeAr5ywvMb-1459-215
    //text was made in Microsoft Word and then cut to create logo
    logo = juce::ImageFileFormat::loadFrom(juce::File::getCurrentWorkingDirectory().getChildFile("OtterDecks.JPG"));

    //rescale logo to fit in header
    logo = logo.rescaled(360, 94, juce::Graphics::mediumResamplingQuality);

    //draw logo
    g.drawImageAt(logo, 300, 3, false);
    
}

void LogoHeader::resized()
{

}
