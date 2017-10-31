/*
==============================================================================

FlashingComponentMain.h
Created: 17 Oct 2017 5:02:52pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "FlashingComponent.h"

//==============================================================================
/*
*/
class FlashingComponentMain : public Component
{
public:
	FlashingComponentMain()
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		setOpaque(true);

		const int diameter = 100;

		flasher1 = new FlashingComponent();
		flasher2 = new FlashingComponent();
		flasher3 = new FlashingComponent();

		flasher1->setSize(diameter, diameter);
		flasher2->setSize(diameter, diameter);
		flasher3->setSize(diameter, diameter);

		flasher1->setFlashColour(Colours::green);
		flasher2->setFlashColour(Colours::yellow);
		flasher3->setFlashColour(Colours::red);

		addAndMakeVisible(flasher1);
		addAndMakeVisible(flasher2);
		addAndMakeVisible(flasher3);

		lflasher1.setText(" Class 1 ", dontSendNotification);
		lflasher1.setSize(80, 80);
		lflasher1.attachToComponent(flasher1, false);
		addAndMakeVisible(lflasher1);
		lflasher2.setText(" Class 2 ", dontSendNotification);
		lflasher2.setSize(80, 80);
		lflasher2.attachToComponent(flasher2, false);
		addAndMakeVisible(lflasher2);
		lflasher3.setText(" Class 3 ", dontSendNotification);
		lflasher2.setSize(80, 80);
		lflasher3.attachToComponent(flasher3, false);
		addAndMakeVisible(lflasher3);

		makeFirstFlash();

		makeSecondFlash();

		makeThirdFlash();

		setSize(300, 100);
	}

	~FlashingComponentMain()
	{
	}

	void paint(Graphics& g) override
	{
		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

	}

	void resized() override
	{
		Rectangle<int> area(getLocalBounds().removeFromBottom(40));
		Rectangle<int> textSpace(getLocalBounds().removeFromTop(20));
		Rectangle<int> spaceUsed(getLocalBounds().removeFromTop(100));

		int itemWidth = 100;
		flasher1->setBounds(spaceUsed.removeFromLeft(itemWidth));
		lflasher1.setBounds(textSpace.removeFromLeft(itemWidth));
		flasher2->setBounds(spaceUsed.removeFromLeft(itemWidth));
		lflasher2.setBounds(textSpace.removeFromLeft(itemWidth));
		flasher3->setBounds(spaceUsed.removeFromLeft(itemWidth));
		lflasher3.setBounds(textSpace.removeFromLeft(itemWidth));

	}

private:

	FlashingComponent* flasher1;
	FlashingComponent* flasher2;
	FlashingComponent* flasher3;

	void makeFirstFlash()
	{
		flasher1->startFlashing();
	}
	void makeSecondFlash() {
		flasher2->startFlashing();
	}
	void makeThirdFlash() {
		flasher3->startFlashing();
	}

	Label lflasher1;
	Label lflasher2;
	Label lflasher3;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlashingComponentMain)
};
