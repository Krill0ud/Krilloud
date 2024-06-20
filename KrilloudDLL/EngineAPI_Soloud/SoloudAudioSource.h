#pragma once
#include "AudioEngineSource.h"
#include "soloud_wav.h"
#include "soloud_bus.h"
#include "SoloudFilter.h"

class SoloudAudioSource : public AudioEngineWav
{
public:
	SoLoud::Wav sound;

	SoloudAudioSource();
	~SoloudAudioSource();

	// Set default volume for instances
	virtual void SetVolume(float volume);

	// Set the looping of the instances created from this audio source
	virtual void SetLooping(bool loop);

	// Set whether only one instance of this sound should ever be playing at the same time
	virtual void SetSingleInstance(bool single_instance);

	// Set the minimum and maximum distances for 3d audio source (closer to min distance = max vol)
	virtual void Set3dMinMaxDistance(float distance_min, float distance_max);

	// Set attenuation model and rolloff factor for 3d audio source
	virtual void Set3dAttenuation(unsigned int attenuation, float roll_off_factor);

	// Set doppler factor to reduce or enhance doppler effect, default = 1.0
	virtual void Set3dDopplerFactor(float doppler_factor);

	// Enable delaying the start of the sound based on the distance.
	virtual void Set3dDistanceDelay(bool delay);

	// Set filter to an AudioSource. Set to NULL to clear the filter.
	virtual void SetFilter(unsigned int filter_id, AudioEngineFilter* filter);

	// Updates all filters contained in the AudioSource.
	virtual void UpdateFilters(AudioEngine* engine);
};

class SoloudAudioBus : public AudioEngineBus
{
public:
	SoLoud::Bus bus;
	std::vector<float>* volume_list;

	SoloudAudioBus(std::vector<float>* volumes);
	~SoloudAudioBus();

	// Set default volume for instances
	virtual void SetVolume(float volume);

	// Set the looping of the instances created from this audio source
	virtual void SetLooping(bool loop);

	// Set whether only one instance of this sound should ever be playing at the same time
	virtual void SetSingleInstance(bool single_instance);

	// Set the minimum and maximum distances for 3d audio source (closer to min distance = max vol)
	virtual void Set3dMinMaxDistance(float distance_min, float distance_max);

	// Set attenuation model and rolloff factor for 3d audio source
	virtual void Set3dAttenuation(unsigned int attenuation, float roll_off_factor);

	// Set doppler factor to reduce or enhance doppler effect, default = 1.0
	virtual void Set3dDopplerFactor(float doppler_factor);

	// Enable delaying the start of the sound based on the distance.
	virtual void Set3dDistanceDelay(bool delay);

	// Set filter to an AudioSource. Set to NULL to clear the filter.
	virtual void SetFilter(unsigned int filter_id, AudioEngineFilter* filter);

	// Updates all filters contained in the AudioSource.
	virtual void UpdateFilters(AudioEngine* engine);

	// Add a new AudioSource to the bus
	virtual void AddAudioSource(AudioEngineSource* source);

	// Start playing a sound.
	virtual void Play();

	// Start playing a sound delayed in relation to other sounds called via this function. Negative volume means to use default.
	virtual void PlayClocked(double time);

	// Start playing a 3d audio source
	virtual void Play3d(float positionX, float positionY, float positionZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f);

	// Start playing a 3d audio source, delayed in relation to other sounds called via this function.
	virtual void Play3dClocked(double time, float positionX, float positionY, float positionZ, float velocityX = 0.0f, float velocityY = 0.0f, float velocityZ = 0.0f);

	// Returns the list of wavs within the bus
	virtual std::vector<SoLoud::Wav*> GetContent();
};