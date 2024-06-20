#include "SoloudAPI.h"
#include "SoloudAudioSource.h"
#include "soloud.h"
#include "soloud_wav.h"
#include <vector>

using namespace SoLoud;

std::shared_ptr<Soloud> soloud_instance;
std::vector<SoloudAudioSource*> session_loaded_audios;

SoloudInstance::SoloudInstance()
{
	soloud_instance = std::make_shared<Soloud>(Soloud());
}

SoloudInstance::~SoloudInstance()
{
	if (soloud_instance != nullptr) 
	{
		soloud_instance->stopAll();
		soloud_instance->deinit();
		soloud_instance.reset();
		soloud_instance = nullptr;
	}
}

int SoloudInstance::Init(unsigned int flags, unsigned int backend, unsigned int sample_rate, unsigned int buffer_size, unsigned int channels)
{
	if (soloud_instance != nullptr)
	{
		int result = soloud_instance->init(flags | Soloud::LEFT_HANDED_3D, backend, sample_rate, buffer_size, channels);

		if (result == 0)
		{
			return result;
		}
	}
	return -1;
}

unsigned int SoloudInstance::SetMaxActiveVoiceCount(unsigned int voices_count)
{
	if(soloud_instance == nullptr)
		return 0;

	return soloud_instance->setMaxActiveVoiceCount(voices_count);
}

unsigned int SoloudInstance::Play(AudioEngineSource * source, float volume, float pan, bool paused, unsigned int bus)
{
	if (soloud_instance == nullptr || source == nullptr)
		return 0;

	//SoLoud::Wav* new_sound = new SoLoud::Wav(((SoloudAudioSource*)source)->sound);

	unsigned int result;
	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		result = soloud_instance->play(((SoloudAudioBus*)source)->bus, volume, pan, paused);
		((SoloudAudioBus*)source)->Play();
	}
	else
	{
		result = soloud_instance->play(((SoloudAudioSource*)source)->sound, volume, pan, paused, bus);
	}
	source->play_handle = result;
	return result;

	//return soloud_instance->play(((SoloudAudioSource*)source)->sound, volume, pan, paused, bus);
}

unsigned int SoloudInstance::PlayClocked(double time, AudioEngineSource * source, float volume, float pan, unsigned int bus)
{
	if (soloud_instance == nullptr || source == nullptr)
		return 0;

	unsigned int result;
	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		result = soloud_instance->playClocked(time, ((SoloudAudioBus*)source)->bus, volume, pan);
		((SoloudAudioBus*)source)->PlayClocked(time);
	}
	else
	{
		result = soloud_instance->playClocked(time, ((SoloudAudioSource*)source)->sound, volume, pan, bus);
	}
	source->play_handle = result;
	return result;

	//return soloud_instance->playClocked(time, ((SoloudAudioSource*)source)->sound, volume, pan, bus);
}

unsigned int SoloudInstance::Play3d(AudioEngineSource * source, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ, float volume, bool paused, unsigned int bus)
{
	if (soloud_instance == nullptr || source == nullptr)
		return 0;

	unsigned int result;
	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		result = soloud_instance->play3d(((SoloudAudioBus*)source)->bus, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume, paused);
		((SoloudAudioBus*)source)->Play3d(positionX, positionY, positionZ, velocityX, velocityY, velocityZ);
	}
	else
	{
		result = soloud_instance->play3d(((SoloudAudioSource*)source)->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume, paused, bus);
	}
	source->play_handle = result;
	return result;

	//return soloud_instance->play3d(((SoloudAudioSource*)source)->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume, paused, bus);
}

unsigned int SoloudInstance::Play3dClocked(double time, AudioEngineSource * source, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ, float volume, unsigned int bus)
{
	if (soloud_instance == nullptr || source == nullptr)
		return 0;

	unsigned int result;
	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		result = soloud_instance->play3dClocked(time, ((SoloudAudioBus*)source)->bus, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume);
		((SoloudAudioBus*)source)->Play3dClocked(time, positionX, positionY, positionZ, velocityX, velocityY, velocityZ);
	}
	else
	{
		result = soloud_instance->play3dClocked(time, ((SoloudAudioSource*)source)->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume, bus);
	}
	source->play_handle = result;
	return result;

	//return soloud_instance->play3dClocked(time, ((SoloudAudioSource*)source)->sound, positionX, positionY, positionZ, velocityX, velocityY, velocityZ, volume, bus);
}

unsigned int SoloudInstance::PlayBackground(AudioEngineSource * source, float volume, bool paused, unsigned int bus)
{
	if (soloud_instance == nullptr || source == nullptr)
		return 0;

	unsigned int result;
	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		result = soloud_instance->playBackground(((SoloudAudioBus*)source)->bus, volume, paused, bus);
		((SoloudAudioBus*)source)->Play();
	}
	else
	{
		result = soloud_instance->playBackground(((SoloudAudioSource*)source)->sound, volume, paused, bus);
	}
	source->play_handle = result;
	return result;

	//return soloud_instance->playBackground(((SoloudAudioSource*)source)->sound, volume, paused, bus);
}

unsigned int SoloudInstance::Seek(int play_handle, double seconds)
{
	if (soloud_instance == nullptr)
		return 0;

	return soloud_instance->seek(play_handle, seconds);
}

void SoloudInstance::Stop(int play_handle)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->stop(play_handle);
}

void SoloudInstance::StopAll()
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->stopAll();
}

void SoloudInstance::StopAudioSource(AudioEngineSource * source)
{
	if (soloud_instance == nullptr || source == nullptr)
		return;

	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		soloud_instance->stopAudioSource(((SoloudAudioBus*)source)->bus);
	}
	else
	{
		soloud_instance->stopAudioSource(((SoloudAudioSource*)source)->sound);
	}
	//soloud_instance->stopAudioSource(((SoloudAudioSource*)source)->sound);
}

void SoloudInstance::SetVolume(AudioEngineSource* source, float volume)
{
	if (soloud_instance == nullptr || source == nullptr)
		return;
	
	source->SetVolume(volume);
	soloud_instance->setVolume(source->play_handle, volume);
}

void SoloudInstance::SetPause(int play_handle, bool paused)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->setPause(play_handle, paused);
}

void SoloudInstance::SetPauseAll(bool paused)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->setPauseAll(paused);
}

bool SoloudInstance::IsPaused(int play_handle)
{
	if (soloud_instance == nullptr)
		return false;

	return soloud_instance->getPause(play_handle);
}

bool SoloudInstance::IsFinished(AudioEngineSource* source)
{
	if (soloud_instance == nullptr || source == nullptr)
		return false;

	if (dynamic_cast<SoloudAudioBus*>(source))
	{
		//soloud_instance->stopAudioSource(((SoloudAudioBus*)source)->bus);
		std::vector<SoLoud::Wav*> bus_content = ((SoloudAudioBus*)source)->GetContent();

		for (size_t x = 0; x < bus_content.size(); x++) 
		{
			if (!soloud_instance->countAudioSource(*bus_content[x]) == 0) 
			{
				return false;
			}
		}
		return true;


		//return (soloud_instance->countAudioSource(((SoloudAudioBus*)source)->bus) == 0);
	}
	else
	{
		return (soloud_instance->countAudioSource(((SoloudAudioSource*)source)->sound) == 0);
	}

	//return (soloud_instance->countAudioSource(((SoloudAudioSource*)source)->sound) == 0);
}

void SoloudInstance::Set3dListenerParameters(float positionX, float positionY, float positionZ, float vectorLookAtX, float vectorLookAtY, float vectorLookAtZ, float vectorUpX, float vectorUpY, float vectorUpZ, float velocityX, float velocityY, float velocityZ)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->set3dListenerParameters(positionX, positionY, positionZ, vectorLookAtX, vectorLookAtY, vectorLookAtZ, vectorUpX, vectorUpY, vectorUpZ, velocityX, velocityY, velocityZ);
}

void SoloudInstance::Set3dSourceParameters(int play_handle, float positionX, float positionY, float positionZ, float velocityX, float velocityY, float velocityZ)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->set3dSourceParameters(play_handle, positionX, positionY, positionZ, velocityX, velocityY, velocityZ);
}

void SoloudInstance::Update3dAudio()
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->update3dAudio();
}


AudioEngineSource* SoloudInstance::GetWavNewInstance(int audio_id)
{
	if (soloud_instance == nullptr)
		return nullptr;
	
	for (size_t x = 0; x < session_loaded_audios.size(); x++) 
	{
		if (session_loaded_audios[x]->id == audio_id) 
		{
			return new SoloudAudioSource(*session_loaded_audios[x]);
		}
	}

	return nullptr;
}

AudioEngineSource* SoloudInstance::GetBusNewInstance(std::vector<float>* volumes)
{
	if (soloud_instance == nullptr)
		return nullptr;

	return new SoloudAudioBus(volumes);
}

bool SoloudInstance::AudioIsLoaded(int audio_id)
{
	if (soloud_instance == nullptr)
		return false;

	for (size_t x = 0; x < session_loaded_audios.size(); x++)
	{
		if (session_loaded_audios[x]->id == audio_id)
		{
			return true;
		}
	}
	return false;
}

AudioEngineSource* SoloudInstance::GetLoadedAudio(int audio_id)
{
	if (soloud_instance == nullptr)
		return nullptr;

	for (size_t x = 0; x < session_loaded_audios.size(); x++)
	{
		if (session_loaded_audios[x]->id == audio_id)
		{
			return session_loaded_audios[x];
		}
	}
	return nullptr;
}

bool SoloudInstance::UnloadAudio(int id)
{
	if (soloud_instance == nullptr)
		return false;

	for (size_t x = 0; x < session_loaded_audios.size(); x++)
	{
		if (session_loaded_audios[x]->id == id)
		{
			session_loaded_audios.erase(session_loaded_audios.begin() + x);
			return true;
			//break;
		}
	}
	return false;
}

void SoloudInstance::UnloadAllAudios()
{
	if (soloud_instance == nullptr)
		return;

	for (size_t y = 0; y < session_loaded_audios.size(); y++)
	{
		if (session_loaded_audios[y] != nullptr && session_loaded_audios[y]->owners.size() <= 0)
		{
			delete session_loaded_audios[y];
			session_loaded_audios.erase(session_loaded_audios.begin() + y);
		}
	}
}

bool SoloudInstance::RegisterTagToLoadedAudio(int id, std::string tag_name)
{
	if (soloud_instance == nullptr)
		return false;

	for (size_t x = 0; x < session_loaded_audios.size(); x++)
	{
		if (session_loaded_audios[x]->id == id)
		{
			for (size_t y = 0; y < session_loaded_audios[x]->owners.size(); y++)
			{
				if (session_loaded_audios[x]->owners[y] == tag_name)
				{
					//already registered
					return false;
				}
			}
			session_loaded_audios[x]->owners.push_back(tag_name);
			return true;
		}
	}
	return false;
}

bool SoloudInstance::RemoveTagToLoadedAudio(int id, std::string tag_name)
{
	if (soloud_instance == nullptr)
		return false;

	for (size_t x = 0; x < session_loaded_audios.size(); x++)
	{
		if (session_loaded_audios[x]->id == id)
		{
			for (size_t y = 0; y < session_loaded_audios[x]->owners.size(); y++)
			{
				if (session_loaded_audios[x]->owners[y] == tag_name)
				{
					session_loaded_audios[x]->owners.erase(session_loaded_audios[x]->owners.begin() + y);
					return true;
				}
			}
			return false;
		}
	}
	return false;
}

AudioEngineSource* SoloudInstance::LoadAudioFromBuffer(int id, char * buffer, long size)
{
	if (soloud_instance == nullptr)
		return nullptr;

	if (!AudioIsLoaded(id))
	{
		SoloudAudioSource* new_source = new SoloudAudioSource();
		new_source->id = id;

		unsigned int result = new_source->sound.loadMem(reinterpret_cast<unsigned char*>(buffer), size);
		
		if (result > 0)
			return nullptr;

		session_loaded_audios.push_back(new_source);
		return new_source;
	}
	return nullptr;
}

AudioEngineSource* SoloudInstance::LoadAudioFromDisk(int id, std::string filename)
{
	if (soloud_instance == nullptr)
		return nullptr;

	if (!AudioIsLoaded(id))
	{
		SoloudAudioSource* new_source = new SoloudAudioSource();
		new_source->id = id;
		
		unsigned int result = new_source->sound.load(filename.c_str());
		
		if (result > 0)
			return nullptr;

		session_loaded_audios.push_back(new_source);

		return new_source;
	}
	return nullptr;
}

KrilloudClass* SoloudInstance::InstanceFilter(std::vector<float> initial_parameters, KrilloudClass::KrilloudFilterType filter_type)
{
	return new SoloudFilter(initial_parameters, filter_type, soloud_instance);
}

void SoloudInstance::SetFilterParameter(int play_handle, unsigned int filter_id, unsigned int attribute_id, float value)
{
	if (soloud_instance == nullptr)
		return;

	soloud_instance->setFilterParameter(play_handle, filter_id, attribute_id, value);
}