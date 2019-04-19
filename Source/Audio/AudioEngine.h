#pragma once

#include "JuceHeader.h"
#include "FlaggedAsyncUpdater.h"



namespace te = tracktion_engine;

class AudioEngine
{
public:

	AudioEngine();
	~AudioEngine();

	te::Project::Ptr createTempProject(te::Engine& engine);

	

	void browseForAudioFile(te::Engine& engine, std::function<void(const File&)> fileChosenCallback);
	void removeAllClips(te::AudioTrack& track);
	te::AudioTrack* getOrInsertAudioTrackAt(te::Edit& edit, int index);
	te::WaveAudioClip::Ptr loadAudioFileAsClip(te::Edit& edit, const File& file);

	template<typename ClipType>
	typename ClipType::Ptr loopAroundClip(ClipType& clip);

	void togglePlay();
	void toggleRecord();
	void saveFile();
	void addTrack();
	void deleteAllTracks();
	void deleteClipOrTrack();
	void createTracksAndAssignInputs();
	void showAudioDeviceSettings();
	void createNewFile(File editFile = {});

	void armTrack(te::AudioTrack& t, bool arm, int position = 0);
	void enableInputMonitoring(te::AudioTrack& t, bool im, int position = 0);

	bool wasEditCreated();
	bool isTrackArmed(te::AudioTrack& t, int position = 0);
	bool isInputMonitoringEnabled(te::AudioTrack& t, int position = 0);
	bool trackHasInput(te::AudioTrack& t, int position = 0);

	te::SelectionManager* getSelectionManager();
	te::SmartThumbnail* getSmartThumbnail();
	te::LambdaTimer getCursorUpdater();
	te::TransportControl& getTransport();
	te::Edit* getEdit();

	ValueTree getChildWithName();
	UndoManager* getUndoManager();

	

	
private:
	
	te::Engine engine{ ProjectInfo::projectName };
	std::unique_ptr<te::Edit> edit;
	
	std::unique_ptr<te::SmartThumbnail> smartThumbnail;
	te::LambdaTimer cursorUpdater;

	std::unique_ptr <FlaggedAsyncUpdater> flaggedAsyncUpdater;
	
	std::unique_ptr <te::SelectionManager> selectionManager;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};

