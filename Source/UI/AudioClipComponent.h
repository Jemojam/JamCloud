/*
  ==============================================================================

    AudioClipComponent.h
    Created: 16 Apr 2019 2:16:10pm
    Author:  Jordan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "ClipComponent.h"
#include "EditViewState.h"

class AudioClipComponent : public ClipComponent
{
public:
    AudioClipComponent (EditViewState&, te::Clip::Ptr);
    
    te::WaveAudioClip* getWaveAudioClip() { return dynamic_cast<te::WaveAudioClip*> (clip.get()); }
    
    void paint (Graphics& g) override;
    
private:
    void updateThumbnail();
    void drawWaveform (Graphics& g, te::AudioClipBase& c, te::SmartThumbnail& thumb, Colour colour,
                       int left, int right, int y, int h, int xOffset, bool drawLoopMarkers);
    void drawChannels (Graphics& g, te::SmartThumbnail& thumb, Rectangle<int> area, bool useHighRes,
                       te::EditTimeRange time, bool useLeft, bool useRight,
                       float leftGain, float rightGain);

    std::unique_ptr<te::SmartThumbnail> thumbnail;
};