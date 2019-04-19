#pragma once
#include "JuceHeader.h"
#include "../Audio/AudioEngine.h"
#include "EditComponent.h"

class ToolbarComponent  : public Component,
                          private ChangeListener
{
public:

    ToolbarComponent(AudioEngine & inEngine);
    ~ToolbarComponent();
   

    void paint (Graphics& g) override;
    void resized() override;


private:
    
    //te::Engine engine { ProjectInfo::projectName };
    //te::SelectionManager selectionManager { engine };
    //std::unique_ptr<te::Edit> edit;
    //std::unique_ptr<EditComponent> editComponent;

    TextButton settingsButton { "Settings" }, newEditButton { "New" }, playPauseButton { "Play" }, recordButton { "Record" },
               showEditButton { "Show Edit" }, newTrackButton { "New Track" }, clearTracksButton { "Clear Tracks" }, deleteButton { "Delete" };
    Label editNameLabel { "No Edit Loaded" };
    ToggleButton showWaveformButton { "Show Waveforms" };
	std::unique_ptr<EditComponent> editComponent;

	AudioEngine& audioEngine;

    void setupButtons();
    
    void updatePlayButtonText();
   
    void updateRecordButtonText();
   
    void createNewEdit (File editFile = {});
    
    void createTracksAndAssignInputs();
  
    void changeListenerCallback (ChangeBroadcaster* source) override;

	void addAndMakeVisibles(Component& parent, const Array<Component*>& children);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarComponent)
};
