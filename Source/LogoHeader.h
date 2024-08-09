/*
  ==============================================================================

    LogoHeader.h
    Created: 5 Sep 2022 12:59:31am
    Author:  kirst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/* This is purely to display the logo header
*/
class LogoHeader  : public juce::Component
{
public:
    LogoHeader();
    ~LogoHeader() override;

    /*Takes in a reference to the juce Graphics and creates the visual graphics returns nothing*/
    void paint (juce::Graphics&) override;

    /** function to handle the resizing of the application window returns nothing*/
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogoHeader)
	juce::Image logo;
};
