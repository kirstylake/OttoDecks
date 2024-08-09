/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 24 Aug 2022 8:01:17pm
    Author:  kirst

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player1, DJAudioPlayer* _player2, DJAudioPlayer* _tempPlayer,
                                     juce::AudioFormatManager& formatManagerToUse, 
    WaveformDisplay* _waveformDisplay1,
    WaveformDisplay* _waveformDisplay2,
    TrackHeader* _header1,
    TrackHeader* _header2 ): player1(_player1),
							 player2(_player2),
							 tempPlayer(_tempPlayer),
							 waveformDisplay1(_waveformDisplay1),
							 waveformDisplay2(_waveformDisplay2),
							 header1(_header1),
							 header2(_header2)
{
    tableComponent.getHeader().addColumn("Tracks", 1, 400);
    tableComponent.getHeader().addColumn("Track Length", 2, 200);
    tableComponent.getHeader().addColumn("Change File", 3, 100);
    tableComponent.getHeader().addColumn("Left Deck", 4, 100);
    tableComponent.getHeader().addColumn("Right Deck", 5, 100);
    tableComponent.getHeader().addColumn("Delete", 6, 100);
    tableComponent.setModel(this);

    //Make components visible
    addAndMakeVisible(tableComponent);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(newButton);
    addAndMakeVisible(loadButton);

    //add listeners
    saveButton.addListener(this);
    newButton.addListener(this);
    loadButton.addListener(this);

    //create search box
    searchBox.setTextToShowWhenEmpty("Type track name and press enter to search", juce::Colour(0, 51, 102));
    //trigger search event when enter pressed
    searchBox.onReturnKey = [this] {search(searchBox.getText()); };
    tableComponent.setMultipleSelectionEnabled(true);
    popup();
    //change colour scheme of search boc
    getLookAndFeel().setColour(juce::TextEditor::textColourId, juce::Colour(0, 51, 102));
    getLookAndFeel().setColour(juce::TextEditor::backgroundColourId, juce::Colour(102, 178, 255));

    tableComponent.setColour(juce::ListBox::backgroundColourId, juce::Colour(218, 229, 245));

    //loads the tracks stored in the local "tracks" folder on start up
    getTracks();
    
}

PlaylistComponent::~PlaylistComponent()
{

}

void PlaylistComponent::paint (juce::Graphics& g)
{
	//background is grey
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    //outline around playlist
    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    //font details
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    //button colors for hover over events
    if (loadButton.isOver())
    {
        loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        loadButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }

    if (newButton.isOver())
    {
        newButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        newButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }

    if (saveButton.isOver())
    {
        saveButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
    }
    else
    {
        saveButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
    }
}

void PlaylistComponent::resized()
{
    const double colW = getWidth() / 6;
    const double rowH = getHeight() / 14;
    saveButton.setBounds(0, 0, colW, rowH);
    loadButton.setBounds(colW, 0 , colW, rowH);
    newButton.setBounds(colW * 2, 0, colW, rowH);
    searchBox.setBounds(colW * 3,0 , colW * 3, rowH);

    tableComponent.setBounds(0, rowH, getWidth(), 8 * getHeight() / 9);

}

int PlaylistComponent::getNumRows()
{
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(juce::Colour(102, 178, 255));
    }
    else {
        g.fillAll(juce::Colour(153, 204, 255));
    }
}
void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    // fix for bug where it prints one too many rows and gives vector short dump
    if (rowNumber >= getNumRows())
    {
        return;
    }else{

        //uses the temporary player to load the URLs of the tracks
        tempPlayer->loadURL(juce::URL(trackURLs[rowNumber]));
        if (columnId == 1)
        {
            g.drawText(trackTitles[rowNumber], 2,
                0, width - 4, height,
                juce::Justification::centredLeft, true);
        }

        if (columnId == 2)
        {
            //access the getSongLength function from the DJAudioPlayer class
            g.drawText(tempPlayer->getSongLength(), 2,
                0, width - 4, height,
                juce::Justification::centredLeft, true);
        }
    }

}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    //Dynamically creating buttons for each row
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {
	        juce::TextButton* btn = new juce::TextButton{ "Change File" };
            juce::String rowid{ std::to_string(rowNumber) };
            juce::String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            if (btn->isOver())
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
            }
            else
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
            }
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
	        juce::TextButton* btn = new juce::TextButton{ "Left Deck" };
            juce::String rowid{ std::to_string(rowNumber) };
            juce::String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            if (btn->isOver())
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
            }
            else
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
            }
            existingComponentToUpdate = btn;
            //make sure the track data is always up to date

        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
	        juce::TextButton* btn = new juce::TextButton{ "Right Deck" };
            juce::String rowid{ std::to_string(rowNumber) };
            juce::String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            if (btn->isOver())
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
            }
            else
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
            }
            existingComponentToUpdate = btn;

        }
    }
    if (columnId == 6) {
        if (existingComponentToUpdate == nullptr) {
	        juce::TextButton* btn = new juce::TextButton{ "Delete" };
            juce::String rowid{ std::to_string(rowNumber) };
            juce::String colid{ std::to_string(columnId) };
            btn->setComponentID(colid + rowid);
            btn->addListener(this);
            if (btn->isOver())
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 102, 204));
            }
            else
            {
                btn->setColour(juce::TextButton::buttonColourId, juce::Colour(0, 51, 102));
            }
            existingComponentToUpdate = btn;
        }
    }
    //make sure the track data is always up to date
    header1->setTrackData(trackURLs);
    header2->setTrackData(trackURLs);

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    //save to playlist
    if (button == &saveButton) {
        //get time and date details to create a .txt file
        auto now = std::time(0);
        std::string dt = ctime(&now);
        tm* ltm = localtime(&now);
        std::string yr = std::to_string(1900 + ltm->tm_year);
        std::string mon = std::to_string(1 + ltm->tm_mon);
        std::string mday = std::to_string(ltm->tm_mday);
        std::string hs = std::to_string(ltm->tm_hour);
        std::string mins = std::to_string(ltm->tm_min);
        std::string secs = std::to_string(ltm->tm_sec);

        //create the text file and get it ready to edit
        playlistSave.open("playlist_" + yr + "_" + mon + "_" + mday + "_" + hs + "_" + mins + "_" + secs + ".txt", std::ios::out | std::ios::app);
        //populate the textfile with our URl and track title separated by a comma
        for (int i = 0; i < trackURLs.size(); i++) {
            juceUrl = trackURLs[i].toString(false);
            stringUrl = juceUrl.toStdString();
            //change URl to a std::string
            filenameString = trackTitles[i].toStdString();
            //change filename to std::string
            playlistSave << stringUrl << "," << filenameString << std::endl;
        }
    }

    if (button == &newButton) {
        //clear the vectors
        trackURLs.clear();
        trackURLs.shrink_to_fit();
        trackTitles.clear();
        tableComponent.updateContent();
        //update the header data
    	header1->setTrackData(trackURLs);
		header2->setTrackData(trackURLs);
    }

    //load a playlist that has already been created
    if (button == &loadButton) {
        trackURLs.clear();
        trackTitles.clear();
        tableComponent.updateContent();
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                const auto chosenFile = chooser.getResult();
				//check if the file exists
                if (chosenFile.exists())
                {
                    //get a long string of what is inside of the file
                    std::ifstream playlistLoad{ chooser.getResult().getFileName().toStdString() };
                    //declare a line to hold our value per line
                    std::string line;
                    //have a small vector to seperate the data
                    std::vector<std::string> fileLineData;
                    if (playlistLoad.is_open()) {
                        while (std::getline(playlistLoad, line)) {
                            try {
                                //tokenising our data
                                fileLineData = tokenise(line, ',');
                                //push to relevant arrays
                                trackURLs.push_back(juce::URL{ fileLineData[0] });
                                trackTitles.push_back(fileLineData[1]);
                            }
                            catch (const std::exception& e) {
                                DBG("CSVReader::readCSV bad data");
                            }
                        }
                    }
                    //keep our data up to date
                    tableComponent.updateContent();
                    tableComponent.resized();
                    header1->setTrackData(trackURLs);
                    header2->setTrackData(trackURLs);
                }
            });
    }

    if (button != &saveButton && button != &newButton && button != &loadButton) {
        //get the ID of the button clicked
        std::string idStr = button->getComponentID().toStdString();
        int id = std::stoi(idStr);
    	trackIndex = std::stoi(idStr.substr(1));

        //change file
        if (idStr.at(0) == '3') {
            //file selector pops up
          
            auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;
            std::string idStr = button->getComponentID().toStdString();
            fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
                {
                   const auto chosenFile = chooser.getResult();
                   
                    if (chosenFile.exists()) {
                        trackTitles[trackIndex] = chooser.getResult().getFileNameWithoutExtension();
                        trackURLs[trackIndex] = chooser.getURLResult();
                        tableComponent.updateContent();
                        tableComponent.repaintRow(trackIndex);
                        header1->setTrackData(trackURLs);
                        header2->setTrackData(trackURLs);
                    }
                });
        }

        //Load song on left deck
        if (idStr.at(0) == '4') {
            player1->loadURL(juce::URL{ trackURLs[trackIndex] });
            waveformDisplay1->loadURL(juce::URL{ trackURLs[trackIndex] });
            header1->setHeader(trackTitles[trackIndex], player1->getSongLength(),trackIndex);
            header1->setTrackData(trackURLs);
        }

        //load song on right deck
        if (idStr.at(0) == '5') {
            player2->loadURL(juce::URL{ trackURLs[trackIndex] });
            waveformDisplay2->loadURL(juce::URL{ trackURLs[trackIndex] });
            header2->setHeader(trackTitles[trackIndex], player2->getSongLength(),trackIndex);
            header2->setTrackData(trackURLs);
        }

        //delete song from playlist
        if (idStr.at(0) == '6') {
            if (trackIndex <= trackURLs.size() - 1 && trackIndex <= trackTitles.size() - 1) {
                trackURLs.erase(trackURLs.begin() + trackIndex);
                trackTitles.erase(trackTitles.begin() + trackIndex);
                tableComponent.updateContent();
                //rebuff the track data
                header1->setTrackData(trackURLs);
                header2->setTrackData(trackURLs);
            }
        }
            
    }

}

bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    return true;
}

void PlaylistComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    //load up multiple files dropped in the playlist component
    if (files.size() >= 1)
    {
        for (int i = 0; i < files.size(); i++) {
            trackTitles.push_back(juce::File{ files[i] } .getFileNameWithoutExtension());
            trackURLs.push_back(juce::URL{ juce::File{files[i]} });
        }
        trackTitles.shrink_to_fit();
       trackURLs.shrink_to_fit();
       tableComponent.resized();
        tableComponent.updateContent();
        

        //make sure the track data is always up to date
        header1->setTrackData(trackURLs);
        header2->setTrackData(trackURLs);

    }
}

void PlaylistComponent::search(juce::String inputtext) {
    //search for a song
    if (inputtext == "") {
        tableComponent.deselectAllRows();
    }
    else {
        //iterate through the trackTitles
        for (int i = 0; i < trackTitles.size(); i++) {
            if (trackTitles[i].contains(inputtext)) {
                tableComponent.selectRow(i, false, false);
            }
        }
    }
}

//create a pop up on start up
void PlaylistComponent::popup() {
    //message for popup
    juce::String m;
    m << "You can drag a file to the playlist table at the bottom to add a track" << juce::newLine;
    m << juce::newLine;
    m << "Tracks saved in the tracks folder will always show on start up" << juce::newLine;
	m << juce::newLine;
	m << "Let's have a swimming time together and make some Otter-ly good music!" << juce::newLine;
    m << juce::newLine;
    m << "Close this to continue" << juce::newLine;
    juce::DialogWindow::LaunchOptions dialog;
    auto* label = new juce::Label();
    label->setText(m, juce::dontSendNotification);
    label->setColour(juce::Label::textColourId, juce::Colours::whitesmoke);
    dialog.content.setOwned(label);

    //space for the text
    juce::Rectangle<int> space(0, 0, 280, 250);
    dialog.content->setSize(space.getWidth(), space.getHeight());

    //popup title
    dialog.dialogTitle = "Welcome to Otter Decks";
    dialog.dialogBackgroundColour = juce::Colour(0, 76, 153);
    dialog.escapeKeyTriggersCloseButton = true;
    dialog.useNativeTitleBar = true;
    dialog.resizable = true;

    //dialog window size
    dialogWindow = dialog.launchAsync();
    if (dialogWindow != nullptr) {
        dialogWindow->centreWithSize(270, 200);
    }
}

std::vector<std::string> PlaylistComponent::tokenise(std::string csvLine, char separator)
{
    //tokenize the data from the file
    std::vector<std::string> tokens;
    signed int first = csvLine.find_first_not_of(separator, 0), end;
    std::string token;
    do {
        end = csvLine.find_first_of(separator, first);
        if (first == csvLine.length() || first == end) break;
        if (end >= 0) token = csvLine.substr(first, end - first);
        else token = csvLine.substr(first, csvLine.length() - first);
        tokens.push_back(token);
        first = end + 1;
    } while (end > 0);
    return tokens;
}

void PlaylistComponent::getTracks()
{
    //load up tracks on start up from the tracks folder
    const juce::String folderPath(juce::File::getCurrentWorkingDirectory().getFullPathName() + "/Tracks");
    const juce::File trackFolder(folderPath);
    //check that folder exists
    if (trackFolder.isDirectory()) 
    {
        juce::DirectoryIterator iter(juce::File(folderPath), true);

        while (iter.next())
        {
            const juce::File fileFound(iter.getFile());
            juce::String title = fileFound.getFileName();
            trackTitles.push_back(fileFound.getFileNameWithoutExtension());
            trackURLs.push_back(juce::URL{ fileFound });
        }

        tableComponent.updateContent();
        tableComponent.resized();
    }
}

