#include "KrilloudProjectHandler.h"

std::vector<GraphSave> copied_node_data;
std::vector<Node*> copied_node_originals;

std::vector<std::string> channel_names;

KrilloudProjectHandler::KrilloudProjectHandler(std::string krilloud_project_path, std::string game_folder_path, std::string project_name)
{
	krilloud_path = krilloud_project_path;
	project_filename = project_name;
	game_path = game_folder_path;
	krilloud_project_name = project_name;
	treeColumn = new AudioPool(krilloud_path);
	
	KrilloudEngine::Krilloud_create(false, krilloud_path);
	KrilloudEngine::Krilloud_init(1, 0, 0, 0, 2);
	
	engine_settings.push_back(EngineSettings(Windows, DefaultBackend, DefaultSampleRate, DefaultBuffer));
	engine_settings.push_back(EngineSettings(Android, DefaultBackend, DefaultSampleRate, DefaultBuffer));
	
	CreateNewTag("NewTag", 0);
	
	KrilloudEngine::CreateNewChannel("Master", 0, -1);
	
	KrilloudProjectInfo info = GetCurrentContractData();
	KrilloudEngine::SetContractData(info);
}

KrilloudProjectHandler::KrilloudProjectHandler(KrilloudProjectInfo loaded_project_info, std::string filename)
{
	//load data
	krilloud_path = loaded_project_info.project_path;
	project_filename = filename;
	game_path = loaded_project_info.game_path;
	krilloud_project_name = loaded_project_info.project_name;
	
	for (size_t x = 0; x < loaded_project_info.variables.size(); x++) 
	{
		CreateVariable(new VariableData(loaded_project_info.variables[x]));
	}
	
	treeColumn = new AudioPool(krilloud_path);
	
	KrilloudEngine::Krilloud_create(false, krilloud_path);
	
	bool engine_initialized = false;
	for (size_t x = 0; x < loaded_project_info.engine_configurations.size(); x++) 
	{
		if (!engine_initialized && loaded_project_info.engine_configurations[x].platform == SupportedPlatform::Windows)
		{
			KrilloudEngine::Krilloud_init(1,
				loaded_project_info.engine_configurations[x].backend,
				loaded_project_info.engine_configurations[x].sample_rate,
				loaded_project_info.engine_configurations[x].buffer_size,
				2);
			engine_initialized = true;
		}
		engine_settings.push_back(loaded_project_info.engine_configurations[x]);
	}
	
	if (!engine_initialized) 
	{
		KrilloudEngine::Krilloud_init(1, 0, 0, 0, 2);
	}
	
	if (loaded_project_info.channels.size() > 0) 
	{
		for (size_t x = 0; x < loaded_project_info.channels.size(); x++) 
		{
			int channel_id = loaded_project_info.channels[x].channel_id;

			if (channel_id < 0) 
			{
				loaded_project_info.channels[x].channel_id = (int)x;
			}

			KrilloudEngine::CreateNewChannel(loaded_project_info.channels[x].channel_name, loaded_project_info.channels[x].channel_id, loaded_project_info.channels[x].parent_id);
			
			KrilloudEngine::ChangeChannelVolume(loaded_project_info.channels[x].channel_name, loaded_project_info.channels[x].volume);
		}
	}
	else 
	{
		KrilloudEngine::CreateNewChannel("Master", 0, -1);
	}

	if (loaded_project_info.tags.size() > 0) 
	{
		for (size_t x = 0; x < loaded_project_info.tags.size(); x++)
		{
			CreateNewTag(loaded_project_info.tags[x]);
		}
	}
	else 
	{
		CreateNewTag("NewTag", 0);
	}

	KrilloudProjectInfo info = GetCurrentContractData();
	KrilloudEngine::SetContractData(info);
}

KrilloudProjectHandler::~KrilloudProjectHandler()
{
	//currentTag->rootLevel->~NodeGraphSystemLevel();
	for (int x = 0; x < myTags.size(); x++) 
	{
		myTags[x].rootLevel->~NodeGraphSystemLevel();
	}


	KrilloudEngine::StopAllInstancedTags();
	KrilloudEngine::Krilloud_deinit(); //deprecated
	KrilloudEngine::Krilloud_destroy();
}

KrilloudProjectInfo KrilloudProjectHandler::load(const char *fileLocation)
{
	 return Load(fileLocation);
}

KrilloudProjectInfo KrilloudProjectHandler::GetCurrentContractData()
{
	std::vector<LevelResult*> levelsInfo;

	for (size_t x = 0; x < myTags.size(); x++)
	{
		LevelResult* result = myTags[x].rootLevel->SaveGraphLevel();
		result->is3Dtag = myTags[x].data.tag.is3D;
		result->attenuationType = myTags[x].data.tag.attenuation;
		result->Max3DDistance = myTags[x].data.tag.max_3D_distance;
		result->Min3DDistance = myTags[x].data.tag.min_3D_distance;
		result->roll_off_factor = myTags[x].data.tag.roll_off_factor;
		result->doppler_active = myTags[x].data.tag.doppler_active;
		result->doppler_factor = myTags[x].data.tag.doppler_factor;
		result->distance_delay = myTags[x].data.tag.distance_delay;
		result->isLoopable = myTags[x].data.tag.isLoopable;
		result->channel_id = myTags[x].data.tag.channel_id;

		levelsInfo.push_back(result);
	}

	KrilloudProjectInfo project_data;

	project_data.game_path = game_path;
	project_data.project_name = krilloud_project_name;
	project_data.project_path = krilloud_path;

	for (int x = 0; x < KrilloudEngine::GetChannelCount(); x++)
	{
		project_data.channels.push_back(*KrilloudEngine::GetChannel(x));
	}

	for (size_t x = 0; x < levelsInfo.size(); x++)
	{
		project_data.tags.push_back(*levelsInfo[x]);
	}

	for (size_t x = 0; x < project_KVars.size(); x++)
	{
		project_data.variables.push_back(*project_KVars[x]);
	}


	return project_data;
}

void KrilloudProjectHandler::SaveProject()
{
	std::vector<LevelResult*> levelsInfo;

	for (size_t x = 0; x < myTags.size(); x++)
	{
		LevelResult* result = myTags[x].rootLevel->SaveGraphLevel();
		result->is3Dtag = myTags[x].data.tag.is3D;
		result->attenuationType = myTags[x].data.tag.attenuation;
		result->Max3DDistance = myTags[x].data.tag.max_3D_distance;
		result->Min3DDistance = myTags[x].data.tag.min_3D_distance;
		result->roll_off_factor = myTags[x].data.tag.roll_off_factor;
		result->doppler_active = myTags[x].data.tag.doppler_active;
		result->doppler_factor = myTags[x].data.tag.doppler_factor;
		result->distance_delay = myTags[x].data.tag.distance_delay;
		result->isLoopable = myTags[x].data.tag.isLoopable;
		result->channel_id = myTags[x].data.tag.channel_id;

		levelsInfo.push_back(result);
	}

	KrilloudProjectInfo project_data;

	project_data.game_path = game_path;
	project_data.project_name = krilloud_project_name;
	project_data.project_path = krilloud_path;

	project_data.engine_configurations = engine_settings;

	for (int x = 0; x < KrilloudEngine::GetChannelCount(); x++)
	{
		project_data.channels.push_back(*KrilloudEngine::GetChannel(x));
	}
	
	for (size_t x = 0; x < levelsInfo.size(); x++)
	{
		project_data.tags.push_back(*levelsInfo[x]);
	}
	
	for (size_t x = 0; x < project_KVars.size(); x++) 
	{
		project_data.variables.push_back(*project_KVars[x]);
	}
	
	Save_As(krilloud_path.c_str(), project_filename, project_data);
}

void KrilloudProjectHandler::SaveAsProject(std::string new_file_name) 
{
	std::vector<LevelResult*> levelsInfo;

	for (size_t x = 0; x < myTags.size(); x++)
	{
		LevelResult* result = myTags[x].rootLevel->SaveGraphLevel();
		result->is3Dtag = myTags[x].data.tag.is3D;
		result->attenuationType = myTags[x].data.tag.attenuation;
		result->Max3DDistance = myTags[x].data.tag.max_3D_distance;
		result->Min3DDistance = myTags[x].data.tag.min_3D_distance;
		result->roll_off_factor = myTags[x].data.tag.roll_off_factor;
		result->doppler_active = myTags[x].data.tag.doppler_active;
		result->doppler_factor = myTags[x].data.tag.doppler_factor;
		result->distance_delay = myTags[x].data.tag.distance_delay;
		result->isLoopable = myTags[x].data.tag.isLoopable;
		result->channel_id = myTags[x].data.tag.channel_id;

		levelsInfo.push_back(result);
	}

	KrilloudProjectInfo project_data;

	project_data.game_path = game_path;
	project_data.project_name = krilloud_project_name;
	project_data.project_path = krilloud_path;

	project_data.engine_configurations = engine_settings;

	for (int x = 0; x < KrilloudEngine::GetChannelCount(); x++)
	{
		project_data.channels.push_back(*KrilloudEngine::GetChannel(x));
	}

	for (size_t x = 0; x < levelsInfo.size(); x++)
	{
		project_data.tags.push_back(*levelsInfo[x]);
	}
	
	for (size_t x = 0; x < project_KVars.size(); x++)
	{
		project_data.variables.push_back(*project_KVars[x]);
	}


	Save_As(krilloud_path.c_str(), new_file_name, project_data);
}

void KrilloudProjectHandler::ExportProject(bool also_save) 
{
	std::vector<LevelResult*> levelsInfo;

	for (size_t x = 0; x < myTags.size(); x++)
	{
		LevelResult* result = myTags[x].rootLevel->SaveGraphLevel();
		result->is3Dtag = myTags[x].data.tag.is3D;
		result->attenuationType = myTags[x].data.tag.attenuation;
		result->Min3DDistance = myTags[x].data.tag.min_3D_distance;
		result->Max3DDistance = myTags[x].data.tag.max_3D_distance;
		result->roll_off_factor = myTags[x].data.tag.roll_off_factor;
		result->doppler_active = myTags[x].data.tag.doppler_active;
		result->doppler_factor = myTags[x].data.tag.doppler_factor;
		result->distance_delay = myTags[x].data.tag.distance_delay;
		result->isLoopable = myTags[x].data.tag.isLoopable;
		result->channel_id = myTags[x].data.tag.channel_id;

		levelsInfo.push_back(result);
	}

	KrilloudProjectInfo project_data;

	project_data.game_path = game_path;
	project_data.project_name = krilloud_project_name;
	project_data.project_path = krilloud_path;

	project_data.engine_configurations = engine_settings;

	for (int x = 0; x < KrilloudEngine::GetChannelCount(); x++)
	{
		project_data.channels.push_back(*KrilloudEngine::GetChannel(x));
	}

	for (size_t x = 0; x < levelsInfo.size(); x++)
	{
		project_data.tags.push_back(*levelsInfo[x]);
	}

	KrillInfo* soundBanksInfo = ExportAllSoundbanks();

	if (soundBanksInfo != nullptr)
	{
		for (size_t x = 0; x < soundBanksInfo->files.size(); x++)
		{
			project_data.sounbank_files.push_back(soundBanksInfo->files[x]);
		}
	}

	for (size_t x = 0; x < project_KVars.size(); x++)
	{
		project_data.variables.push_back(*project_KVars[x]);
	}

	if (also_save) 
	{
		Save(krilloud_path.c_str(), project_data);
	}

	time_t now = time(0);
	long int creation_time = static_cast<long int>(now);
	project_data.creation_time = creation_time;

	std::stringstream ss;
	ss << game_path << "\\Contract.json";
	const std::string outputMetadataPath = ss.str();
	try
	{
		std::ofstream ofsMetadata(outputMetadataPath);
		{
			cereal::JSONOutputArchive archive(ofsMetadata);
			archive(project_data);
		}
	}
	catch (...)
	{

	}
}

// -- Project options
const char* KrilloudProjectHandler::GetProjectPath()
{
	return krilloud_path.c_str();
}

bool KrilloudProjectHandler::SetNewProjectPath(std::string new_project_path)
{
	if (new_project_path.size() <= 0)
		return false;

	krilloud_path = new_project_path;
	return true;
}

const char* KrilloudProjectHandler::GetGamePath()
{
	return game_path.c_str();
}

bool KrilloudProjectHandler::SetNewGamePath(std::string new_game_path)
{
	if (new_game_path.size() <= 0)
		return false;

	game_path = new_game_path;
	return true;
}

const char* KrilloudProjectHandler::GetProjectName()
{
	return krilloud_project_name.c_str();
}

bool KrilloudProjectHandler::SetNewProjectName(std::string new_project_name) 
{
	if (new_project_name.size() <= 0)
		return false;

	krilloud_project_name = new_project_name;
	//project_filename = new_project_name;
	return true;
}

std::vector<std::pair<const char*, int>> KrilloudProjectHandler::GetEngineAvailableBackends(SupportedPlatform platform)
{
	std::vector<std::pair<const char*, int>> result;
	
	if (platform == SupportedPlatform::Windows) 
	{
		result.push_back(std::pair<const char*, int>("Default (Wasapi)", 0));
		result.push_back(std::pair<const char*, int>("WinMM", 1));
		result.push_back(std::pair<const char*, int>("XAudio2", 2));
		result.push_back(std::pair<const char*, int>("Wasapi", 3));
	}
	else if (platform == SupportedPlatform::Android)
	{
		result.push_back(std::pair<const char*, int>("Default (OpenSLES)", 0));
		result.push_back(std::pair<const char*, int>("OpenSLES", 4));
	}

	return result;
}
std::vector<std::pair<const char*, int>> KrilloudProjectHandler::GetEngineAvailableBufferSizes(SupportedPlatform platform)
{
	std::vector<std::pair<const char*, int>> result;

	result.push_back(std::pair<const char*, int>("Default (4096)", 0));
	for (size_t x = 1; x < 9; x++) 
	{
		std::stringstream ss;
		ss << std::pow(2,4+x);
		result.push_back(std::pair<const char*, int>(ss.str().c_str(), x));
	}

	return result;
}
std::vector<std::pair<const char*, int>> KrilloudProjectHandler::GetEngineAvailableChannels(SupportedPlatform platform)
{
	std::vector<std::pair<const char*, int>> result;

	result.push_back(std::pair<const char*, int>("Default (Stereo)", 0));
	result.push_back(std::pair<const char*, int>("Mono", 1));
	result.push_back(std::pair<const char*, int>("Stereo", 2));

	return result;
}
std::vector<std::pair<const char*, int>> KrilloudProjectHandler::GetEngineAvailableSampleRates(SupportedPlatform platform)
{
	std::vector<std::pair<const char*, int>> result;

	result.push_back(std::pair<const char*, int>("Default (44100)", 0));
	result.push_back(std::pair<const char*, int>("44100", 1));
	result.push_back(std::pair<const char*, int>("48000", 2));
	result.push_back(std::pair<const char*, int>("88200", 3));
	result.push_back(std::pair<const char*, int>("96000", 4));

	return result;
}

bool KrilloudProjectHandler::SetEngineSettings(std::vector<EngineSettings> new_settings)
{
	bool everything_ok = true;
	for (size_t x = 0; x < new_settings.size(); x++) 
	{
		for (size_t y = 0; y < engine_settings.size(); y++)
		{
			if (engine_settings[y].platform == new_settings[x].platform)
			{
				if (engine_settings[y].platform == SupportedPlatform::Windows) 
				{
					if ((engine_settings[y].backend != new_settings[x].backend)
						|| (engine_settings[y].sample_rate != new_settings[x].sample_rate)
						|| (engine_settings[y].buffer_size != new_settings[x].buffer_size))
					{

						everything_ok = KrilloudEngine::Krilloud_init(1,
							new_settings[x].backend,
							new_settings[x].sample_rate,
							new_settings[x].buffer_size, 
							2);
					}
				}
				engine_settings[y] = new_settings[x];
				break;
			}
		}		
	}
	return everything_ok;
}

std::vector<EngineSettings> KrilloudProjectHandler::GetEngineSettings()
{
	return engine_settings;
}

// -- Selections
void KrilloudProjectHandler::CopySelection(size_t position, std::vector<int> nodes_selection)
{
	if (position < myTags.size()) 
	{
		if (myTags[position].currentLevel != nullptr)
		{
			myTags[position].currentLevel->CopySelection(nodes_selection);
		}
	}
}

void KrilloudProjectHandler::CopySelection(std::string tag_name, std::vector<int> nodes_selection)
{
	for (size_t x = 0; x < myTags.size(); x++) 
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			CopySelection(x, nodes_selection);
			return;
		}
	}
}

void KrilloudProjectHandler::PasteSelection(size_t position, KPair start_position, std::vector<int> nodes_selection)
{
	if (position < myTags.size())
	{
		if (myTags[position].currentLevel != nullptr)
		{
			myTags[position].currentLevel->PasteSelection(start_position, nodes_selection);
		}
	}
}

void KrilloudProjectHandler::PasteSelection(std::string tag_name, KPair start_position, std::vector<int> nodes_selection)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			PasteSelection(x, start_position, nodes_selection);
			return;
		}
	}
}

void KrilloudProjectHandler::DuplicateSelection(size_t position, KPair start_position, std::vector<int> nodes_selection)
{
	if (position < myTags.size())
	{
		if (myTags[position].currentLevel != nullptr)
		{
			myTags[position].currentLevel->DuplicateSelection(start_position, nodes_selection);
		}
	}
}

void KrilloudProjectHandler::DuplicateSelection(std::string tag_name, KPair start_position, std::vector<int> nodes_selection)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			DuplicateSelection(x, start_position, nodes_selection);
			return;
		}
	}
}

void KrilloudProjectHandler::DeleteSelectedNodes(size_t position, std::vector<int> nodes_selection)
{
	if (position < myTags.size())
	{
		if (myTags[position].currentLevel != nullptr)
		{
			myTags[position].currentLevel->DeleteSelectedNodes(nodes_selection);
		}
	}
}

void KrilloudProjectHandler::DeleteSelectedNodes(std::string tag_name, std::vector<int> nodes_selection)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			DeleteSelectedNodes(x, nodes_selection);
			return;
		}
	}
}

// --- Helper functions



void KrilloudProjectHandler::CreateNewVariable(std::string newName, int tries)
{
	bool no_conflict = true;
	char name[1024];
	if (tries > 0) {
		sprintf_s(name, "%s%d", newName.c_str(), tries);
	}
	else {
		sprintf_s(name, "%s", newName.c_str());
	}

	for (size_t x = 0; x < project_KVars.size(); x++)
	{
		if (project_KVars[x]->variable_name == name)
		{
			no_conflict = false;
			break;
		}
	}

	if (no_conflict)
	{
		VariableData* new_Kvar = new VariableData();

		new_Kvar->variable_name = name;
		new_Kvar->type = Global;
		new_Kvar->content = Raw;
		new_Kvar->source = InGame;
		new_Kvar->current_value = 0;
		new_Kvar->range_min = 0;
		new_Kvar->range_max = 1;
		new_Kvar->set_only_on_play = false;

		CreateVariable(new_Kvar);
	}
	else
	{
		CreateNewVariable(newName, ++tries);
	}
}

void KrilloudProjectHandler::CreateVariable(VariableData* data)
{
	if (data != nullptr) 
	{
		project_KVars.push_back(KrilloudEngine::CreateNewKVariable(*data));
	}
}

void KrilloudProjectHandler::RemoveVariable(int position) 
{
	if (position < project_KVars.size()) 
	{
		project_KVars.erase(project_KVars.begin() + position);

		KrilloudEngine::RemoveKVariable(position);
	}
}

// -- GraphNode helper
Node* KrilloudProjectHandler::GetGraphNode(size_t target_tag_position, int node_id)
{
	if (target_tag_position < myTags.size()) 
	{
		return myTags[target_tag_position].currentLevel->GetNode(node_id);
	}
	return nullptr;
}

// --- Soundbank management functions

KrillInfo* KrilloudProjectHandler::ExportAllSoundbanks() 
{
	if (treeColumn != nullptr) 
	{
		int index = 0;
		std::stringstream ss;

		std::hash<std::string> hasher;
		size_t hash = hasher(krilloud_project_name);

		KrillInfo* final_metadata = new KrillInfo();
		KrillInfo* received_metadata = nullptr;

		if (treeColumn->directories->files.size() > 0) 
		{
			ss << "KSB" << hash << index << "DATA";
			index++;
			received_metadata = ExportSoundbank(ss.str().c_str(), GetPoolFileData(treeColumn->directories->files));
		}
		
		if (received_metadata != nullptr) 
		{
			for (size_t x = 0; x < received_metadata->files.size(); x++)
			{
				final_metadata->files.emplace_back(received_metadata->files[x]);
			}
		}

		ss.clear();
		ss.str(std::string());

		std::vector<std::string> files;
		for (size_t x = 0; x < treeColumn->directories->directories.size(); x++)
		{
			files.clear();

			files = GetAllFilesInDirectory(*treeColumn->directories->directories[x]);

			if (files.size() > 0) 
			{
				ss.clear();
				ss.str(std::string());
				ss << "KSB" << hash << index << "DATA";

				received_metadata = ExportSoundbank(ss.str().c_str(), GetPoolFileData(files));
				
				if (received_metadata != nullptr) 
				{
					for (size_t y = 0; y < received_metadata->files.size(); y++)
					{
						final_metadata->files.emplace_back(received_metadata->files[y]);
					}
				}

				index++;
			}
		}

		return final_metadata;
	}
	return nullptr;
}

std::vector<std::string> KrilloudProjectHandler::GetAllFilesInDirectory(directory_system directory)
{
	/*for (size_t x = 0; x < directory->files.size(); x++)
	{
		recipient_list->push_back(directory->files[x]);
	}

	for (size_t x = 0; x < directory->directories.size(); x++)
	{
		GetAllFilesInDirectory(directory->directories[x], recipient_list);
	}*/

	std::vector<std::string> recipient_list;

	for (size_t x = 0; x < directory.files.size(); x++)
	{
		recipient_list.push_back(directory.files[x]);
	}

	for (size_t x = 0; x < directory.directories.size(); x++)
	{
		std::vector<std::string> directories_list = GetAllFilesInDirectory(*directory.directories[x]);

		for (size_t y = 0; y < directories_list.size(); y++)
		{
			recipient_list.push_back(directories_list[y]);
		}
	}

	return recipient_list;
}

std::vector<pool_file_data> KrilloudProjectHandler::GetPoolFileData(std::vector<std::string> full_path_fileNames)
{
	std::vector<pool_file_data> new_pool_file_data;
	for (size_t x = 0; x < treeColumn->files_data.size(); x++)
	{
		for (size_t y = 0; y < full_path_fileNames.size(); y++)
		{
			if (treeColumn->files_data[x].file_name == full_path_fileNames[y]) 
			{
				new_pool_file_data.push_back(treeColumn->files_data[x]);
				break;
			}
		}
	}
	return new_pool_file_data;
}

KrillInfo* KrilloudProjectHandler::ExportSoundbank(const char * soundbank_file_name, std::vector<pool_file_data> files_to_pack) 
{
	if (files_to_pack.size() > 0) 
	{
		std::stringstream ss;
		ss << soundbank_file_name << ".krll";
		const std::string soundBank_fileName = ss.str();

		ss.clear();
		ss.str(std::string());
		ss << game_path << "\\" << soundBank_fileName;
		const std::string outputBinaryPath = ss.str();

		KrillInfo* metadata = new KrillInfo();;

		// create binary file with all input wavs
		std::ofstream ofsBinary(outputBinaryPath, std::ios::binary);

		for (size_t x = 0; x < files_to_pack.size(); x++)
		{
			DumpFile(files_to_pack[x].file_name.c_str(), files_to_pack[x].id, ofsBinary, soundBank_fileName, *metadata);
		}

		ss.clear();
		ss.str(std::string());

		ss << game_path << "\\" << soundbank_file_name << ".json";
		const std::string outputMetadataPath = ss.str();

		return metadata;
	}
	return nullptr;
}

void KrilloudProjectHandler::DumpFile(const char* path, int file_id, std::ofstream &ofs, const std::string &outputBankPath, KrillInfo &metadata)
{
	std::ifstream ifs(path, std::ios::binary);
	if (ifs)
	{
		const size_t initialOffset = (size_t)ofs.tellp();

		// get size of file
		ifs.seekg(0, ifs.end);
		long size = long(ifs.tellg());
		ifs.seekg(0);

		// allocate memory for file content
		char* buffer = new char[size];

		// read content of infile
		ifs.read(buffer, size);

		// write to outfile
		ofs.write(buffer, size);
		// TODO: padding (escribir ceros hasta que el offset sea el adecuado para la plataforma)

		ifs.close();

		const size_t endOffset = size_t(ofs.tellp());
		
		metadata.files.emplace_back(KrillFileDescriptor{ file_id
														, outputBankPath
														, initialOffset
														, endOffset - initialOffset }); // write position (overall write offset in output stream)
	}
}



bool KrilloudProjectHandler::FileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

// -- Channels
bool KrilloudProjectHandler::CreateNewChannel(std::string channel_name, int parent_id)
{
	int new_id = 1;
	int channels_count = KrilloudEngine::GetChannelCount();
	std::vector<int> ids;

	for (int x = 0; x < channels_count; x++) 
	{
		Channel* channel = KrilloudEngine::GetChannel(x);

		if (channel != nullptr) 
		{
			ids.push_back(channel->channel_id);
		}
	}

	bool found = false;

	while (!found) 
	{
		found = true;
		for (size_t x = 0; x < ids.size(); x++) 
		{
			if (ids[x] == new_id)
			{
				new_id++;
				found = false;
			}
		}		
	}

	return KrilloudEngine::CreateNewChannel(channel_name, new_id, parent_id);
}

void KrilloudProjectHandler::RemoveChannel(int channel_id)
{
	KrilloudEngine::RemoveChannel(channel_id);
}

Channel* KrilloudProjectHandler::GetChannel(int position)
{
	return KrilloudEngine::GetChannel(position);
}

Channel* KrilloudProjectHandler::GetChannelByID(int channel_id)
{
	return KrilloudEngine::GetChannelByID(channel_id);
}

int KrilloudProjectHandler::GetChannelCount()
{
	return KrilloudEngine::GetChannelCount();
}

const char* KrilloudProjectHandler::GetChannelName(int position)
{
	auto count = KrilloudEngine::GetChannelCount();

	if (position < count) 
	{
		channel_names.clear();

		for (int x = 0; x <= position; x++) 
		{
			channel_names.push_back(KrilloudEngine::GetChannelName(x));
		}

		return channel_names[position].c_str();
	}
	else
	{
		return "";
	}
}

float KrilloudProjectHandler::GetChannelVolume(int position)
{
	return KrilloudEngine::GetChannelVolume(position);
}

float KrilloudProjectHandler::GetChannelOverallVolume(int position)
{
	return KrilloudEngine::GetChannelOverallVolume(position);
}

void KrilloudProjectHandler::ChangeChannelParent(int channel_id, int new_parent)
{
	KrilloudEngine::ChangeChannelParent(channel_id, new_parent);

	if (KrilloudRT::IsRealTimeOn())
	{
		KrilloudRT::NotifyChannelParentID(channel_id, new_parent);
	}
}

void KrilloudProjectHandler::ChangeChannelName(int channel_id, std::string new_name)
{
	KrilloudEngine::ChangeChannelName(channel_id, new_name);

	if (KrilloudRT::IsRealTimeOn())
	{
		KrilloudRT::NotifyChannelName(channel_id, new_name);
	}
}

void KrilloudProjectHandler::ChangeChannelVolume(int channel_id, float volume)
{
	KrilloudEngine::ChangeChannelVolume(channel_id, volume);

	if (KrilloudRT::IsRealTimeOn())
	{
		KrilloudRT::NotifyChannelVolume(channel_id, volume);
	}
}

// -- Audio Pool
void KrilloudProjectHandler::RefreshPool()
{
	if (treeColumn != nullptr) 
	{
		treeColumn->RefreshTree();
	}
}

size_t KrilloudProjectHandler::GetPoolFileCount()
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFileDataCount();
	}
	return -1;
}

size_t KrilloudProjectHandler::GetPoolFoldersCount()
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFolderDataCount();
	}
	return 0;
}


directory_system* KrilloudProjectHandler::GetPoolTree()
{
	if (treeColumn != nullptr) 
	{
		return treeColumn->GetDirectorySystem();
	}
	return new directory_system();
}

pool_file_data* KrilloudProjectHandler::GetPoolFileData(std::string file_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFileData(file_name);
	}
	return nullptr;
}

pool_file_data* KrilloudProjectHandler::GetPoolFileData(int id)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFileData(id);
	}
	return nullptr;
}

pool_file_data* KrilloudProjectHandler::GetFileDataByIndex(int file_index)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFileDataByIndex(file_index);
	}
	return nullptr;
}

pool_file_data* KrilloudProjectHandler::GetFolderDataByIndex(int file_index)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetFolderDataByIndex(file_index);
	}
	return nullptr;
}



int KrilloudProjectHandler::GetPoolItemID(std::string file_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->GetID(file_name);
	}
	return -1;
}

void KrilloudProjectHandler::AddFiles(std::vector<std::string> file_paths, std::string target_directory)
{
	if (treeColumn != nullptr)
	{
		treeColumn->AddFiles(file_paths, target_directory);
	}
}

bool KrilloudProjectHandler::MovePoolFile(std::string source_file, std::string directory_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->MovePoolFile(source_file, directory_name);
	}
	return false;
}

bool KrilloudProjectHandler::RenamePoolFile(std::string file_name, std::string new_file_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->RenamePoolFile(file_name, new_file_name);
	}
	return false;
}

bool KrilloudProjectHandler::RemovePoolFile(std::string file_name, bool update_file_data)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->RemovePoolFile(file_name, update_file_data);
	}
	return false;
}

bool KrilloudProjectHandler::CreateNewSubdirectory(std::string path_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->CreateNewSubdirectory(path_name);
	}
	return false;
}

bool KrilloudProjectHandler::MovePoolDirectory(std::string source_path, std::string output_path)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->MovePoolDirectory(source_path, output_path);
	}
	return false;
}

bool KrilloudProjectHandler::DeleteDirectory(std::string path_name)
{
	if (treeColumn != nullptr)
	{
		return treeColumn->DeleteDirectory(path_name);
	}
	return false;
}

// -- Tags
KrillTag* KrilloudProjectHandler::GetTag(size_t position)
{
	if (myTags.size() > position) 
	{
		KrillTag* selectedTag = new KrillTag();

		selectedTag->tag_name = myTags[position].data.tag.tag_name;
		selectedTag->owner_id = 0;
		
		selectedTag->is3D = myTags[position].data.tag.is3D;
		selectedTag->min_3D_distance = myTags[position].data.tag.min_3D_distance;
		selectedTag->max_3D_distance = myTags[position].data.tag.max_3D_distance;
		selectedTag->roll_off_factor = myTags[position].data.tag.roll_off_factor;
		selectedTag->attenuation = myTags[position].data.tag.attenuation;
		selectedTag->distance_delay = myTags[position].data.tag.distance_delay;
		selectedTag->doppler_active = myTags[position].data.tag.doppler_active;
		selectedTag->doppler_factor = myTags[position].data.tag.doppler_factor;

		selectedTag->channel_id = myTags[position].data.tag.channel_id;

		selectedTag->isLoopable = myTags[position].data.tag.isLoopable;

		//selectedTag->tag_content

		return selectedTag;
	}
	return nullptr;
}

KrillTag* KrilloudProjectHandler::GetTag(std::string tagname)
{
	for (size_t x = 0; x < myTags.size(); x++) 
	{
		if (myTags[x].data.tag.tag_name == tagname)
		{
			return GetTag(x);
		}
	}
	return nullptr;
}

int KrilloudProjectHandler::GetTagIndex(std::string tag_name)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			return x;
		}
	}
	return -1;
}

const char* KrilloudProjectHandler::GetTagName(size_t position)
{
	if (position < myTags.size()) 
	{
		return myTags[position].data.tag.tag_name.c_str();
	}
	return "";
}

size_t KrilloudProjectHandler::GetTagCount()
{
	return myTags.size();
}

bool KrilloudProjectHandler::IsTagPlaying(std::string tagname)
{
	return KrilloudEngine::isTagPlaying(tagname, 0);
}

int KrilloudProjectHandler::PlayTag(size_t position)
{
	if (myTags.size() > position)
	{
		return myTags[position].rootLevel->PlayCurrentTag();
	}
	return -101;
}

void KrilloudProjectHandler::StopTag(size_t position)
{
	if (myTags.size() > position)
	{
		myTags[position].rootLevel->StopPlayingTag();
	}
}

void KrilloudProjectHandler::CreateNewTag(std::string newName, int tries)
{
	bool no_conflict = true;
	char name[1024];
	if (tries > 0) {
		sprintf_s(name, "%s%d", newName.c_str(), tries);
	}
	else {
		sprintf_s(name, "%s", newName.c_str());
	}

	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == name)
		{
			no_conflict = false;
			break;
		}
	}

	if (no_conflict)
	{
		ProjectTag newTag;
		newTag.data.tag.tag_name = name;

		newTag.data.tag.is3D = false;
		newTag.data.tag.attenuation = 0;
		newTag.data.tag.min_3D_distance = 1;
		newTag.data.tag.max_3D_distance = 2;
		newTag.data.tag.roll_off_factor = 1;
		newTag.data.tag.doppler_active = false;
		newTag.data.tag.doppler_factor = 1;
		newTag.data.tag.distance_delay = false;
		newTag.data.tag.isLoopable = false;

		newTag.data.tag.channel_id = 0;

		newTag.data.tag_content = KrilloudEngine::CreateKLevel(); //&newTag.currentLevel->myKLevel;

		newTag.rootLevel = new NodeGraphSystemLevel(treeColumn, newTag.data.tag.tag_name, &project_KVars, newTag.data.tag_content);
		newTag.currentLevel = newTag.rootLevel;
		newTag.levelRootReader = newTag.currentLevel;

		myTags.push_back(newTag);

		//currentTag = &myTags[selectedTag];

		KrilloudEngine::CreateNewTag(name, newTag.data.tag_content);
		KrilloudEngine::SetTagLoopable(name, newTag.data.tag.isLoopable);
		KrilloudEngine::SetTagChannel(name, newTag.data.tag.channel_id);
	}
	else
	{
		CreateNewTag(newName, ++tries);
	}
}

void KrilloudProjectHandler::CreateNewTag(LevelResult tag_info)
{
	ProjectTag newTag;
	newTag.data.tag.tag_name = tag_info.tagname;

	newTag.data.tag.is3D = tag_info.is3Dtag;
	newTag.data.tag.attenuation = tag_info.attenuationType;
	newTag.data.tag.min_3D_distance = tag_info.Min3DDistance;
	newTag.data.tag.max_3D_distance = tag_info.Max3DDistance;
	newTag.data.tag.roll_off_factor = tag_info.roll_off_factor;
	newTag.data.tag.doppler_active = tag_info.doppler_active;
	newTag.data.tag.doppler_factor = tag_info.doppler_factor;
	newTag.data.tag.distance_delay = tag_info.distance_delay;
	newTag.data.tag.isLoopable = tag_info.isLoopable;

	newTag.data.tag.channel_id = tag_info.channel_id;

	
	newTag.data.tag_content = KrilloudEngine::CreateKLevel(); //&newTag.currentLevel->myKLevel;

	newTag.rootLevel = new NodeGraphSystemLevel(treeColumn, newTag.data.tag.tag_name, &project_KVars, newTag.data.tag_content);
	newTag.currentLevel = newTag.rootLevel;
	newTag.levelRootReader = newTag.currentLevel;
	newTag.rootLevel->LoadGraphLevel(&tag_info);

	myTags.push_back(newTag);

	//currentTag = &myTags[selectedTag];

	KrilloudEngine::CreateNewTag(newTag.data);

	/*KrilloudEngine::CreateNewTag(newTag.data.tag_name, newTag.data.tag_content);
	KrilloudEngine::SetTagLoopable(name, newTag.data.isLoopable);
	KrilloudEngine::SetTagChannel(name, newTag.data.channel_id);*/
}

void KrilloudProjectHandler::CloneTag(int selection)
{
	int tag_array_size = (int)myTags.size();

	if (selection < tag_array_size)
	{
		LevelResult* result = myTags[selection].rootLevel->SaveGraphLevel();
		result->is3Dtag = myTags[selection].data.tag.is3D;
		result->attenuationType = myTags[selection].data.tag.attenuation;
		result->Min3DDistance = myTags[selection].data.tag.min_3D_distance;
		result->Max3DDistance = myTags[selection].data.tag.max_3D_distance;
		result->roll_off_factor = myTags[selection].data.tag.roll_off_factor;
		result->doppler_active = myTags[selection].data.tag.doppler_active;
		result->doppler_factor = myTags[selection].data.tag.doppler_factor;
		result->distance_delay = myTags[selection].data.tag.distance_delay;
		result->isLoopable = myTags[selection].data.tag.isLoopable;
		result->channel_id = myTags[selection].data.tag.channel_id;

		std::stringstream ss;
		ss << result->tagname << "_clone";

		CreateNewTag(ss.str(), 0);

		myTags[tag_array_size].data.tag.channel_id = result->channel_id;
		myTags[tag_array_size].data.tag.isLoopable = result->isLoopable;

		myTags[tag_array_size].data.tag.is3D = result->is3Dtag;
		myTags[tag_array_size].data.tag.attenuation = result->attenuationType;
		myTags[tag_array_size].data.tag.min_3D_distance = result->Min3DDistance;
		myTags[tag_array_size].data.tag.max_3D_distance = result->Max3DDistance;
		myTags[tag_array_size].data.tag.roll_off_factor = result->roll_off_factor;

		myTags[tag_array_size].data.tag.doppler_active = result->doppler_active;
		myTags[tag_array_size].data.tag.doppler_factor = result->doppler_factor;

		myTags[tag_array_size].data.tag.distance_delay = result->distance_delay;

		myTags[tag_array_size].rootLevel->LoadGraphLevel(result);
	}
}

int KrilloudProjectHandler::TagExists(std::string tagname)
{
	return KrilloudEngine::IsTagLoaded(tagname);
}

bool KrilloudProjectHandler::RenameTag(std::string oldname, std::string newname)
{
	int position = -1;
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == oldname)
		{
			position = x;
		}
	}

	if (position >= 0) 
	{
		bool ok = KrilloudEngine::RenameTag(oldname, newname);

		if (ok)
		{
			myTags[position].data.tag.tag_name = newname;
			myTags[position].rootLevel->ChangeAssociatedTag(newname);
		}

		return ok;
	}
	return false;
}


void KrilloudProjectHandler::RemoveTag(std::string name)
{
	for (size_t x = 0; x < myTags.size(); x++) 
	{
		if (myTags[x].data.tag.tag_name == name)
		{
			myTags.erase(myTags.begin() + x);
			KrilloudEngine::RemoveTag(name);
		}
	}
}

bool KrilloudProjectHandler::SetTagLoopable(std::string name, bool new_value)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == name)
		{
			if (KrilloudEngine::SetTagLoopable(name, new_value)) 
			{
				myTags[x].data.tag.isLoopable = new_value;
				return true;
			}
			return false;
		}
	}
	return false;
}

bool KrilloudProjectHandler::SetTagChannel(std::string name, int new_value)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == name)
		{
			if (KrilloudEngine::SetTagChannel(name, new_value)) 
			{
				myTags[x].data.tag.channel_id = new_value;
				return true;
			}
			return false;
		}
	}
	return false;
}

bool KrilloudProjectHandler::SetTagAttenuation(std::string tag_name, int attenuation_type)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagAttenuation(tag_name, attenuation_type))
			{
				myTags[x].data.tag.attenuation = attenuation_type;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTag3D(std::string tag_name, bool is_tag_3d)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTag3D(tag_name, is_tag_3d))
			{
				myTags[x].data.tag.is3D = is_tag_3d;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTagDistanceDelay(std::string tag_name, bool has_distance_delay)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagDistanceDelay(tag_name, has_distance_delay))
			{
				myTags[x].data.tag.distance_delay = has_distance_delay;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTagDopplerEffect(std::string tag_name, bool is_doppler_active)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagDopplerEffect(tag_name, is_doppler_active))
			{
				myTags[x].data.tag.doppler_active = is_doppler_active;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTagDopplerFactor(std::string tag_name, float doppler_factor)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagDopplerFactor(tag_name, doppler_factor))
			{
				myTags[x].data.tag.doppler_factor = doppler_factor;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTagMinMaxDistance(std::string tag_name, float min_distance, float max_distance)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagMinMaxDistance(tag_name, min_distance, max_distance))
			{
				myTags[x].data.tag.min_3D_distance = min_distance;
				myTags[x].data.tag.max_3D_distance = max_distance;
				return true;
			}
			return false;
		}
	}
	return false;
}
bool KrilloudProjectHandler::SetTagRollOffFactor(std::string tag_name, float roll_off_factor)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == tag_name)
		{
			if (KrilloudEngine::SetTagRollOffFactor(tag_name, roll_off_factor))
			{
				myTags[x].data.tag.roll_off_factor = roll_off_factor;
				return true;
			}
			return false;
		}
	}
	return false;
}

// -- Nodes
int KrilloudProjectHandler::AddNode(NodeClass node_class, std::string target_tag_name, KPair position)
{
	for (size_t x = 0; x < myTags.size(); x++) 
	{
		if (myTags[x].data.tag.tag_name == target_tag_name)
		{
			return AddNode(node_class, x, position);
		}
	}
	return -1;
}

int KrilloudProjectHandler::AddNode(NodeClass node_class, size_t target_tag_position, KPair position)
{
	if (target_tag_position < myTags.size()) 
	{
		return myTags[target_tag_position].currentLevel->AddNewNode(node_class, position);
	}
	return -1;
}

int KrilloudProjectHandler::AddNodeCopy(size_t target_tag_position, int node_id)
{
	if (target_tag_position < myTags.size())
	{
		return myTags[target_tag_position].currentLevel->AddNodeCopy(node_id);
	}
	return -1;
}

bool KrilloudProjectHandler::RemoveNode(int iD, std::string target_tag_name)
{
	for (size_t x = 0; x < myTags.size(); x++)
	{
		if (myTags[x].data.tag.tag_name == target_tag_name)
		{
			return RemoveNode(iD, x);
		}
	}
	return false;
}

bool KrilloudProjectHandler::RemoveNode(int iD, size_t target_tag_position)
{
	if (target_tag_position < myTags.size())
	{
		return myTags[target_tag_position].currentLevel->RemoveNode(iD);
	}
	return false;
}

std::vector<int> KrilloudProjectHandler::GetNodesIDList(size_t target_tag_position)
{
	if (target_tag_position >= myTags.size() || myTags[target_tag_position].currentLevel == nullptr)
		return {};

	std::vector<Node*> nodes = myTags[target_tag_position].currentLevel->GetNodeList();

	std::vector<int> nodeInfoList;

	for (size_t x = 0; x < nodes.size(); x++) 
	{
		nodeInfoList.push_back(nodes[x]->GetID());
	}

	return nodeInfoList;
}

///GraphNodes
GraphSave KrilloudProjectHandler::GetNodeData(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return GraphSave();

	return node->SaveNodeInfo();
}

NodeType KrilloudProjectHandler::GetNodeType(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return NodeType::Node_Normal;

	return node->GetType();
}

NodeClass KrilloudProjectHandler::GetNodeClass(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return NodeClass::Class_ERROR;

	return node->GetNodeClass();
}

KPair KrilloudProjectHandler::GetNodeSize(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return { -1, -1 };

	return node->GetSize();
}

void KrilloudProjectHandler::SetNodeSize(size_t target_tag_position, int node_id, KPair new_size)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	node->SetSize(new_size);
}

KPair KrilloudProjectHandler::GetNodePosition(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return { -1, -1 };

	return node->GetPosition();
}

void KrilloudProjectHandler::SetNodePosition(size_t target_tag_position, int node_id, KPair new_position)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	node->SetPosition(new_position);
}

bool KrilloudProjectHandler::CanBeDeleted(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	return node->CanBeDeleted();
}

const char* KrilloudProjectHandler::GetNodeName(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return "";

	return node->GetName();
}

void KrilloudProjectHandler::SetNodeName(size_t target_tag_position, int node_id, std::string new_name)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	node->SetName(new_name);
}

int KrilloudProjectHandler::GetNodeOutputsCount(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return node->GetOutputsCount();
}

int KrilloudProjectHandler::GetNodeInputsCount(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return node->GetInputsCount();
}

bool KrilloudProjectHandler::NodeCanHaveLinks(size_t target_tag_position, int node_id, size_t socketIndex)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	return node->CanHaveLinks(socketIndex);
}

size_t KrilloudProjectHandler::GetNodeSocketLinksCount(size_t target_tag_position, int node_id, size_t socketIndex)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return 0;

	return node->SocketLinksCount(socketIndex);
}

bool KrilloudProjectHandler::CreateNodeLink(size_t target_tag_position, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex)
{
	Node* node = GetGraphNode(target_tag_position, node_id);
	Node* nodeToLink = GetGraphNode(target_tag_position, nodeToLink_id);

	if (node == nullptr || nodeToLink == nullptr)
		return false;

	return node->CreateLink(socketIndex, nodeToLink, linkedNodeSocketIndex);
	//return true;
}

bool KrilloudProjectHandler::RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	return node->RemoveLink(socket);
	//return true;
}

bool KrilloudProjectHandler::RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, size_t link)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	return node->RemoveLink(socket, link);
	//return true;
}

bool KrilloudProjectHandler::RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, int link_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);
	Node* link = GetGraphNode(target_tag_position, link_id);
	
	if (node == nullptr || link == nullptr)
		return false;

	return node->RemoveLink(socket, link);
	//return true;
}

bool KrilloudProjectHandler::RemoveAllNodeLinks(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	return node->RemoveAllLinks();
	//return true;
}

const char* KrilloudProjectHandler::GetLinkedNodeName(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return "";

	return node->GetName();
}

int KrilloudProjectHandler::GetLinkedID(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return node->GetLinkedID(socketIndex, link_index);
}

int KrilloudProjectHandler::GetLinkedIDSocket(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return node->GetLinkedIDSocket(socketIndex, link_index);
}

bool KrilloudProjectHandler::CreateNodeSocket(size_t target_tag_position, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	node->CreateSocket(position, socket_id, sType, sDType, isDeletable, multipleLinks);
	return true;
}

bool KrilloudProjectHandler::DeleteNodeSocket(size_t target_tag_position, int node_id, size_t socket_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	node->DeleteSocket(socket_id);
	return true;
}

int KrilloudProjectHandler::NodeSocketHasConnection(size_t target_tag_position, int node_id, int index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return node->SocketHasConnection(index);
}

int KrilloudProjectHandler::GetNodeSocketCount(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return 0;

	return node->GetSocketCount();
}

float KrilloudProjectHandler::GetNodeSocketPosition(size_t target_tag_position, int node_id, int index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return {};

	return node->GetSocketPosition(index);
}

SocketDataType KrilloudProjectHandler::GetNodeSocketDataType(size_t target_tag_position, int node_id, int index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return SocketDataType::Normal;

	return node->GetSocketDataType(index);
}

SocketType KrilloudProjectHandler::GetSocketType(size_t target_tag_position, int node_id, int index)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return SocketType::Input;

	return node->GetSocketType(index);
}

std::vector<FilterParameter> KrilloudProjectHandler::GetNodeFilterParameters(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return {};

	if (node->GetType() != NodeType::Node_Filter)
		return {};

	return ((FilterNode*)node)->GetParameters();
}

void KrilloudProjectHandler::SetNodeFilterParameter(size_t target_tag_position, int node_id, int parameter_index, float new_value)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	if (node->GetType() != NodeType::Node_Filter)
		return;

	((FilterNode*)node)->SetParameter(parameter_index, new_value);

	if (KrilloudRT::IsRealTimeOn())
	{
		KrilloudRT::NotifyFilterParameter(target_tag_position, node_id, parameter_index, new_value);
	}
}

///Mixer specifics
std::vector<float> KrilloudProjectHandler::GetMixerCurrentVolumes(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return {};

	if (node->GetType() != NodeType::Node_Mixer)
		return {};

	return ((MixerNode*)node)->GetCurrentVolumes();
}

void KrilloudProjectHandler::SetMixerSocketCurrentVolume(size_t target_tag_position, int node_id, int parameter_index, float new_value)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	if (node->GetType() != NodeType::Node_Mixer)
		return;

	((MixerNode*)node)->SetVolume(parameter_index, new_value);

	if (KrilloudRT::IsRealTimeOn())
	{
		KrilloudRT::NotifyMixerVolumeChange(target_tag_position, node_id, parameter_index, new_value);
	}
}

/// Audio source specifics
int KrilloudProjectHandler::GetAudioSourceSelectionMode(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return 0;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return 0;

	return ((AudioSourceNode*)node)->GetSelectionMode();
}

void KrilloudProjectHandler::SetAudioSourceSelectionMode(size_t target_tag_position, int node_id, int new_value)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return;

	((AudioSourceNode*)node)->SetSelectionMode(new_value);
}

void KrilloudProjectHandler::SetAudioContainerLoopable(size_t target_tag_position, int node_id, bool loopable)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return;

	((AudioSourceNode*)node)->SetLoop(loopable);
}

bool KrilloudProjectHandler::GetAudioContainerLoopable(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return false;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return false;

	return ((AudioSourceNode*)node)->GetLoopable();
}

int KrilloudProjectHandler::RemoveSound(size_t target_tag_position, int node_id, int sound_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return -1;

	return ((AudioSourceNode*)node)->RemoveSound(sound_id);
}

int KrilloudProjectHandler::AddSound(size_t target_tag_position, int node_id, int sound_id, std::string file_full_path, int volume)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return -1;

	return ((AudioSourceNode*)node)->AddSound(sound_id, file_full_path, volume);
}

std::vector<AudioSourceContent> KrilloudProjectHandler::GetContainedIDs(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return {};

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return {};

	return ((AudioSourceNode*)node)->GetContainedIDs();
}

void KrilloudProjectHandler::SetContainedClipVolume(size_t target_tag_position, int node_id, int audioclip_id, float volume)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	if (node->GetNodeClass() != NodeClass::Class_AudioContainer)
		return;

	((AudioSourceNode*)node)->SetContainedClipVolume(audioclip_id, volume);
}

/// Variable node specifics
void KrilloudProjectHandler::SetSimulatedValue(size_t target_tag_position, int node_id, float newValue)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	((IngameVariable*)node)->SetSimulatedValue(newValue);
}

void KrilloudProjectHandler::SetOnlyOnPlay(size_t target_tag_position, int node_id, bool newValue)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	((IngameVariable*)node)->SetOnlyOnPlay(newValue);
}

void KrilloudProjectHandler::SetVariableSelection(size_t target_tag_position, int node_id, int new_value)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return;

	((IngameVariable*)node)->SetVariableSelection(new_value);
}

int KrilloudProjectHandler::GetVariableSelection(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return -1;

	return ((IngameVariable*)node)->GetVariableSelection();
}

//void KrilloudProjectHandler::UpdateKVariableData(size_t target_tag_position, int node_id)
//{
//	Node* node = GetGraphNode(target_tag_position, node_id);
//
//	if (node == nullptr)
//		return;
//
//	((IngameVariable*)node)->UpdateKVariableData();
//}

VariableData* KrilloudProjectHandler::GetNodeAssociatedKVar(size_t target_tag_position, int node_id)
{
	Node* node = GetGraphNode(target_tag_position, node_id);

	if (node == nullptr)
		return new VariableData();

	return ((IngameVariable*)node)->GetAssociatedData();
}

// -- KrillVariables
size_t KrilloudProjectHandler::GetKVarCount()
{
	return project_KVars.size();
}

const char* KrilloudProjectHandler::GetKVariableName(int variable_position)
{
	if (variable_position < project_KVars.size())
	{
		return project_KVars[variable_position]->variable_name.c_str();
	}
	return "";
}

int KrilloudProjectHandler::GetKVariablePosition(std::string variable_name) 
{
	for (size_t x = 0; x < project_KVars.size(); x++) 
	{
		if (project_KVars[x]->variable_name == variable_name) 
		{
			return x;
		}
	}
	return -1;
}

bool KrilloudProjectHandler::SetVariableName(int position, std::string new_name)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->previous_name = project_KVars[position]->variable_name;

		project_KVars[position]->variable_name = new_name;
		
		return KrilloudEngine::SetVariableName(position, new_name);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableCurrentValue(int position, float new_value)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->current_value = new_value;

		return KrilloudEngine::SetVariableCurrentValue(position, new_value);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableType(int position, VarType new_type)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->type = new_type;

		return KrilloudEngine::SetVariableType(position, new_type);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableSource(int position, VarSourceType new_source_type)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->source = new_source_type;

		return KrilloudEngine::SetVariableSource(position, new_source_type);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableContent(int position, VarContentType new_content_type)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->content = new_content_type;

		return KrilloudEngine::SetVariableContent(position, new_content_type);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableRangeMin(int position, float new_range_min)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->range_min = new_range_min;

		return KrilloudEngine::SetVariableRangeMin(position, new_range_min);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableRangeMax(int position, float new_range_max)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->range_max = new_range_max;

		return KrilloudEngine::SetVariableRangeMax(position, new_range_max);
	}
	return false;
}

bool KrilloudProjectHandler::SetVariableOnPlay(int position, bool new_set_on_play)
{
	if (position < project_KVars.size())
	{
		project_KVars[position]->set_only_on_play = new_set_on_play;

		return KrilloudEngine::SetVariableOnPlay(position, new_set_on_play);
	}
	return false;
}

VariableData* KrilloudProjectHandler::GetKVariable(size_t position)
{
	if (position < project_KVars.size()) 
	{
		return project_KVars[position];
	}
	return new VariableData();
}

VariableData* KrilloudProjectHandler::GetKVariable(std::string variable_name)
{
	for (size_t x = 0; x < project_KVars.size(); x++) 
	{
		if (project_KVars[x]->variable_name == variable_name) 
		{
			return project_KVars[x];
		}
	}
	return new VariableData();
}

// -- Ingame object info
std::vector<IngameObject> KrilloudProjectHandler::GetIngameObjectList()
{
	return KrilloudEngine::GetIngameObjectList();
}

std::vector<PlayingTag> KrilloudProjectHandler::GetPlayingTagsList()
{
	return KrilloudEngine::GetPlayingTagsList();
}