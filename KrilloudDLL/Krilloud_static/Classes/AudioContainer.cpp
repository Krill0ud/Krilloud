//#include "AudioContainer.h"
#include "KrilloudClasses.h"
#include <random>
#include "AudioEngine.h"

AudioContainer::AudioContainer(std::vector<LoadedSound*>* loaded_audios_list, AudioEngine* engine)
{
	audios_list_pointer = loaded_audios_list;
	//AddSoundID(sound_ids_list);
	engine_instance = engine;
	myTagInfo = new TagInfo();

	loopable_audio = false;
}

AudioContainer::~AudioContainer()
{
}

TagInfo* AudioContainer::GetInfo() 
{
	myTagInfo = new TagInfo();
	
	if (audios_list_pointer != nullptr && content_list.size() > 0)
	{
		if (selection_mode == 0) //Random Mode
		{
			std::random_device seed;
			std::mt19937 engine(seed());
			std::uniform_int_distribution<int> choose(0, (int)content_list.size() - 1);

			size_t selection = choose(engine);

			if (last_selection_from_list != selection) 
			{
				last_selection_from_list = selection;
			}
			else
			{
				last_selection_from_list = (++last_selection_from_list) % content_list.size();
			}
		}
		else if (selection_mode == 1) //Sequential Mode
		{
			last_selection_from_list = (++last_selection_from_list) % content_list.size();
		}
		else
		{
			last_selection_from_list = 0;
		}

		if (last_selection_from_list < content_list.size())
		{
			int selected_id = content_list[last_selection_from_list]->id;

			size_t index = 0;
			bool found = false;

			do 
			{
				if ((*audios_list_pointer)[index]->id == selected_id)
				{
					//myTagInfo->wave = new SoLoud::Wav((*audios_list_pointer)[index]->sound);
					
					myTagInfo->source = engine_instance->GetWavNewInstance(selected_id);
					
					myTagInfo->source->SetLooping(loopable_audio);

					myTagInfo->source->volume = content_list[last_selection_from_list]->volume;
					//myTagInfo->source->id = (*audios_list_pointer)[index]->id;
					found = true;
				}
				index++;
			} while (!found && index < audios_list_pointer->size());
		}
		return myTagInfo;
	}

	return nullptr;
}

std::vector<AudioSourceContent*>* AudioContainer::GetContainedIDs()
{
	return &content_list;
}

void AudioContainer::ChangeSelection(int new_mode) 
{
	if (new_mode >= 0 && new_mode <= 1) 
	{
		selection_mode = new_mode;
	}
}

void AudioContainer::SetLoop(bool loopable)
{
	loopable_audio = loopable;
}


bool AudioContainer::GetLoop()
{
	return loopable_audio;
}

int AudioContainer::AddSoundID(int id, float volume) 
{
	bool found = false;
	for (size_t y = 0; y < content_list.size(); y++)
	{
		if (content_list[y]->id == id)
		{
			content_list[y]->volume = volume;
			found = true;
			break;
		}
	}

	if (found) 
	{
		return 0;
	}
	else
	{
		AudioSourceContent* asContent = new AudioSourceContent();
		asContent->id = id;
		asContent->filename = "";
		asContent->volume = volume;
		content_list.push_back(asContent);
		return 1;
	}
}

int AudioContainer::AddSoundID(int id, std::string filename, float volume)
{
	bool found = false;
	for (size_t y = 0; y < content_list.size(); y++)
	{
		if (content_list[y]->id == id)
		{
			found = true;
			break;
		}
	}


	if (!found) 
	{
		AudioSourceContent* asContent = new AudioSourceContent();
		asContent->id = id;
		asContent->filename = filename.c_str();
		asContent->volume = volume;

		content_list.push_back(asContent);
		return 1;
	}
	return 0;
}

int AudioContainer::AddSoundID(std::vector<AudioSourceContent*>* sound_ids_list)
{
	if (sound_ids_list != nullptr)
	{
		int count = 0;
		
		for (size_t x = 0; x < sound_ids_list->size(); x++)
		{
			count += AddSoundID((*sound_ids_list)[x]->id, (*sound_ids_list)[x]->filename, (*sound_ids_list)[x]->volume);
		}

		return count;
	}
	return 0;
}

int AudioContainer::RemoveSoundID(int id)
{
	for (size_t x = 0; x < content_list.size(); x++)
	{
		if (content_list[x]->id == id) 
		{
			content_list.erase(content_list.begin() + x);
			return 1;
		}
	}
	return 0;
}