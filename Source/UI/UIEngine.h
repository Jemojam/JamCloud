
#pragma once
#include "JuceHeader.h"
#include "../Audio/AudioEngine.h"
#include "ToolbarComponent.h"

class UIEngine : public Component
{
public:
	UIEngine(AudioEngine& inEngine);
	~UIEngine();

	void paint(Graphics& g) override;
	void resized() override;

private:
	AudioEngine& audioEngine;
	ToolbarComponent toolbarComponent;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIEngine)
};



