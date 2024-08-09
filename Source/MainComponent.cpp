#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 800);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(waveformDisplay1);
    addAndMakeVisible(waveformDisplay2);
    addAndMakeVisible(header1);
    addAndMakeVisible(header2);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(logoHeader);
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour(0, 25, 51));
    g.setFont(20.0f);
	
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    logoHeader.setBounds(0, 0, getWidth(), 100);
    header1.setBounds(0, 100, getWidth() / 2, 80);
    header2.setBounds(getWidth() / 2, 100, getWidth() / 2, 80);

    deckGUI1.setBounds(0, 182, getWidth() / 2, getHeight() * 0.67);
    deckGUI2.setBounds(getWidth() / 2, 182, getWidth() / 2, getHeight() * 0.67);

    const float third = getHeight() * 0.33;
    waveformDisplay1.setBounds(0, getHeight() / 2 + 100, getWidth() / 2, 80);
    waveformDisplay2.setBounds(getWidth() / 2, getHeight() / 2 + 100, getWidth() / 2, 80);

    playlistComponent.setBounds(0, getHeight() - third + 40, getWidth(), getHeight() );
}

void MainComponent::buttonClicked(juce::Button* button)
{
    
}

void MainComponent::sliderValueChanged(juce::Slider *slider)
{

}

