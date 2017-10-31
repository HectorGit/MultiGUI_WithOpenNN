#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//==============================================================================
/*
*/
class FlashingComponent : public Component,
	public ChangeBroadcaster,
	private Timer
{
public:
	FlashingComponent()
		: flashAlpha(0.0f),
		colour(Colours::red)
	{
	}

	void startFlashing()
	{
		flashAlpha = 1.0f;
		startTimer(300);
	}

	void stopFlashing()
	{
		flashAlpha = 0.0f;
		stopTimer();
		repaint();
	}

	void setFlashColour(const Colour newColour)
	{
		colour = newColour;
		repaint();
	}

	void paint(Graphics& g) override
	{
		g.setColour(colour.overlaidWith(Colours::white.withAlpha(flashAlpha)));
		g.fillEllipse(getLocalBounds().toFloat());
	}

	void mouseDown(const MouseEvent&) override
	{
		startFlashing();
	}

private:
	float flashAlpha;
	Colour colour;

	void timerCallback() override
	{
		flashAlpha -= 0.075f;

		if (flashAlpha < 0.05f)
		{
			stopFlashing();
			sendChangeMessage();
		}

		repaint();
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FlashingComponent)
};
