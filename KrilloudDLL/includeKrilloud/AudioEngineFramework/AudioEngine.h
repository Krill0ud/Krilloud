#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H
#include "AudioEngineSource.h"
#include "AudioEngineFilter.h"

class AudioEngineSource;

class AudioEngine 
{
public:
	// --- Configuration ---------------------------------------------------------------------------------------------------------------------------------

	// Initializes the Audio engine
	virtual int Init(unsigned int flags, unsigned int backend, unsigned int sample_rate, unsigned int buffer_size, unsigned int channels) = 0;

	// Set current maximum active voice setting
	virtual unsigned int SetMaxActiveVoiceCount(unsigned int voices_count) = 0;

	// --- Sound reproduction ----------------------------------------------------------------------------------------------------------------------------
	
	// Start playing a sound.
	virtual unsigned int Play(AudioEngineSource* source, float volume = 1, float pan = 0.0f, bool paused = 0, unsigned int bus = 0) = 0;
	
	// Start playing a sound delayed in relation to other sounds called via this function.
	virtual unsigned int PlayClocked(double time, AudioEngineSource* source, float volume = 1, float pan = 0.0f, unsigned int bus = 0) = 0;
	
	// Start playing a 3d audio source
	virtual unsigned int Play3d(AudioEngineSource* source, float positionX, float positionY, float positionZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f, float volume = 1, bool paused = 0, unsigned int bus = 0) = 0;
	
	// Start playing a 3d audio source, delayed in relation to other sounds called via this function.
	virtual unsigned int Play3dClocked(double time, AudioEngineSource* source, float positionX, float positionY, float positionZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f, float volume = 1, unsigned int bus = 0) = 0;
	
	// Start playing a sound without any panning.
	virtual unsigned int PlayBackground(AudioEngineSource* source, float volume = 1, bool paused = 0, unsigned int bus = 0) = 0;

	// Seek the audio stream to certain point in time.
	virtual unsigned int Seek(int play_handle, double seconds) = 0;

	// Stop the playback
	virtual void Stop(int play_handle) = 0;
	
	// Stop all voices.
	virtual void StopAll() = 0;

	// Stop all voices that play this sound source
	virtual void StopAudioSource(AudioEngineSource* source) = 0;

	// Set playback's volume
	virtual void SetVolume(AudioEngineSource* source, float volume) = 0;

	// Set the pause state
	virtual void SetPause(int play_handle, bool paused) = 0;
	
	// Pause all voices
	virtual void SetPauseAll(bool paused) = 0;

	// Get the current pause state of the playback
	virtual bool IsPaused(int play_handle) = 0;

	// Get whether the playback is finished or not
	virtual bool IsFinished(AudioEngineSource* source) = 0;

	// --- 3D functions ----------------------------------------------------------------------------------------------------------------------------------
	
	// Set 3d listener parameters
	virtual void Set3dListenerParameters(float positionX, float positionY, float positionZ, float vectorLookAtX, float vectorLookAtY, float vectorLookAtZ, 
										 float vectorUpX, float vectorUpY, float vectorUpZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f) = 0;

	// Set 3D object parameters
	virtual void Set3dSourceParameters(int play_handle, float positionX, float positionY, float positionZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f) = 0;
	
	// Force 3D audio updates
	virtual void Update3dAudio() = 0;

	// --- Audio load ------------------------------------------------------------------------------------------------------------------------------------
	
	// Gets a new instance of an already loaded audio
	virtual AudioEngineSource* GetWavNewInstance(int audio_id) = 0;

	// Gets a new instance of a bus
	virtual AudioEngineSource* GetBusNewInstance(std::vector<float>* volumes) = 0;

	// Get whether an audio is loaded or not
	virtual bool AudioIsLoaded(int audio_id) = 0;

	// Get a loaded audio 
	virtual AudioEngineSource* GetLoadedAudio(int audio_id) = 0;

	// Unload an audio
	virtual bool UnloadAudio(int audio_id) = 0;

	// Unload all audios
	virtual void UnloadAllAudios() = 0;

	// Add a new owner tag to a loaded audio
	virtual bool RegisterTagToLoadedAudio(int audio_id, std::string tag_name) = 0;

	// Removes an owner tag to a loaded audio
	virtual bool RemoveTagToLoadedAudio(int audio_id, std::string tag_name) = 0;

	// Load an audio from a buffer
	virtual AudioEngineSource* LoadAudioFromBuffer(int audio_id, char* buffer, long size) = 0;

	// Load an audio from path
	virtual AudioEngineSource* LoadAudioFromDisk(int audio_id, std::string filename) = 0;

	// --- Filters functions -----------------------------------------------------------------------------------------------------------------------------

	// Create a new instance of a filter
	virtual KrilloudClass* InstanceFilter(std::vector<float> initial_parameters, KrilloudClass::KrilloudFilterType filter_type) = 0;

	// Set a live filter parameter. Use 0 for the global filters.
	virtual void SetFilterParameter(int play_handle, unsigned int filter_id, unsigned int attribute_id, float value) = 0;
};
#endif //AUDIO_ENGINE_H