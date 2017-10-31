/*
==============================================================================

FeatureExtractionComponent.h
Created: 16 Oct 2017 10:30:27pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class FeatureExtractionComponent : public Component,
	private Button::Listener,
	private Timer
{
public:
	FeatureExtractionComponent()
	{
		// In your constructor, you should add any child components, and
		// initialise any special settings that your component needs.
		extractFeaturesButton.setButtonText("Performing Feature Extraction");
		addAndMakeVisible(extractFeaturesButton);
		extractFeaturesButton.addListener(this);
		message = "Starting Feature Extraction";
		setSize(200, 200);
	}

	~FeatureExtractionComponent()
	{
	}

	void paint(Graphics& g) override
	{
		/* This demo code just fills the component's background and
		draws some placeholder text to get you started.

		You should replace everything in this method with your own
		drawing code..
		*/

		g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

		g.setColour(Colours::grey);
		g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

		g.setColour(Colours::white);
		g.setFont(14.0f);
		/*g.drawText ("FeatureExtractionComponent", getLocalBounds(),
		Justification::centred, true);   // draw some placeholder text*/
	}

	void resized() override
	{
		// This method is where you should set the bounds of any child
		// components that your component contains..
		Rectangle<int> brownies(getLocalBounds());
		int buttonHeight = 80;
		extractFeaturesButton.setBounds(brownies.removeFromTop(buttonHeight).reduced(5));

	}
	TextButton* getButton() {
		return &extractFeaturesButton;
	}
private:

	TextButton extractFeaturesButton;
	String message;
	void timerCallback() override
	{
		extractFeaturesButton.setButtonText("Feature Extraction Starting");
	}

	void buttonClicked(Button* button) override
	{
		if (button == &extractFeaturesButton)
		{
			startTimer(500);

		}
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeatureExtractionComponent)
};
