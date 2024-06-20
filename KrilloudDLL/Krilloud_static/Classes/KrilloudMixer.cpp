#include "KrilloudClasses.h"
#include "AudioEngine.h"

namespace KrilloudEngine 
{
	extern AudioEngine* engine_instance;
}

KrilloudMixer::KrilloudMixer()
{
	
}

KrilloudMixer::~KrilloudMixer()
{

}

int KrilloudMixer::CreateNewSocket(int ID, SocketType type, SocketDataType data)
{
	int id = KrilloudClass::CreateNewSocket(ID, type, data);

	if (type == SocketType::Input && data == SocketDataType::Variable)
	{
		volumes.push_back(1);
	}

	return id;
}

bool KrilloudMixer::RemoveSocket(int id)
{
	if (id < (int)myKLinks.size())
	{
		if (myKLinks[id].data_type == SocketDataType::Variable)
		{
			volumes.erase(volumes.begin() + (int)(id / 2));
		}
		myKLinks.erase(myKLinks.begin() + id);
		return true;
	}
	return false;
}

std::vector<float> KrilloudMixer::GetVolumesList()
{
	return volumes;
}

void KrilloudMixer::SetVolume(int selection, float volume)
{
	if (selection < volumes.size())
	{
		volumes[selection] = std::fmaxf(0, std::fminf(1, volume));
	}
}

void KrilloudMixer::UpdateVolumeValues()
{
	int variable_index = -1;

	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Variable)
		{
			variable_index++;
			if (myKLinks[x].connections.size() > 0)
			{
				KrilloudVar* variable_node = dynamic_cast<KrilloudVar*>(myKLinks[x].connections[0]);

				if (variable_node != nullptr)
				{
					//Update parameters from connected variables
					volumes[variable_index] = std::fmin(1, std::fmax(0, variable_node->GetVariableValue()));
				}
			}
		}
	}
}

TagInfo* KrilloudMixer::GetInfo()
{
	TagInfo* result = new TagInfo();
	result->source = KrilloudEngine::engine_instance->GetBusNewInstance(&volumes);
	
	//Get normal input sockets that can have something connected to them
	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Normal)
		{
			if (myKLinks[x].connections.size() > 0)
			{
				TagInfo* linkInfo = myKLinks[x].connections[0]->GetInfo();

				if (linkInfo != nullptr && linkInfo->source != nullptr) 
				{
					((AudioEngineBus*)result->source)->AddAudioSource(linkInfo->source);

					for (size_t y = 0; y < linkInfo->mixers.size(); y++)
					{
						result->mixers.push_back(linkInfo->mixers[y]);
					}
					result->mixers.push_back(this);

					for (size_t y = 0; y < linkInfo->kvars.size(); y++)
					{
						result->kvars.push_back(linkInfo->kvars[y]);
					}
				}
				else
				{
					// Add a null source to "use" the volume index x
					((AudioEngineBus*)result->source)->AddAudioSource(nullptr);
				}
			}
			else
			{
				// Add a null source to "use" the volume index x
				((AudioEngineBus*)result->source)->AddAudioSource(nullptr);
			}
		}
	}

	return result;
}