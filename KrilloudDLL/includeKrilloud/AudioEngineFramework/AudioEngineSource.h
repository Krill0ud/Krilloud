#ifndef AUDIO_ENGINE_SOURCE_H
#define AUDIO_ENGINE_SOURCE_H
#include <vector>
#include <string>

class AudioEngineFilter;
class AudioEngine;

class AudioEngineSource
{
public:
	int id;
	int play_handle;
	float volume;
	std::vector<AudioEngineFilter*> filters;

	virtual ~AudioEngineSource() {};

	// Set default volume for instances
	virtual void SetVolume(float volume) = 0;

	// Set the looping of the instances created from this audio source
	virtual void SetLooping(bool loop) = 0;

	// Set whether only one instance of this sound should ever be playing at the same time
	virtual void SetSingleInstance(bool single_instance) = 0;

	// Set the minimum and maximum distances for 3d audio source (closer to min distance = max vol)
	virtual void Set3dMinMaxDistance(float distance_min, float distance_max) = 0;

	// Set attenuation model and rolloff factor for 3d audio source
	virtual void Set3dAttenuation(unsigned int attenuation, float roll_off_factor) = 0;

	// Set doppler factor to reduce or enhance doppler effect, default = 1.0
	virtual void Set3dDopplerFactor(float doppler_factor) = 0;

	// Enable delaying the start of the sound based on the distance.
	virtual void Set3dDistanceDelay(bool delay) = 0;

	// Set filter to an AudioSource. Set to NULL to clear the filter.
	virtual void SetFilter(unsigned int filter_id, AudioEngineFilter* filter) = 0;

	// Updates all filters contained in the AudioSource.
	virtual void UpdateFilters(AudioEngine* engine) = 0;
};

class AudioEngineWav : public AudioEngineSource
{
public:
	std::vector<std::string> owners;
};

class AudioEngineBus : public AudioEngineSource
{
public:
	std::vector<AudioEngineSource*> content;

	// Add a new AudioSource to the bus
	virtual void AddAudioSource(AudioEngineSource* source) = 0;
};
#endif //AUDIO_ENGINE_SOURCE_H