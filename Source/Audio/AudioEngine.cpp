
#include "AudioEngine.h"

using namespace tracktion_engine;

AudioEngine::AudioEngine()
{
	edit = std::make_unique<Edit>(engine, createEmptyEdit(), Edit::forEditing, nullptr, 0);
	selectionManager= std::make_unique<SelectionManager>(engine);

	
	//removeAllTracks();
}

AudioEngine::~AudioEngine()
{
	engine.getTemporaryFileManager().getTempDirectory().deleteRecursively();
}

te::Project::Ptr AudioEngine::createTempProject(te::Engine& engine)
{
	auto file = engine.getTemporaryFileManager().getTempDirectory().getChildFile("temp_project").withFileExtension(te::projectFileSuffix);
	te::ProjectManager::TempProject tempProject(*te::ProjectManager::getInstance(), file, true);
	return tempProject.project;
}

void AudioEngine::showAudioDeviceSettings()
{
	DialogWindow::LaunchOptions o;
	o.dialogTitle = TRANS("Audio Settings");
	o.dialogBackgroundColour = LookAndFeel::getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId);
	o.content.setOwned(new AudioDeviceSelectorComponent(engine.getDeviceManager().deviceManager,
		0, 512, 1, 512, false, false, true, true));
	o.content->setSize(400, 600);
	o.launchAsync();

}

void AudioEngine::browseForAudioFile(te::Engine& engine, std::function<void(const File&)> fileChosenCallback)
{
	auto fc = std::make_shared<FileChooser>("Please select an audio file to load...",
		engine.getPropertyStorage().getDefaultLoadSaveDirectory("pitchAndTimeExample"),
		engine.getAudioFileFormatManager().readFormatManager.getWildcardForAllFormats());

	fc->launchAsync(FileBrowserComponent::openMode + FileBrowserComponent::canSelectFiles,
		[fc, &engine, callback = std::move(fileChosenCallback)](const FileChooser&)
	{
		const auto f = fc->getResult();

		if (f.existsAsFile())
			engine.getPropertyStorage().setDefaultLoadSaveDirectory("pitchAndTimeExample", f.getParentDirectory());

		callback(f);
	});
}

void AudioEngine::removeAllClips(te::AudioTrack& track)
{
	auto clips = track.getClips();

	for (int i = clips.size(); --i >= 0;)
		clips.getUnchecked(i)->removeFromParentTrack();
}

te::AudioTrack* AudioEngine::getOrInsertAudioTrackAt(te::Edit& edit, int index)
{
	edit.ensureNumberOfAudioTracks(index + 1);
	return te::getAudioTracks(edit)[index];
}

te::WaveAudioClip::Ptr AudioEngine::loadAudioFileAsClip(te::Edit& edit, const File& file)
{
	// Find the first track and delete all clips from it
	if (auto track = getOrInsertAudioTrackAt(edit, 0))
	{
		removeAllClips(*track);

		// Add a new clip to this track
		te::AudioFile audioFile(file);

		if (audioFile.isValid())
			if (auto newClip = track->insertWaveClip(file.getFileNameWithoutExtension(), file,
				{ { 0.0, audioFile.getLength() }, 0.0 }, false))
				return newClip;
	}

	return {};
}

template<typename ClipType>
typename ClipType::Ptr AudioEngine::loopAroundClip(ClipType& clip)
{
	auto& transport = clip.edit.getTransport();
	transport.setLoopRange(clip.getEditTimeRange());
	transport.looping = true;
	transport.position = 0.0;
	transport.play(false);

	return clip;
}

void AudioEngine::togglePlay()
{
	auto& transport = getTransport();

	if (transport.isPlaying())
		transport.stop(false, false);
	else
		transport.play(false);
}

void AudioEngine::toggleRecord()
{
	auto& transport = getTransport();

	if (transport.isRecording())
		transport.stop(true, false);
	else
		transport.record(false);
}

void AudioEngine::saveFile()
{
	te::EditFileOperations(*edit).save(true, true, false);
}

void AudioEngine::addTrack()
{
	edit->ensureNumberOfAudioTracks(getAudioTracks(*edit).size() + 1);
}

void AudioEngine::deleteAllTracks()
{
	for (auto t : te::getAudioTracks(*edit))
		edit->deleteTrack(t);
}

void AudioEngine::deleteClipOrTrack()
{
	auto sel = selectionManager->getSelectedObject(0);
	if (auto clip = dynamic_cast<te::Clip*> (sel))
	{
		clip->removeFromParentTrack();
	}
	else if (auto track = dynamic_cast<te::Track*> (sel))
	{
		if (!(track->isMarkerTrack() || track->isTempoTrack() || track->isChordTrack()))
			edit->deleteTrack(track);
	}
}

bool AudioEngine::wasEditCreated()
{
	//checks if there is an edit..
	//used alot in the UI..
	if (edit.get() != nullptr)
	{
		return true;
	}
	return false;
}


void AudioEngine::armTrack(te::AudioTrack& t, bool arm, int position = 0)
{
	auto& edit = t.edit;
	for (auto instance : edit.getAllInputDevices())
		if (instance->getTargetTrack() == &t && instance->getTargetIndex() == position)
			instance->setRecordingEnabled(arm);
}

bool AudioEngine::isTrackArmed(te::AudioTrack& t, int position = 0)
{
	auto& edit = t.edit;
	for (auto instance : edit.getAllInputDevices())
		if (instance->getTargetTrack() == &t && instance->getTargetIndex() == position)
			return instance->isRecordingEnabled();

	return false;
}



bool AudioEngine::isInputMonitoringEnabled(te::AudioTrack& t, int position = 0)
{
	auto& edit = t.edit;
	for (auto instance : edit.getAllInputDevices())
		if (instance->getTargetTrack() == &t && instance->getTargetIndex() == position)
			return instance->getInputDevice().isEndToEndEnabled();

	return false;
}

void AudioEngine::enableInputMonitoring(te::AudioTrack& t, bool im, int position = 0)
{
	if (isInputMonitoringEnabled(t, position) != im)
	{
		auto& edit = t.edit;
		for (auto instance : edit.getAllInputDevices())
			if (instance->getTargetTrack() == &t && instance->getTargetIndex() == position)
				instance->getInputDevice().flipEndToEnd();
	}
}

bool AudioEngine::trackHasInput(te::AudioTrack& t, int position = 0)
{
	auto& edit = t.edit;
	for (auto instance : edit.getAllInputDevices())
		if (instance->getTargetTrack() == &t && instance->getTargetIndex() == position)
			return true;

	return false;
}

void AudioEngine::createTracksAndAssignInputs()
{
	auto& dm = engine.getDeviceManager();

	for (int i = 0; i < dm.getNumWaveInDevices(); i++)
		if (auto wip = dm.getWaveInDevice(i))
			wip->setStereoPair(false);

	for (int i = 0; i < dm.getNumWaveInDevices(); i++)
	{
		if (auto wip = dm.getWaveInDevice(i))
		{
			wip->setEndToEnd(true);
			wip->setEnabled(true);
		}
	}

	edit->restartPlayback();

	int trackNum = 0;
	for (auto instance : edit->getAllInputDevices())
	{
		if (instance->getInputDevice().getDeviceType() == te::InputDevice::waveDevice)
		{
			if (auto t = getOrInsertAudioTrackAt(*edit, trackNum))
			{
				instance->setTargetTrack(t, 0);
				instance->setRecordingEnabled(true);

				trackNum++;
			}
		}
	}

	edit->restartPlayback();
}

te::SelectionManager* AudioEngine::getSelectionManager()
{
	return selectionManager.get();
}

te::SmartThumbnail* AudioEngine::getSmartThumbnail()
{
	return smartThumbnail.get();
}

te::LambdaTimer AudioEngine::getCursorUpdater()
{
	return cursorUpdater;
}

te::TransportControl & AudioEngine::getTransport()
{
	return edit->getTransport();
}

te::Edit * AudioEngine::getEdit()
{
	return edit.get();
}

ValueTree AudioEngine::getChildWithName()
{
	return edit->state;
}

UndoManager* AudioEngine::getUndoManager()
{
	return &edit->getUndoManager();
}


void AudioEngine::createNewFile(File editFile = {})
{
        if (editFile == File())
        {
            FileChooser fc ("New Edit", File::getSpecialLocation (File::userDocumentsDirectory), "*.tracktionedit");
            if (fc.browseForFileToSave (true))
                editFile = fc.getResult();
            else
                return;
        }
        
        selectionManager->deselectAll();
		edit->editFileRetriever = [editFile] { return editFile; };
		edit->playInStopEnabled = true;

		te::EditFileOperations(*edit).save(true, true, false);
}




