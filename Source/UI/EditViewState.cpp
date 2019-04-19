
#include "EditViewState.h"

EditViewState::EditViewState(AudioEngine& inEngine, te::SelectionManager& s)
	: audioEngine(inEngine)
{
		auto stateOfChild = audioEngine.getChildWithName();
		state = stateOfChild.getOrCreateChildWithName(IDs::EDITVIEWSTATE, nullptr);
        auto um = audioEngine.getUndoManager();
        
        showGlobalTrack.referTo (state, IDs::showGlobalTrack, um, false);
        showMarkerTrack.referTo (state, IDs::showMarkerTrack, um, false);
        showChordTrack.referTo (state, IDs::showChordTrack, um, false);
        drawWaveforms.referTo (state, IDs::drawWaveforms, um, true);

        viewX1.referTo (state, IDs::viewX1, um, 0);
        viewX2.referTo (state, IDs::viewX2, um, 60);
        viewY.referTo (state, IDs::viewY, um, 0);
}
    
    int EditViewState::timeToX (double time, int width) const
    {
        return roundToInt (((time - viewX1) * width) / (viewX2 - viewX1));
    }
    
    double EditViewState::xToTime (int x, int width) const
    {
        return (double (x) / width) * (viewX2 - viewX1) + viewX1;
    }


