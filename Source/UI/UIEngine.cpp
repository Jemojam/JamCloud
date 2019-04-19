#include "UIEngine.h"


UIEngine::UIEngine(AudioEngine& inEngine) :
	audioEngine(inEngine),
	toolbarComponent(inEngine)

{
	addAndMakeVisible(toolbarComponent);
	//addAndMakeVisible(channelWindow);
	//addAndMakeVisible(gridWindow);

}

UIEngine::~UIEngine()
{
}

void UIEngine::paint(Graphics& g)
{
}

void UIEngine::resized()
{
	toolbarComponent.setBounds(0, 0, getWidth(), getHeight());

}
