

#include "ToolbarComponent.h"

    ToolbarComponent::ToolbarComponent(AudioEngine & inEngine) : audioEngine(inEngine)
    {
        settingsButton.onClick  = [this] { audioEngine.showAudioDeviceSettings(); createTracksAndAssignInputs(); };
        newEditButton.onClick = [this] { createNewEdit(); };
        
        updatePlayButtonText();
        updateRecordButtonText();
        editNameLabel.setJustificationType (Justification::centred);
        addAndMakeVisibles (*this, { &settingsButton, &newEditButton, &playPauseButton, &recordButton, &showEditButton,
                                    &newTrackButton, &clearTracksButton, &deleteButton, &editNameLabel, &showWaveformButton });

        deleteButton.setEnabled (false);
        createNewEdit (File::getSpecialLocation (File::tempDirectory).getNonexistentChildFile ("Test", ".tracktionedit", false));
	
		audioEngine.getSelectionManager()->addChangeListener (this);
        
        setupButtons();
        
        setSize (700, 500);
    }

    ToolbarComponent::~ToolbarComponent()
    {
        
    }
	 void ToolbarComponent::addAndMakeVisibles(Component& parent, const Array<Component*>& children)
	{
		for (auto c : children)
			parent.addAndMakeVisible(c);
	}

    void ToolbarComponent::paint (Graphics& g)
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    }

    void ToolbarComponent::resized()
    {
        auto r = getLocalBounds();
        int w = r.getWidth() / 8;
        auto topR = r.removeFromTop (30);
        settingsButton.setBounds (topR.removeFromLeft (w).reduced (2));
        newEditButton.setBounds (topR.removeFromLeft (w).reduced (2));
        playPauseButton.setBounds (topR.removeFromLeft (w).reduced (2));
        recordButton.setBounds (topR.removeFromLeft (w).reduced (2));
        showEditButton.setBounds (topR.removeFromLeft (w).reduced (2));
        newTrackButton.setBounds (topR.removeFromLeft (w).reduced (2));
        clearTracksButton.setBounds (topR.removeFromLeft (w).reduced (2));
        deleteButton.setBounds (topR.removeFromLeft (w).reduced (2));
        topR = r.removeFromTop (30);
        showWaveformButton.setBounds (topR.removeFromLeft (w * 2).reduced (2));
        editNameLabel.setBounds (topR);
        
        if (editComponent != nullptr)
            editComponent->setBounds (r);
    }

    void ToolbarComponent::setupButtons()
    {
        playPauseButton.onClick = [this]
        {
			
			bool wasRecording = audioEngine.getTransport().isRecording();
			audioEngine.togglePlay();
			if (wasRecording)
				audioEngine.saveFile();
        };
        recordButton.onClick = [this]
        {
            bool wasRecording = audioEngine.getTransport().isRecording();
			audioEngine.toggleRecord ();
            if (wasRecording)
				audioEngine.saveFile();
        };
        newTrackButton.onClick = [this]
        {
			audioEngine.addTrack();
        };
        clearTracksButton.onClick = [this]
        {
			audioEngine.deleteAllTracks();     
        };
        deleteButton.onClick = [this]
        {
			audioEngine.deleteClipOrTrack();
        };
        showWaveformButton.onClick = [this]
        {
            auto& evs = editComponent->getEditViewState();
            evs.drawWaveforms = ! evs.drawWaveforms.get();
            showWaveformButton.setToggleState (evs.drawWaveforms, dontSendNotification);
        };
    }
    
    void ToolbarComponent::updatePlayButtonText()
    {
        if (audioEngine.wasEditCreated())
            playPauseButton.setButtonText (audioEngine.getTransport().isPlaying() ? "Stop" : "Play");
    }
    
    void ToolbarComponent::updateRecordButtonText()
    {
        if (audioEngine.wasEditCreated())
            recordButton.setButtonText (audioEngine.getTransport().isRecording() ? "Abort" : "Record");
    }

    void ToolbarComponent::createNewEdit (File editFile = {})
    {
		audioEngine.createNewFile();
		editComponent = nullptr;

		auto& transport = audioEngine.getTransport();
		transport.addChangeListener(this);

		editNameLabel.setText(editFile.getFileNameWithoutExtension(), dontSendNotification);
		showEditButton.onClick = [editFile]
		{
			editFile.revealToUser();
		};

		createTracksAndAssignInputs();
		
		
		/*need to continue code copy...*/
	
		editComponent = std::make_unique<EditComponent>(audioEngine.getEdit(), audioEngine.getSelectionManager());
		addAndMakeVisible(*editComponent);
		resized();
    }
    
    void ToolbarComponent::createTracksAndAssignInputs()
    {
		audioEngine.createTracksAndAssignInputs();
    }
    
    void ToolbarComponent::changeListenerCallback (ChangeBroadcaster* source)
    {
        if (audioEngine.wasEditCreated() && source == &audioEngine.getTransport())
        {
            updatePlayButtonText();
            updateRecordButtonText();
        }
        else if (source == audioEngine.getSelectionManager())
        {
            auto sel = audioEngine.getSelectionManager()->getSelectedObject (0);
            deleteButton.setEnabled (dynamic_cast<te::Clip*> (sel) != nullptr || dynamic_cast<te::Track*> (sel) != nullptr);
        }
    }

   

