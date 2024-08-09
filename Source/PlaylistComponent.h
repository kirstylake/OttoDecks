/*
  ==============================================================================

    PlaylistComponent.h
    Created: 24 Aug 2022 8:01:17pm
    Author:  kirst

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "WaveformDisplay.h"
#include <fstream>
#include "TrackHeader.h"

//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
				           public juce::TableListBoxModel,
						   public juce::Button::Listener,
						   public juce::FileDragAndDropTarget
{
public:
    PlaylistComponent(DJAudioPlayer* _player1, 
					  DJAudioPlayer* _player2,
                      DJAudioPlayer* _tempPlayer,
                      juce::AudioFormatManager& formatManagerToUse,
                      WaveformDisplay* _waveformDisplay1, 
					  WaveformDisplay* _waveformDisplay2,
                      TrackHeader* _header1,
					  TrackHeader* _header2);
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    /** get the Number of Rows returns a integer of this value*/
    int getNumRows() override;

    /** paint row background takes in a reference to the juce Graphics, a rowNumber, the width, height and a
     *boolean of whether the row is selected returns nothing*/
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    /** paint row background takes in a reference to the juce Graphics, a columnId, the width, height and a
   *boolean of whether the row is selected returns nothing*/
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    /** refresh Component for cell takes in a rowNumber, a columnId, boolean of whether the row is selected
     * and a pointer to a Component returns the Component*/
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    /** implement button listener, takes in a point to the juce Button and returns nothing*/
    void buttonClicked(juce::Button*) override;

    /** file drop toggle to decide if the application will accept files returns a boolean*/
    bool isInterestedInFileDrag(const juce::StringArray& files) override;

    /**file drop listeners with an input of file arrays*/
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    /** Tokenize a string from playlist file, takes in a string line from a textfile and a seperator , returns a vector with 2 values*/
    static std::vector<std::string> tokenise(std::string Line, char separator);

    /** initialize tracks list from "Tracks" folder returns nothing*/
    void getTracks();
private:

    //Playlist Components
    juce::TableListBox tableComponent;
    juce::TextEditor searchBox;

    //Buttons
    juce::TextButton saveButton{ "Save Playlist" };
    juce::TextButton newButton{ "New Playlist" };
    juce::TextButton loadButton{ "Load Playlist" };

    //Strings
    juce::String juceUrl;
    std::string stringUrl;
    std::string filenameString;

    std::ofstream playlistSave;

    //Variables
    int trackIndex;

    //FileChooser
    juce::FileChooser fChooser{ "Select a file..." };

    //Pointers
    DJAudioPlayer* player1;
    DJAudioPlayer* player2;
    DJAudioPlayer* tempPlayer;
    WaveformDisplay* waveformDisplay1;
    WaveformDisplay* waveformDisplay2;
    TrackHeader* header1;
    TrackHeader* header2;

    //Vectors
    std::vector<juce::String> trackTitles;//file's name
    std::vector<juce::URL> trackURLs;//the files' URL
    std::vector<int> trackLengths;


    //Dialog
    SafePointer<juce::DialogWindow> dialogWindow;

    /*Search functionality, takes an input text and selected the rows that match, returns nothing*/
    void search(juce::String inputtext);

    /*Dialog Window to display on start of application, returns nothing*/
    void popup();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
