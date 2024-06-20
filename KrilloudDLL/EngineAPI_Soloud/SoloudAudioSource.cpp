#include "SoloudAudioSource.h"

extern std::shared_ptr<SoLoud::Soloud> soloud_instance;

SoloudAudioSource::SoloudAudioSource()
{
}

SoloudAudioSource::~SoloudAudioSource()
{
}

void SoloudAudioSource::SetVolume(float volume)
{
	//this->volume = volume;
	//sound.setVolume(volume);
}

void SoloudAudioSource::SetLooping(bool loop)
{
	sound.setLooping(loop);
}

void SoloudAudioSource::SetSingleInstance(bool single_instance)
{
	sound.setSingleInstance(single_instance);
}

void SoloudAudioSource::Set3dMinMaxDistance(float distance_min, float distance_max)
{
	sound.set3dMinMaxDistance(distance_min, distance_max);
}

void SoloudAudioSource::Set3dAttenuation(unsigned int attenuation, float roll_off_factor)
{
	sound.set3dAttenuation(attenuation, roll_off_factor);
}

void SoloudAudioSource::Set3dDopplerFactor(float doppler_factor)
{
	sound.set3dDopplerFactor(doppler_factor);
}

void SoloudAudioSource::Set3dDistanceDelay(bool delay)
{
	sound.set3dDistanceDelay(delay);
}

void SoloudAudioSource::SetFilter(unsigned int filter_id, AudioEngineFilter * filter)
{
	if (filter == nullptr)
		return;
	
	SoLoud::Filter* soloud_filter = ((SoloudFilter*)filter)->GetFilter();
	
	if (soloud_filter == nullptr)
		return;
	
	sound.setFilter(filter_id, soloud_filter);
}

void SoloudAudioSource::UpdateFilters(AudioEngine* engine)
{
	for (size_t x = 0; x < filters.size(); x++)
	{
		filters[x]->UpdatePlayingValues(engine, play_handle);
	}
}

// ------------------------------------------------------

SoloudAudioBus::SoloudAudioBus(std::vector<float>* volumes)
{
	volume_list = volumes;
	volume = 1;
}

SoloudAudioBus::~SoloudAudioBus()
{
}

void SoloudAudioBus::SetVolume(float volume)
{
	//bus.setVolume(volume);
	if (volume_list != nullptr) 
	{
		for (size_t x = 0; x < content.size(); x++)
		{
			if (content[x] != nullptr) 
			{
				soloud_instance->setVolume(content[x]->play_handle, (*volume_list)[x]);
				if (dynamic_cast<SoloudAudioBus*>(content[x]))
				{
					content[x]->SetVolume(volume);
				}
			}
		}
	}
}

void SoloudAudioBus::SetLooping(bool loop)
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr)
		{
			content[x]->SetLooping(loop);
		}
	}
}

void SoloudAudioBus::SetSingleInstance(bool single_instance)
{
	bus.setSingleInstance(single_instance);
}

void SoloudAudioBus::Set3dMinMaxDistance(float distance_min, float distance_max)
{
	bus.set3dMinMaxDistance(distance_min, distance_max);
}

void SoloudAudioBus::Set3dAttenuation(unsigned int attenuation, float roll_off_factor)
{
	bus.set3dAttenuation(attenuation, roll_off_factor);
}

void SoloudAudioBus::Set3dDopplerFactor(float doppler_factor)
{
	bus.set3dDopplerFactor(doppler_factor);
}

void SoloudAudioBus::Set3dDistanceDelay(bool delay)
{
	bus.set3dDistanceDelay(delay);
}

void SoloudAudioBus::SetFilter(unsigned int filter_id, AudioEngineFilter * filter)
{
	if (filter == nullptr)
		return;

	SoLoud::Filter* soloud_filter = ((SoloudFilter*)filter)->GetFilter();

	if (soloud_filter == nullptr)
		return;

	bus.setFilter(filter_id, soloud_filter);
}

void SoloudAudioBus::UpdateFilters(AudioEngine* engine)
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr) 
		{
			content[x]->UpdateFilters(engine);
		}
	}

	for (size_t x = 0; x < filters.size(); x++) 
	{
		filters[x]->UpdatePlayingValues(engine, play_handle);
	}
}

void SoloudAudioBus::AddAudioSource(AudioEngineSource* source)
{
	/*if (source != nullptr) 
	{
		
	}*/
	// Allow sources to be null, so each socket can use its proper audio volume index.
	content.push_back(source);
}

void SoloudAudioBus::Play()
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr)
		{
			if (dynamic_cast<SoloudAudioBus*>(content[x]))
			{
				content[x]->play_handle = bus.play(((SoloudAudioBus*)content[x])->bus, (*volume_list)[x]);
				((SoloudAudioBus*)content[x])->Play();
			}
			else
			{
				content[x]->play_handle = bus.play(((SoloudAudioSource*)content[x])->sound, (*volume_list)[x]);
			}
		}
	}
}

void SoloudAudioBus::PlayClocked(double time)
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr)
		{
			if (dynamic_cast<SoloudAudioBus*>(content[x]))
			{
				content[x]->play_handle = bus.playClocked(time, ((SoloudAudioBus*)content[x])->bus, (*volume_list)[x]);
				((SoloudAudioBus*)content[x])->PlayClocked(time);
			}
			else
			{
				content[x]->play_handle = bus.playClocked(time, ((SoloudAudioSource*)content[x])->sound, (*volume_list)[x]);
			}
		}
	}
}

void SoloudAudioBus::Play3d(float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ)
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr)
		{
			if (dynamic_cast<SoloudAudioBus*>(content[x]))
			{
				content[x]->play_handle = bus.play3d(((SoloudAudioBus*)content[x])->bus, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, (*volume_list)[x]);
				((SoloudAudioBus*)content[x])->Play3d(positionX, positionY, positionZ, velocityX, velocityY, velocityZ);
			}
			else
			{
				content[x]->play_handle = bus.play3d(((SoloudAudioSource*)content[x])->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, (*volume_list)[x]);
			}
		}
	}
}

void SoloudAudioBus::Play3dClocked(double time, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ)
{
	for (size_t x = 0; x < content.size(); x++)
	{
		if (content[x] != nullptr)
		{
			if (dynamic_cast<SoloudAudioBus*>(content[x]))
			{
				content[x]->play_handle = bus.play3dClocked(time, ((SoloudAudioBus*)content[x])->bus, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, (*volume_list)[x]);
				((SoloudAudioBus*)content[x])->Play3dClocked(time, positionX, positionY, positionZ, velocityX, velocityY, velocityZ);
			}
			else
			{
				content[x]->play_handle = bus.play3dClocked(time, ((SoloudAudioSource*)content[x])->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, (*volume_list)[x]);
			}
		}
	}
}

std::vector<SoLoud::Wav*> SoloudAudioBus::GetContent()
{
	std::vector<SoLoud::Wav*> result;

	for (size_t x = 0; x < content.size(); x++) 
	{
		if (content[x] != nullptr) 
		{
			if (dynamic_cast<SoloudAudioBus*>(content[x]))
			{
				std::vector<SoLoud::Wav*> bus_content = ((SoloudAudioBus*)content[x])->GetContent();

				for (size_t y = 0; y < bus_content.size(); y++) 
				{
					result.push_back(bus_content[y]);
				}
			}
			else
			{
				result.push_back(&((SoloudAudioSource*)content[x])->sound);
			}
		}
	}

	return result;
}