/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 2 Aug 2022 8:16:15pm
    Author:  kirst

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource,
	                  public juce::PositionableAudioSource
{
public:
	// constructor and destructor
	DJAudioPlayer(juce::AudioFormatManager& _formatManager);
	~DJAudioPlayer() override;

	/*standard virtual functions for AudioSource*/
	/*prepares the audioblock to be played takes in an integer of samplesPerBlockExpected and a double of sampleRate*/
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

	/*get the next audio block and takes in a reference to AudioSourceChannelInfo*/
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

	/*releases resources once the file is finished with the audio block returns nothing*/
	void releaseResources() override;

	/*takes an input of a file location and loads it into the audio player to play return nothing*/
	void loadURL(juce::URL file);

	/*start playing the file input of nothing returns nothing*/
	void play();

	/*stop playing the file input of nothing returns nothing*/
	void stop();

	/*get the relative position of the playhead, returns a double of this position*/
	double getPositionRelative();

	/*set relative position of file takes the position in seconds as input returns nothing*/
	void setPosition(double posInSecs);

	/*set relative position takes the input of position*/
	void setPositionRelative(double pos);

	/*set gain/volume of file takes an input of gain*/
	void setGain(double gain);

	/*set speed of file takes an input of the ratio*/
	void setSpeed(double ratio);

	/*get song length, returns a string with song length*/
	juce::String getSongLength();

	//pure virtual functions from PositionableAudioSource class to implement loop feature
	/*sets the next read position of the reader and takes in the newPosition aa an int64*/
	void setNextReadPosition(juce::int64 newPosition) override;

	/*gets the next read position and returns an int64 of the next read position*/
	juce::int64 getNextReadPosition() const override;

	/*gets the total length of the traack and returns an int64 of the track length*/
	juce::int64 getTotalLength() const override;

	/*returns a boolean as to whether the track is set to be looping or not*/
	bool isLooping() const override;

	/*sets the track to loop, returns nothing*/
	void setLoop();

	/*unsets the track to loop, returns nothing*/
	void unsetLoop();

private:
	juce::AudioFormatManager& formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};


