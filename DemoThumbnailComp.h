/*
==============================================================================

DemoThumbnailComp.h
Created: 12 Oct 2017 10:08:43pm
Author:  Owner

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class DemoThumbnailComp : public Component,
	public ChangeListener,
	public FileDragAndDropTarget,
	public ChangeBroadcaster,
	private ScrollBar::Listener,
	private Timer
{
public:
	DemoThumbnailComp(AudioFormatManager& formatManager,
		AudioTransportSource& transportSource_)//,
											   //Slider& slider)
		: transportSource(transportSource_),
		//zoomSlider(slider),
		scrollbar(false),
		thumbnailCache(5),
		thumbnail(512, formatManager, thumbnailCache),
		isFollowingTransport(false)
	{
		thumbnail.addChangeListener(this);

		addAndMakeVisible(scrollbar);
		scrollbar.setRangeLimits(visibleRange);
		scrollbar.setAutoHide(false);
		scrollbar.addListener(this);

		currentPositionMarker.setFill(Colours::white.withAlpha(0.85f));
		addAndMakeVisible(currentPositionMarker);
	}

	~DemoThumbnailComp()
	{
		scrollbar.removeListener(this);
		thumbnail.removeChangeListener(this);
	}

	void setFile(const File& file)
	{
		if (!file.isDirectory())
		{
			thumbnail.setSource(new FileInputSource(file));
			const Range<double> newRange(0.0, thumbnail.getTotalLength());
			scrollbar.setRangeLimits(newRange);
			setRange(newRange);

			startTimerHz(40);
		}
	}

	File getLastDroppedFile() const noexcept { return lastFileDropped; }

	void setZoomFactor(double amount)
	{
		if (thumbnail.getTotalLength() > 0)
		{
			const double newScale = jmax(0.001, thumbnail.getTotalLength() * (1.0 - jlimit(0.0, 0.99, amount)));
			const double timeAtCentre = xToTime(getWidth() / 2.0f);
			setRange(Range<double>(timeAtCentre - newScale * 0.5, timeAtCentre + newScale * 0.5));
		}
	}

	void setRange(Range<double> newRange)
	{
		visibleRange = newRange;
		scrollbar.setCurrentRange(visibleRange);
		updateCursorPosition();
		repaint();
	}

	void setFollowsTransport(bool shouldFollow)
	{
		isFollowingTransport = shouldFollow;
	}

	void paint(Graphics& g) override
	{
		g.fillAll(Colours::darkgrey);
		g.setColour(Colours::lightblue);

		if (thumbnail.getTotalLength() > 0.0)
		{
			Rectangle<int> thumbArea(getLocalBounds());
			thumbArea.removeFromBottom(scrollbar.getHeight() + 4);
			thumbnail.drawChannels(g, thumbArea.reduced(2),
				visibleRange.getStart(), visibleRange.getEnd(), 1.0f);
		}
		else
		{
			g.setFont(14.0f);
			g.drawFittedText("(No audio file selected)", getLocalBounds(), Justification::centred, 2);
		}
	}

	void resized() override
	{
		scrollbar.setBounds(getLocalBounds().removeFromBottom(14).reduced(2));
	}

	void changeListenerCallback(ChangeBroadcaster*) override
	{
		// this method is called by the thumbnail when it has changed, so we should repaint it..
		repaint();
	}

	bool isInterestedInFileDrag(const StringArray& /*files*/) override
	{
		return true;
	}

	void filesDropped(const StringArray& files, int /*x*/, int /*y*/) override
	{
		lastFileDropped = File(files[0]);
		sendChangeMessage();
	}

	void mouseDown(const MouseEvent& e) override
	{
		mouseDrag(e);
	}

	void mouseDrag(const MouseEvent& e) override
	{
		if (canMoveTransport())
			transportSource.setPosition(jmax(0.0, xToTime((float)e.x)));
	}

	void mouseUp(const MouseEvent&) override
	{
		transportSource.start();
	}

	void mouseWheelMove(const MouseEvent&, const MouseWheelDetails& wheel) override
	{
		if (thumbnail.getTotalLength() > 0.0)
		{
			double newStart = visibleRange.getStart() - wheel.deltaX * (visibleRange.getLength()) / 10.0;
			newStart = jlimit(0.0, jmax(0.0, thumbnail.getTotalLength() - (visibleRange.getLength())), newStart);

			if (canMoveTransport())
				setRange(Range<double>(newStart, newStart + visibleRange.getLength()));

			if (wheel.deltaY != 0.0f)
				//zoomSlider.setValue(zoomSlider.getValue() - wheel.deltaY);

				repaint();
		}
	}

private:

	AudioTransportSource& transportSource;
	//Slider& zoomSlider;
	ScrollBar scrollbar;

	AudioThumbnailCache thumbnailCache;
	AudioThumbnail thumbnail;
	Range<double> visibleRange;
	bool isFollowingTransport;
	File lastFileDropped;

	DrawableRectangle currentPositionMarker;

	float timeToX(const double time) const
	{
		return getWidth() * (float)((time - visibleRange.getStart()) / (visibleRange.getLength()));
	}

	double xToTime(const float x) const
	{
		return (x / getWidth()) * (visibleRange.getLength()) + visibleRange.getStart();
	}

	bool canMoveTransport() const noexcept
	{
		return !(isFollowingTransport && transportSource.isPlaying());
	}

	void scrollBarMoved(ScrollBar* scrollBarThatHasMoved, double newRangeStart) override
	{
		if (scrollBarThatHasMoved == &scrollbar)
			if (!(isFollowingTransport && transportSource.isPlaying()))
				setRange(visibleRange.movedToStartAt(newRangeStart));
	}

	void timerCallback() override
	{
		if (canMoveTransport())
			updateCursorPosition();
		else
			setRange(visibleRange.movedToStartAt(transportSource.getCurrentPosition() - (visibleRange.getLength() / 2.0)));
	}

	void updateCursorPosition()
	{
		currentPositionMarker.setVisible(transportSource.isPlaying() || isMouseButtonDown());

		currentPositionMarker.setRectangle(Rectangle<float>(timeToX(transportSource.getCurrentPosition()) - 0.75f, 0,
			1.5f, (float)(getHeight() - scrollbar.getHeight())));
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoThumbnailComp)
};
