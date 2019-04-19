
#include "EditComponent.h"
EditComponent::EditComponent (te::Edit& e, te::SelectionManager& sm)
    : edit (e), editViewState (e, sm)
{
    edit.state.addListener (this);
    editViewState.selectionManager.addChangeListener (this);
    
    addAndMakeVisible (playhead);
    
    markAndUpdate (updateTracks);
}

EditComponent::~EditComponent()
{
    editViewState.selectionManager.removeChangeListener (this);
    edit.state.removeListener (this);
}

void EditComponent::valueTreePropertyChanged (juce::ValueTree& v, const juce::Identifier& i)
{
    if (v.hasType (IDs::EDITVIEWSTATE))
    {
        if (i == IDs::viewX1
            || i == IDs::viewX2
            || i == IDs::viewY)
        {
            markAndUpdate (updateZoom);
        }
        else if (i == IDs::drawWaveforms)
        {
            repaint();
        }
    }
}

void EditComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& c)
{
    if (te::TrackList::isTrack (c))
        markAndUpdate (updateTracks);
}

void EditComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& c, int)
{
    if (te::TrackList::isTrack (c))
        markAndUpdate (updateTracks);
}

void EditComponent::valueTreeChildOrderChanged (juce::ValueTree& v, int a, int b)
{
    if (te::TrackList::isTrack (v.getChild (a)))
        markAndUpdate (updateTracks);
    else if (te::TrackList::isTrack (v.getChild (b)))
        markAndUpdate (updateTracks);
}

void EditComponent::handleAsyncUpdate()
{
    if (compareAndReset (updateTracks))
        buildTracks();
    else if (compareAndReset (updateZoom))
        resized();
}

void EditComponent::resized()
{
    jassert (headers.size() == tracks.size());
    
    const int trackHeight = 50, headerWidth = 150, trackGap = 2;
    
    playhead.setBounds (getLocalBounds().withTrimmedLeft (headerWidth));
    
    int y = editViewState.viewY;
    for (int i = 0; i < jmin (headers.size(), tracks.size()); i++)
    {
        auto h = headers[i];
        auto t = tracks[i];
        
        h->setBounds (0, y, headerWidth, trackHeight);
        t->setBounds (headerWidth, y, getWidth() - headerWidth, trackHeight);
        
        y += trackHeight + trackGap;
    }
    
    for (auto t : tracks)
        t->resized();
}

void EditComponent::buildTracks()
{
    tracks.clear();
    headers.clear();
    
    for (auto t : getAllTracks (edit))
    {
        TrackComponent* c = nullptr;
        
        if (t->isTempoTrack())
        {
            if (editViewState.showGlobalTrack)
                c = new TrackComponent (editViewState, t);
        }
        else if (t->isMarkerTrack())
        {
            if (editViewState.showMarkerTrack)
                c = new TrackComponent (editViewState, t);
        }
        else if (t->isChordTrack())
        {
            if (editViewState.showChordTrack)
                c = new TrackComponent (editViewState, t);
        }
        else
        {
            c = new TrackComponent (editViewState, t);
        }
        
        if (c != nullptr)
        {
            tracks.add (c);
            addAndMakeVisible (c);
            
            auto h = new TrackHeaderComponent (editViewState, t);
            headers.add (h);
            addAndMakeVisible (h);                        
        }
    }
    
    playhead.toFront (false);
    resized();
}
