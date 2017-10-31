/*
==============================================================================

This file was auto-generated!

==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioRecorderComponent.h"
#include "LogWindowComponent.h"
#include "AudioPlayBackComponent.h"
#include "NeuralNetworkComponent.h"
#include "FeatureExtractionComponent.h"
#include "FreePlayComponent.h"
#include "FlashingComponentMain.h"
#include <iostream>
//#include "marsyas/Conversions.h"

//==============================================================================
/*
This component lives inside our window, and this is where you should put all
your controls and content.
*/
class MainContentComponent : public AudioAppComponent,
	private Button::Listener,
	private Timer,
	private ComboBox::Listener
{
public:
	//==============================================================================
	MainContentComponent()
	{
		//Marsyas::mrs_real freq = (Marsyas::mrs_real)1200.0;
		//Marsyas::mrs_real result = Marsyas::hertz2octs(freq);

		class_being_recorded = 0;
		featuresExtracted = false;
		networkTrained = false;
		recordingClass1 = true; //initally
		recordingClass2 = false;
		recordingClass3 = false;
		counterClass1 = 0;
		counterClass2 = 0;
		counterClass3 = 0;

		trainNeuralNetworkButtonComponent.getButton()->addListener(this);//.getButton().addListener(this);
		featureExtractionButtonComponent.getButton()->addListener(this);//.getButton().addListener(this);
		activateFreePlayButtonComponent.getButton()->addListener(this);

		recorder.getButton()->addListener(this);
		addAndMakeVisible(recorder);
		addAndMakeVisible(playBack1);
		addAndMakeVisible(playBack2);
		addAndMakeVisible(playBack3);
		addAndMakeVisible(logWindow);
		addAndMakeVisible(logWindow2);
		addAndMakeVisible(logWindow3);
		addAndMakeVisible(classChosenComboBox);
		classChosenComboBox.addListener(this);
		classChosenComboBox.addItem("See Messages for Class 1", 1);
		classChosenComboBox.addItem("See Messages for Class 2", 2);
		classChosenComboBox.addItem("See Messages for Class 3", 3);
		classChosenComboBox.setSelectedItemIndex(0);
		class1recordings.push_back("Class 1 Recordings: \n");
		class2recordings.push_back("Class 2 Recordings: \n");
		class3recordings.push_back("Class 3 Recordings: \n");
		//logWindow.logMessage("result is"+(String)result);
		//logWindow.logMessage("result is" + (String)result);
		//logWindow2.logMessage("result is" + (String)result);
		//logWindow3.logMessage("result is" + (String)result);

		//dummy buttons
		addAndMakeVisible(featureExtractionButtonComponent);
		addAndMakeVisible(trainNeuralNetworkButtonComponent);
		addAndMakeVisible(activateFreePlayButtonComponent);
		addAndMakeVisible(flasher);
		addAndMakeVisible(flasher2);

		/*------------------------------------------------------------
		Initially, only the recorder and the first logbox are enabled.*/

		//sector 1 things active initially
		recorder.setEnabled(true);
		logWindow.setEnabled(true); // i dont think this will ever become disabled

									//sector 2 things all inactive initally
		featureExtractionButtonComponent.setEnabled(false);
		logWindow2.setEnabled(false); // i dont think this will ever become disabled
		trainNeuralNetworkButtonComponent.setEnabled(false);
		logWindow3.setEnabled(false); // i dont think this will ever become disabled
		activateFreePlayButtonComponent.setEnabled(false); // i dont think this will ever become disabled

														   //sector 3 components inactive as well
		playBack1.setEnabled(false);  // made avail. after free play mode has been activated.
		playBack2.setEnabled(false);
		playBack3.setEnabled(false);

		/* -------------------------------------------------------*/

		setSize(1200, 1200);
		// specify the number of input and output channels that we want to open
		setAudioChannels(2, 2);
	}

	~MainContentComponent()
	{
		shutdownAudio();
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

		// You can add your drawing code here!
	}

	void resized() override
	{
		Rectangle<int> area(getLocalBounds());
		Rectangle<int> sector1(area.removeFromLeft(getLocalBounds().getWidth() / 3));
		Rectangle<int> sector2(area.removeFromLeft(getLocalBounds().getWidth() / 3));
		Rectangle<int> sector3(area.removeFromLeft(getLocalBounds().getWidth() / 3));


		int buttonSize = area.getHeight() / 20;
		int regItemSize = area.getHeight() / 6;
		int logWindowSize = area.getHeight() / 3;
		int sector3Width = sector3.getWidth();

		//sector 1
		flasher.setBounds(sector1.removeFromTop(100));
		recorder.setBounds(sector1.removeFromTop(regItemSize));
		classChosenComboBox.setBounds(sector1.removeFromTop(80).reduced(5));
		logWindow.setBounds(sector1.removeFromTop(logWindowSize));

		//sector 2
		featureExtractionButtonComponent.setBounds(sector2.removeFromTop(buttonSize));
		logWindow2.setBounds(sector2.removeFromTop(logWindowSize));
		trainNeuralNetworkButtonComponent.setBounds(sector2.removeFromTop(buttonSize));
		logWindow3.setBounds(sector2.removeFromTop(logWindowSize));

		//sector 3
		activateFreePlayButtonComponent.setBounds(sector3.removeFromTop(buttonSize));
		playBack1.setBounds(sector3.removeFromTop(regItemSize));
		playBack2.setBounds(sector3.removeFromTop(regItemSize));
		playBack3.setBounds(sector3.removeFromTop(regItemSize));
		flasher2.setBounds(sector3.removeFromTop(100));

	}


private:
	//==============================================================================

	// Your private member variables go here...
	AudioRecorderComponent recorder;
	LogWindowComponent logWindow;
	LogWindowComponent logWindow2;
	LogWindowComponent logWindow3;
	AudioPlayBackComponent playBack1;
	AudioPlayBackComponent playBack2;
	AudioPlayBackComponent playBack3;

	bool recordingClass1;
	int counterClass1;
	bool recordingClass2;
	int counterClass2;
	bool recordingClass3;
	int counterClass3;

	String message;

	/* custom filenames for creating the audio files - could pass this to recorder class.
	just a detail, not really necessary !
	String filename1 = "class1";
	String filename2 = "class1";
	String filename3 = "class1";*/

	/* IMPLEMENT THIS LATER.*/
	ComboBox classChosenComboBox;
	std::vector<String> class1recordings;
	std::vector<String> class2recordings;
	std::vector<String> class3recordings;

	//Dummy Buttons for Now - need to create them as components
	bool recordingsDone;
	FeatureExtractionComponent featureExtractionButtonComponent; //associated with bool featuresExtracted
	bool featuresExtracted;
	NeuralNetworkComponent trainNeuralNetworkButtonComponent;//associated with bool networkTrained
	bool networkTrained;
	FreePlayComponent activateFreePlayButtonComponent; //associated with both previous bools.
	bool freePlayActivated;
	FlashingComponentMain flasher;
	FlashingComponentMain flasher2;

	int class_being_recorded;
	void buttonClicked(Button* button) override
	{
		if (recordingClass1) {
			class_being_recorded = 1;
			if (counterClass1 >= 2) {
				recordingClass1 = false; //done
				recordingClass2 = true;
				recordingClass3 = false;
				message = "recordings done for class 1";
				logWindow.logMessage(message);
				class1recordings.push_back(message); //will retrieve these later.
				logWindow.clearMessages();
			}
		}
		if (recordingClass2) {
			class_being_recorded = 2;
			if (counterClass2 >= 2) {
				recordingClass1 = false; //done
				recordingClass2 = false;
				recordingClass3 = true;
				message = "recordings done for class 2";
				logWindow.logMessage(message);
				class2recordings.push_back(message); //will retrieve these later.
				logWindow.clearMessages();
			}
		}
		if (recordingClass3) {
			class_being_recorded = 3;
			if (counterClass3 >= 2) {
				recordingClass1 = false; //done
				recordingClass2 = false;
				recordingClass3 = false;
				message = "recordings done for class 3";
				logWindow.logMessage(message);
				class3recordings.push_back(message);
				class_being_recorded = -1;
				recordingsDone = true; // this allows us to go and click on Feature Extraction Button
				recorder.setEnabled(false); // weird placement for this?
				featureExtractionButtonComponent.setEnabled(true);
				logWindow2.setEnabled(true);
				logWindow.clearMessages();
			}
		}

		bool isRecording = recorder.getAudioRecorder()->isRecording();

		if (button == recorder.getButton() && !isRecording)//recorder.getButton())
		{
			if (class_being_recorded == 1) {
				message = "Class 1 being recorded";
				logWindow.logMessage(message);
				class1recordings.push_back(message);
				counterClass1++;

			}
			else if (class_being_recorded == 2) {
				message = "Class 2 being recorded";
				logWindow.logMessage(message);
				class2recordings.push_back(message);
				counterClass2++;

			}
			else if (class_being_recorded == 3) {
				message = "Class 3 being recorded";
				logWindow.logMessage(message);
				class3recordings.push_back(message);
				counterClass3++;
			}
			else {
				//do nothing.
			}
		}

		if (button == featureExtractionButtonComponent.getButton()) {
			logWindow2.logMessage("Extracting Features...");
			featuresExtracted = true;
			trainNeuralNetworkButtonComponent.setEnabled(true);
			featureExtractionButtonComponent.setEnabled(false); //disable itself
																//startTimer(500);
		}

		if (button == trainNeuralNetworkButtonComponent.getButton()) {
			logWindow3.logMessage("Training Neural Network...");
			trainNeuralNetworkButtonComponent.trainNeuralNetwork();//or actually train neural network.
			networkTrained = true;
			activateFreePlayButtonComponent.setEnabled(true);
			trainNeuralNetworkButtonComponent.setEnabled(false);//disable itself
																//startTimer(500);
		}

		if (button == activateFreePlayButtonComponent.getButton()) {
			//startTimer(500);
			freePlayActivated = true;
			playBack1.setEnabled(true);
			playBack2.setEnabled(true);
			playBack3.setEnabled(true);
			activateFreePlayButtonComponent.classifyIncomingVectors();
			
		}
	}

	void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
	{
		if (comboBoxThatHasChanged == &classChosenComboBox) {
			//determine what the choice was!

			if (classChosenComboBox.getSelectedId() == 1)
			{
				logWindow.clearMessages();
				retrieveMessages(class1recordings);
			}
			else if (classChosenComboBox.getSelectedId() == 2)
			{
				logWindow.clearMessages();
				retrieveMessages(class2recordings);
			}
			else if (classChosenComboBox.getSelectedId() == 3) {
				logWindow.clearMessages();
				retrieveMessages(class3recordings);
			}
			else {
				//nothing
			}
		}
	}

	void retrieveMessages(std::vector<String> &messageVector) {
		for (int i = 0; i < messageVector.size(); i++) {
			logWindow.logMessage(messageVector[i]);
		}
	}

	void timerCallback() override {
		//logWindow3.logMessage("Training Neural Network...");
		//stopTimer();
	}


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }
