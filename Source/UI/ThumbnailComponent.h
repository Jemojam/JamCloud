#pragma once

#include "JuceHeader.h"
#include "../Audio/AudioEngine.h"

class ThumbnailComponent : public Component
{
public:

	ThumbnailComponent(AudioEngine & inEngine);

	void setFile(const te::AudioFile& file);

	void paint(Graphics& g) override;
	
	void mouseDown(const MouseEvent& e) override;
	void mouseDrag(const MouseEvent& e) override;
	void mouseUp(const MouseEvent&) override;

private:
	//te::TransportControl& transport;
	//te::SmartThumbnail smartThumbnail{ transport.engine, te::AudioFile(), *this, nullptr };
	DrawableRectangle cursor;
	//te::LambdaTimer cursorUpdater;

	void updateCursorPosition();

	AudioEngine& audioEngine;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ThumbnailComponent)
};
