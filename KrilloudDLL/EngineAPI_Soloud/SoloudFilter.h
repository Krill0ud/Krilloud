#pragma once
#include "AudioEngineFilter.h"
#include "soloud.h"

class SoloudFilter : public AudioEngineFilter
{
	std::shared_ptr<SoLoud::Soloud> soloud_instance;

	//Soloud filter instance
	SoLoud::Filter* this_filter;

	KrilloudFilterType this_filter_type;
	int my_instance_id;
	bool init_error = false;
public:
	SoloudFilter(std::vector<float> initial_parameters, KrilloudFilterType filter_type, std::shared_ptr<SoLoud::Soloud> engine );
	~SoloudFilter();

	SoLoud::Filter* GetFilter();

	//Real-time value updating
	virtual void UpdatePlayingValues(AudioEngine* engine, int gBusHandle);

	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo();
};