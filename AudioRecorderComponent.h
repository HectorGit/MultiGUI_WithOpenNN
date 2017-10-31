/*
==============================================================================

AudioRecorderComponent.h
Created: 13 Oct 2017 4:32:27pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorder.h"
#include "RecordingThumbnail.h"

//==============================================================================
/*
*/
class AudioRecorderComponent : public AudioAppComponent,
	private AudioIODeviceCallback,
	private Button::Listener,
	private Timer
{
public:
	AudioRecorderComponent() : audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
		true, // showMidiInputOptions must be true
		true, true, false), //deviceManager.getSharedAudioDeviceManager(),
		recorder(recordingThumbnail.getAudioThumbnail())
	{
		//class_being_recorded = 0;
		message = "Recordings haven't started yet";
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
		//audioDeviceManager.addMidiInputCallback(String(), this); // [6]
		audioDeviceManager.addAudioCallback(this);

		setOpaque(true);

		addAndMakeVisible(explanationLabel);
		explanationLabel.setText("Click RECORD to capture an example.", dontSendNotification);
		explanationLabel.setFont(Font(15.00f, Font::plain));
		explanationLabel.setJustificationType(Justification::topLeft);
		explanationLabel.setEditable(false, false, false);
		explanationLabel.setColour(TextEditor::textColourId, Colours::black);
		explanationLabel.setColour(TextEditor::backgroundColourId, Colour(0x00000000));

		addAndMakeVisible(recordButton);
		recordButton.setButtonText("Record");
		recordButton.addListener(this);
		recordButton.setColour(TextButton::buttonColourId, Colour(0xffff5c5c));
		recordButton.setColour(TextButton::textColourOnId, Colours::black);

		addAndMakeVisible(recordingThumbnail);

		deviceManager.addAudioCallback(&recorder);

		setSize(600, 600); //i kept this.

						   // specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}

	~AudioRecorderComponent()
	{
		//shutdownAudio();
		deviceManager.removeAudioCallback(&recorder);
		//deviceManager.removeAudioCallback(&liveAudioScroller);
	}

	//==============================================================================
	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		// This function will be called when the audio device is started, or when
		// its settings (i.e. sample rate, block size, etc) are changed.

		// You can use this function to initialise any resources you might need,
		// but be careful - it will be called on the audio thread, not the GUI thread.

		// For more details, see the help for AudioProcessor::prepareToPlay()
	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		// Your audio-processing code goes here!

		// For more details, see the help for AudioProcessor::getNextAudioBlock()

		// Right now we are not producing any data, in which case we need to clear the buffer
		// (to prevent the output of random noise)
		bufferToFill.clearActiveBufferRegion();
	}

	void releaseResources() override
	{
		// This will be called when the audio device stops, or when it is being
		// restarted due to a setting change.

		// For more details, see the help for AudioProcessor::releaseResources()
	}

	//==============================================================================
	void paint(Graphics& g) override
	{
		// (Our component is opaque, so we must completely fill the background with a solid colour)
		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
		//g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));

		// You can add your drawing code here!
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
		Rectangle<int> area(getLocalBounds());
		Rectangle<int> takenArea(area.removeFromTop(80));
		recordButton.setBounds(takenArea.removeFromLeft(140).reduced(8));
		recordingThumbnail.setBounds(takenArea.removeFromLeft(300).reduced(8)); //recordingThumbnail not recognized
		explanationLabel.setBounds(takenArea.removeFromLeft(100).reduced(8));
	}

	void audioDeviceIOCallback(const float** /*inputChannelData*/, int /*numInputChannels*/,
		float** outputChannelData, int numOutputChannels,
		int numSamples) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code
	}

	void audioDeviceAboutToStart(AudioIODevice* device) override
	{
		//so the compiler doesn't cry - grab from the recording button sample code.
	}

	void audioDeviceStopped() override
	{
		//so the compiler doesn't cry - originally empty - grab from the recording button sample code.
	}
	// Your private member variables go here...

	//int class_being_recorded;

	/*String returnMessage() {
	return message;
	}*/


	AudioRecorder* getAudioRecorder() {
		return &recorder;
	}

	TextButton* getButton() {
		return &recordButton;
	}

private:

	//==============================================================================
	TextButton recordButton;

	AudioDeviceManager audioDeviceManager;         // [3]
	AudioDeviceSelectorComponent audioSetupComp;   // [4]

	String message;
	//AudioDeviceManager& deviceManager;
	RecordingThumbnail recordingThumbnail;
	AudioRecorder recorder;
	Label explanationLabel;

	void startRecording()
	{
		startTimer(500);
		const File file(File::getSpecialLocation(File::userDocumentsDirectory)
			.getNonexistentChildFile("Juce Demo Audio Recording", ".wav"));
		recorder.startRecording(file);

		recordButton.setButtonText("Stop");
		recordingThumbnail.setDisplayFullThumbnail(false);
		if (getTimerInterval() > 2000) {
			stopRecording();
			stopTimer();
		}
	}

	void stopRecording()
	{
		recorder.stop();
		recordButton.setButtonText("Record");
		recordingThumbnail.setDisplayFullThumbnail(true);
	}

	void buttonClicked(Button* button) override
	{
		if (button == &recordButton)
		{
			if (recorder.isRecording())
				stopRecording();
			else
				/*if (class_being_recorded==1) {
				message = "Class 1 being recorded";
				}else if (class_being_recorded == 2) {
				message = "Class 2 being recorded";
				}else{
				message = "Class 3 being recorded.";
				}*/
				//startTimer()
				startRecording();
		}
	}

	/*void timerCallBack()
	{
	repaint();
	}*/

	void timerCallback() override
	{

		repaint();
	}


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioRecorderComponent)
};

