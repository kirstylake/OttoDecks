/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 2 Aug 2022 8:16:15pm
    Author:  kirst

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager)
{

}
DJAudioPlayer::~DJAudioPlayer()
{

}
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}


void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource
        (new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
    }
}
void DJAudioPlayer::play()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop();
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    if (posInSecs < 0 || posInSecs > transportSource.getLengthInSeconds()) {
        std::cout << "DJAudioPlayer::setPosition: warning set position " << posInSecs \
            << " greater than length " << transportSource.getLengthInSeconds() << std::endl;
        return;
    }
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1)
    {
        DBG("DJAudioPlayer::setPositionRelative pos should be between 0 and 1");

    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
        
    }

}


void DJAudioPlayer::setGain(double gain)
{
    if(gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");

    }else
    {
        transportSource.setGain(gain);
    }
    
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100)
    {
        DBG("DJAudioPlayer::setSpeed ratio should be between 0 and 100");

    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

juce::String DJAudioPlayer::getSongLength()
{
    double minutesLength{ transportSource.getLengthInSeconds() / 60 };

    //return value of getLengthInSeconds is in seconds
    //take the decimal and the integer parts
    double decimal, integer;
    decimal = modf(minutesLength, &integer);

    //convert to mins and secs
    std::string min{ std::to_string((int)integer) };
    std::string sec{ std::to_string((int)round(decimal * 60)) };

    juce::String songLength{ min + " min : " + sec + " sec" };

    return songLength;
}

//Pure virtual functions implemented below because inheritence from PositionableAudioSource class
void DJAudioPlayer::setNextReadPosition(juce::int64 newPosition)
{

}

juce::int64 DJAudioPlayer::getNextReadPosition() const
{
    return readerSource->getNextReadPosition();
}

juce::int64 DJAudioPlayer::getTotalLength() const
{
    return readerSource->getTotalLength();
}

//Returning true if it is looping, or false if it is not
bool DJAudioPlayer::isLooping() const
{
    return false;
}

//Setting the playback to loop
void DJAudioPlayer::setLoop()
{
    //if the readerSource is not a null pointer, the readerSource is set to loop
    //using the function from the PositionableAudioSource class

    if (readerSource != nullptr)
    {
        readerSource->setLooping(true);
    }
}

//Setting the playback not to loop
void DJAudioPlayer::unsetLoop()
{
    //if the readerSource is not a null pointer, the readerSource is set to not loop
    //using the function from the PositionableAudioSource class
    if (readerSource != nullptr)
    {
        readerSource->setLooping(false);
    }
}
