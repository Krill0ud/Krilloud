#include "AudioSourceNode.h"

AudioSourceNode::AudioSourceNode(int id, KPair pos, AudioPool * tree_pointer, std::string special_name)
{
	iD = id;
	
	if (special_name == "") 
	{
		name = "Audio Container";
	}
	else
	{
		name = special_name;
	}
	

	this->pos = pos;
	//inputsCount = 0;
	//outputsCount = 1;

	myClass = Class_AudioContainer;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(Class_AudioContainer, name, id, initial, 0);
	containedIDs = ((AudioContainer*)myKClass)->GetContainedIDs();

	can_be_deleted = true;

	filePath = "Select File";
	
	this->size = KPair(200, 100);

	tree_data_pointer = tree_pointer;

	nodeType = NodeType::Node_Container;

	CreateSocket(50, 0, Output, Normal, false, false);

	new_name = this->name;
}

AudioSourceNode::~AudioSourceNode()
{
	RemoveAllLinks();

	((AudioContainer*)myKClass)->~AudioContainer();
}

TagInfo * AudioSourceNode::DoTheThing()
{
	if (myKClass != nullptr) 
	{
		return myKClass->GetInfo();
	}
	return nullptr;
}

void AudioSourceNode::CheckNodeSize() 
{
	float extra = (show_special_info && containedIDs->size() > 0) ? (float)containedIDs->size() - 1 : 0;
	
	this->size = KPair(200, 100 + ((float)extra * 18));
}

int AudioSourceNode::GetSelectionMode()
{
	return ((AudioContainer*)myKClass)->selection_mode;
}

void AudioSourceNode::SetSelectionMode(int new_mode)
{
	((AudioContainer*)myKClass)->ChangeSelection(new_mode);
}

void AudioSourceNode::SetLoop(bool loopable)
{
	((AudioContainer*)myKClass)->SetLoop(loopable);
}

bool AudioSourceNode::GetLoopable()
{
	return ((AudioContainer*)myKClass)->GetLoop();
}

int AudioSourceNode::RemoveSound(int sound_id)
{
	return ((AudioContainer*)myKClass)->RemoveSoundID(sound_id);
}

int AudioSourceNode::AddSound(int sound_id, std::string file_full_path, int volume)
{
	return ((AudioContainer*)myKClass)->AddSoundID(sound_id, file_full_path.c_str(), volume);
}

std::vector<AudioSourceContent> AudioSourceNode::GetContainedIDs()
{
	std::vector<AudioSourceContent> result;

	for (size_t x = 0; x < containedIDs->size(); x++) 
	{
		AudioSourceContent content;

		content.filename = (*containedIDs)[x]->filename;
		content.id = (*containedIDs)[x]->id;
		content.volume = (*containedIDs)[x]->volume;

		result.push_back(content);
	}

	return result;
}

void AudioSourceNode::SetContainedClipVolume(int audioclip_id, float volume)
{
	for (size_t x = 0; x < containedIDs->size(); x++)
	{
		if ((*containedIDs)[x] != nullptr && (*containedIDs)[x]->id == audioclip_id) 
		{
			(*containedIDs)[x]->volume = volume;
			break;
		}
	}
}

void AudioSourceNode::AddSelectedTreeItem() 
{
	if (tree_data_pointer != nullptr && tree_data_pointer->selected_item_file_data != nullptr)
	{
		((AudioContainer*)myKClass)->AddSoundID(
			tree_data_pointer->selected_item_file_data->id, 
			tree_data_pointer->selected_item_file_data->file_name,
			1
		);

		CheckNodeSize();
	}
}

void AudioSourceNode::AddTreeFolder(directory_system* directory)
{
	if (tree_data_pointer != nullptr)
	{
		for (size_t x = 0; x < directory->files.size(); x++) 
		{
			for (size_t y = 0; y < tree_data_pointer->files_data.size(); y++) 
			{
				if (tree_data_pointer->files_data[y].file_name == directory->files[x]) 
				{
					((AudioContainer*)myKClass)->AddSoundID(
						tree_data_pointer->files_data[y].id,
						tree_data_pointer->files_data[y].file_name,
						1
					);
					break;
				}
			}
		}

		for (size_t x = 0; x < directory->directories.size(); x++) 
		{
			AddTreeFolder(directory->directories[x]);
		}

		CheckNodeSize();
	}
}

GraphSave AudioSourceNode::SaveNodeInfo()
{
	GraphSave myInfoToSave = Node::SaveNodeInfo();

	//myInfoToSave.loopable = GetLoopable();
	myInfoToSave.values.push_back(GetLoopable() ? 1 : 0);
	myInfoToSave.values.push_back(GetSelectionMode());

	for (size_t x = 0; x < containedIDs->size(); x++)
	{
		myInfoToSave.values.push_back((float)(*containedIDs)[x]->id);
		myInfoToSave.values.push_back((float)(*containedIDs)[x]->volume);
	}

	return myInfoToSave;
}

void AudioSourceNode::LoadNodeInfo(GraphSave* node_info)
{
	Node::LoadNodeInfo(node_info);

	//SetLoop(node_info->loopable);
	SetLoop(node_info->values[0] == 1 ? true : false);
	SetSelectionMode(node_info->values[1] == 1 ? 1 : 0);

	for (size_t x = 2; x < node_info->values.size(); x+=2)
	{
		for (size_t y = 0; y < tree_data_pointer->files_data.size(); y++) 
		{
			if (tree_data_pointer->files_data[y].id == (int)node_info->values[x]) 
			{
				((AudioContainer*)myKClass)->AddSoundID(tree_data_pointer->files_data[y].id, tree_data_pointer->files_data[y].file_name, node_info->values[x+1]);
				break;
			}
		}
	}
}