#ifndef AUDIO_ENGINE_FILTER_H
#define AUDIO_ENGINE_FILTER_H
#include "KrilloudClasses.h"

class AudioEngine;

class AudioEngineFilter : public KrilloudClass
{
protected:
	//Filter parameters
	std::vector<float> parameters;
public:
	//Filter parameters control

	// Add a new parameter
	int AddParameter(float value);

	// Remove a paremeter
	bool RemoveParameter(int position);

	// Set parameter's new info
	bool SetParameter(int position, float parameter_value);

	// Set the whole list of parameters
	bool SetParameters(std::vector<float> new_parameters);

	// Get the list of current parameters
	std::vector<float> GetParameters();

	//Real-time value updating
	virtual void UpdatePlayingValues(AudioEngine* engine, int gBusHandle) = 0;

	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo() = 0;
};
#endif // AUDIO_ENGINE_FILTER_H