
#pragma once
#include "JuceHeader.h"
#include "EditViewState.h"

class ClipComponent : public Component
{
public:
    ClipComponent (EditViewState&, te::Clip::Ptr);
    
    void paint (Graphics& g) override;
    void mouseDown (const MouseEvent& e) override;
    
    te::Clip& getClip() { return *clip; }
    
protected:
    EditViewState& editViewState;
    te::Clip::Ptr clip;
};