#pragma once
#include "JuceHeader.h"
#include "../Audio/AudioEngine.h"

namespace IDs
{
    #define DECLARE_ID(name)  const juce::Identifier name (#name);
    DECLARE_ID (EDITVIEWSTATE)
    DECLARE_ID (showGlobalTrack)
    DECLARE_ID (showMarkerTrack)
    DECLARE_ID (showChordTrack)
    DECLARE_ID (viewX1)
    DECLARE_ID (viewX2)
    DECLARE_ID (viewY)
    DECLARE_ID (drawWaveforms)
    #undef DECLARE_ID
}


class EditViewState
{
public:
    EditViewState (AudioEngine& inEngine, te::SelectionManager& s);
    
    int timeToX (double time, int width) const;   
    double xToTime (int x, int width) const;
    
    CachedValue<bool> showGlobalTrack, showMarkerTrack, showChordTrack, drawWaveforms;
    CachedValue<double> viewX1, viewX2, viewY;
    
    ValueTree state;
	te::SelectionManager selectionManager;
	AudioEngine& audioEngine;

	
};