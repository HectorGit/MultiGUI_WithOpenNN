/*
==============================================================================

LogWindowComponent.h
Created: 13 Oct 2017 4:38:02pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LogWindowComponent : public AudioAppComponent,
	private AudioIODeviceCallback,
	public ChangeListener
{
public:
	LogWindowComponent() : audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
		true, // showMidiInputOptions must be true
		true, true, false)
	{
		setOpaque(true);

		audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
		audioDeviceManager.addAudioCallback(this);
		audioDeviceManager.addChangeListener(this);

		/*addAndMakeVisible(audioSetupComp
		= new AudioDeviceSelectorComponent(MainAppWindow::getSharedAudioDeviceManager(),
		0, 256, 0, 256, true, true, true, false));*/

		addAndMakeVisible(diagnosticsBox);
		diagnosticsBox.setMultiLine(true);
		diagnosticsBox.setReturnKeyStartsNewLine(true);
		diagnosticsBox.setReadOnly(true);
		diagnosticsBox.setScrollbarsShown(true);
		diagnosticsBox.setCaretVisible(false);
		diagnosticsBox.setPopupMenuEnabled(true);

		//MainAppWindow::getSharedAudioDeviceManager().addChangeListener(this);

		//logMessage("Audio device diagnostics:\n");
		dumpDeviceInfo();

		setSize(400, 400);
		setAudioChannels(2, 2);

	}

	~LogWindowComponent()
	{
		//shutdownAudio();
		//MainAppWindow::getSharedAudioDeviceManager().removeChangeListener(this);
		audioDeviceManager.removeChangeListener(this);
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
		//        g.fillAll (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));

		// You can add your drawing code here!
	}

	void resized() override
	{
		// This is called when the MainContentComponent is resized.
		// If you add any child components, this is where you should
		// update their positions.
		Rectangle<int> r(getLocalBounds().reduced(4));
		Rectangle<int> usedArea(r.removeFromTop(200).reduced(20));
		//audioSetupComp->setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
		//audioSetupComp.setBounds(r.removeFromTop(proportionOfHeight(0.20f)));//setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
		//audioSetupComp.setBounds(usedArea.removeFromTop(proportionOfHeight(0.60f)));//setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
		diagnosticsBox.setBounds(usedArea.removeFromTop(proportionOfHeight(0.90f)));
	}

	//void changeListenerCallback(Something &Source) override
	//{

	//}

	void dumpDeviceInfo()
	{
		AudioDeviceManager& dm = audioDeviceManager;//MainAppWindow::getSharedAudioDeviceManager();

													/*logMessage("--------------------------------------");
													logMessage("Current audio device type: " + (dm.getCurrentDeviceTypeObject() != nullptr
													? dm.getCurrentDeviceTypeObject()->getTypeName()
													: "<none>"));
													*/
		if (AudioIODevice* device = dm.getCurrentAudioDevice())
		{
			/*logMessage("Current audio device: " + device->getName().quoted());
			logMessage("Sample rate: " + String(device->getCurrentSampleRate()) + " Hz");
			logMessage("Block size: " + String(device->getCurrentBufferSizeSamples()) + " samples");
			logMessage("Output Latency: " + String(device->getOutputLatencyInSamples()) + " samples");
			logMessage("Input Latency: " + String(device->getInputLatencyInSamples()) + " samples");
			logMessage("Bit depth: " + String(device->getCurrentBitDepth()));
			logMessage("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
			logMessage("Active input channels: " + getListOfActiveBits(device->getActiveInputChannels()));
			logMessage("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
			logMessage("Active output channels: " + getListOfActiveBits(device->getActiveOutputChannels()));
			*/
		}
		else
		{
			//logMessage("No audio device open");
		}
	}

	void logMessage(const String& m)
	{
		diagnosticsBox.moveCaretToEnd();
		diagnosticsBox.insertTextAtCaret(m + newLine);
	}

	void clearMessages() {
		diagnosticsBox.clear();
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

private:

	// Your private member variables go here...
	AudioDeviceManager audioDeviceManager;         // [3]
	AudioDeviceSelectorComponent audioSetupComp;   // [4]
												   //ScopedPointer<AudioDeviceSelectorComponent> audioSetupComp;
	TextEditor diagnosticsBox;

	void changeListenerCallback(ChangeBroadcaster*) override
	{
		dumpDeviceInfo();
	}

	void lookAndFeelChanged() override
	{
		diagnosticsBox.applyFontToAllText(diagnosticsBox.getFont());
	}

	static String getListOfActiveBits(const BitArray& b)
	{
		StringArray bits;

		for (int i = 0; i <= b.getHighestBit(); ++i)
			if (b[i])
				bits.add(String(i));

		return bits.joinIntoString(", ");
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LogWindowComponent)
};

/*

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
class MainContentComponent : public AudioAppComponent,
private AudioIODeviceCallback,
public ChangeListener
{
public:
//==============================================================================
MainContentComponent() : audioSetupComp(audioDeviceManager, 0, 0, 0, 256,
true, // showMidiInputOptions must be true
true, true, false)
{

setOpaque(true);

audioDeviceManager.initialise(0, 2, nullptr, true, String(), nullptr);
audioDeviceManager.addAudioCallback(this);
audioDeviceManager.addChangeListener(this);

//addAndMakeVisible(audioSetupComp
= new AudioDeviceSelectorComponent(MainAppWindow::getSharedAudioDeviceManager(),
0, 256, 0, 256, true, true, true, false));

addAndMakeVisible(diagnosticsBox);
diagnosticsBox.setMultiLine(true);
diagnosticsBox.setReturnKeyStartsNewLine(true);
diagnosticsBox.setReadOnly(true);
diagnosticsBox.setScrollbarsShown(true);
diagnosticsBox.setCaretVisible(false);
diagnosticsBox.setPopupMenuEnabled(true);

//MainAppWindow::getSharedAudioDeviceManager().addChangeListener(this);

logMessage("Audio device diagnostics:\n");
dumpDeviceInfo();

setSize(800, 600);
setAudioChannels(2, 2);
}

~MainContentComponent()
{
//shutdownAudio();
//MainAppWindow::getSharedAudioDeviceManager().removeChangeListener(this);
audioDeviceManager.removeChangeListener(this);
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
//        g.fillAll (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));

// You can add your drawing code here!
}

void resized() override
{
// This is called when the MainContentComponent is resized.
// If you add any child components, this is where you should
// update their positions.
Rectangle<int> r(getLocalBounds().reduced(4));
//audioSetupComp->setBounds(r.removeFromTop(proportionOfHeight(0.65f)));
audioSetupComp.setBounds(r.removeFromTop(proportionOfHeight(0.65f)));//setBounds(r.removeFromTop(proportionOfHeight(0.65f)));

diagnosticsBox.setBounds(r);
}

//void changeListenerCallback(Something &Source) override
//{

//}

void dumpDeviceInfo()
{
AudioDeviceManager& dm = audioDeviceManager;//MainAppWindow::getSharedAudioDeviceManager();

logMessage("--------------------------------------");
logMessage("Current audio device type: " + (dm.getCurrentDeviceTypeObject() != nullptr
? dm.getCurrentDeviceTypeObject()->getTypeName()
: "<none>"));

if (AudioIODevice* device = dm.getCurrentAudioDevice())
{
logMessage("Current audio device: " + device->getName().quoted());
logMessage("Sample rate: " + String(device->getCurrentSampleRate()) + " Hz");
logMessage("Block size: " + String(device->getCurrentBufferSizeSamples()) + " samples");
logMessage("Output Latency: " + String(device->getOutputLatencyInSamples()) + " samples");
logMessage("Input Latency: " + String(device->getInputLatencyInSamples()) + " samples");
logMessage("Bit depth: " + String(device->getCurrentBitDepth()));
logMessage("Input channel names: " + device->getInputChannelNames().joinIntoString(", "));
logMessage("Active input channels: " + getListOfActiveBits(device->getActiveInputChannels()));
logMessage("Output channel names: " + device->getOutputChannelNames().joinIntoString(", "));
logMessage("Active output channels: " + getListOfActiveBits(device->getActiveOutputChannels()));
}
else
{
logMessage("No audio device open");
}
}

void logMessage(const String& m)
{
diagnosticsBox.moveCaretToEnd();
diagnosticsBox.insertTextAtCaret(m + newLine);
}

void audioDeviceIOCallback(const float** //inputChannelData, int //numInputChannels,
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

private:
//==============================================================================

// Your private member variables go here...
AudioDeviceManager audioDeviceManager;         // [3]
AudioDeviceSelectorComponent audioSetupComp;   // [4]
//ScopedPointer<AudioDeviceSelectorComponent> audioSetupComp;
TextEditor diagnosticsBox;

void changeListenerCallback(ChangeBroadcaster*) override
{
dumpDeviceInfo();
}

void lookAndFeelChanged() override
{
diagnosticsBox.applyFontToAllText(diagnosticsBox.getFont());
}

static String getListOfActiveBits(const BitArray& b)
{
StringArray bits;

for (int i = 0; i <= b.getHighestBit(); ++i)
if (b[i])
bits.add(String(i));

return bits.joinIntoString(", ");
}

JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent() { return new MainContentComponent(); }

*/


