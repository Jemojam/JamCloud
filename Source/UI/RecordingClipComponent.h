
#pragma once
#include "JuceHeader.h"
#include "EditViewState.h"

class RecordingClipComponent : public Component,
                               private Timer
{
public:
    RecordingClipComponent (te::Track::Ptr t, EditViewState&);
    
    void paint (Graphics& g) override;
    
private:
    void timerCallback() override;
    void updatePosition();
    void initialiseThumbnailAndPunchTime();
    void drawThumbnail (Graphics& g, Colour waveformColour) const;
    bool getBoundsAndTime (Rectangle<int>& bounds, Range<double>& times) const;
    
    te::Track::Ptr track;
    EditViewState& editViewState;
    
    te::RecordingThumbnailManager::Thumbnail::Ptr thumbnail;
    double punchInTime = -1.0;
};