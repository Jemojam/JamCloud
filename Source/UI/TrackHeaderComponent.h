

#pragma once
#include "JuceHeader.h"
#include "EditViewState.h"

class TrackHeaderComponent : public Component,
                             private te::ValueTreeAllEventListener
{
public:
    TrackHeaderComponent (EditViewState&, te::Track::Ptr);
    ~TrackHeaderComponent();
    
    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    void resized() override;
    
private:
    void valueTreeChanged() override {}
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
	void addAndMakeVisibles(Component& parent, const Array<Component*>& children);
    EditViewState& editViewState;
    te::Track::Ptr track;
    
    ValueTree inputsState;
    Label trackName;
    TextButton armButton {"A"}, muteButton {"M"}, soloButton {"S"}, inputButton {"I"};
};
