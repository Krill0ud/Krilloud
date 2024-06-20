#pragma once
#include "KrilloudAPI.h"
#include "SaveSystem.h"
#include "KrilloudProjectHandler.h"
//#include <direct.h>
#include "NodeInfoStructures.h"
#include "PublicStaticKrilloud.h"
#include "KrilloudSerialized.h"
//#include <filesystem>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include "ArrayVectorOperations.h"

std::vector<KrilloudProjectHandler*> krilloudHandlers;
KrilloudProjectHandler* currentKHandler;
bool connection_stablished = false;
const char* my_text;

struct PoolFileTest {
	const char* name;
};

namespace Util 
{
	bool FileExist(std::string pathname)
	{
		struct stat info;

		if (stat(pathname.c_str(), &info) != 0)
			return false;
		else if (info.st_mode & S_IFDIR)
			return false;
		else
			return true; //name exists but it's not a directory
	}

	bool DirectoryExists(std::string pathname)
	{
		struct stat info;

		if (stat(pathname.c_str(), &info) != 0)
			return false;
		else if (info.st_mode & S_IFDIR)
			return true;
		else
			return false; //name exists but it's not a directory
	}
	
	bool CreateNewSubdirectory(std::string pathname) {

		if (DirectoryExists(pathname)) {
			return false;
		}
		else
		{
			//int result = _mkdir(pathname.c_str());
			bool result = std::experimental::filesystem::create_directory(pathname.c_str());

			return result; // == 0;
		}
	}

	KLInfo CreateKLInfo(bool first_value, const char* second_value)
	{
		KLInfo info;

		info.first = first_value;
		info.second = second_value;

		return info;
	}

	KLEngineInfo CreateKLEngineInfo(const char* first_value, int second_value)
	{
		KLEngineInfo info;

		info.item_name = first_value;
		info.item_id = second_value;

		return info;
	}

	KLPair CreateKLPair(float first_value, float second_value)
	{
		KLPair pair;

		pair.first = first_value;
		pair.second = second_value;

		return pair;
	}

	KLInfo Check(KrilloudProjectInfo info) {

		if (info.project_name.size() <= 0)
		{
			return CreateKLInfo(false, "Please, enter project name");
		}
		else if (info.project_path.length() <= 0)
		{
			return CreateKLInfo(false, "Please, enter project path");
		}
		else if (info.game_path.length() <= 0)
		{
			return CreateKLInfo(false, "Please, enter game path");
		}
		else
		{
			std::stringstream ss;
			ss << info.project_path << "\\" << info.project_name << ".kpj";
			if (FileExist(ss.str()))
			{
				return CreateKLInfo(false, "Project path already used");
			}
		}
		
		return CreateKLInfo(true, "OK");
	}

	void SetEngineSettings(KrilloudProjectInfo* info) 
	{
		info->engine_configurations.push_back(EngineSettings(Windows, DefaultBackend, DefaultSampleRate, DefaultBuffer));
		info->engine_configurations.push_back(EngineSettings(Android, DefaultBackend, DefaultSampleRate, DefaultBuffer));
	}

	template <typename T>
	T* GetArrayFromVector(std::vector<T> origin_vector, int length)
	{
		T *ret;
		ret = new T[length];
		for (int i = 0; i < length; i++)
		{
			ret[i] = T(origin_vector[i]);
		}

		return ret;
	}

	namespace PairConversion 
	{
		std::pair<float, float> GetKPair(KLPair pair)
		{
			return std::pair<float, float>(pair.first, pair.second);
		}

		std::vector<std::pair<float, float>> GetKPairVector(std::vector<KLPair> vector)
		{
			std::vector<std::pair<float, float>> result;
			for (size_t x = 0; x < vector.size(); x++) 
			{
				result.push_back(GetKPair(vector[x]));
			}
			return result;
		}

		KLPair GetKLPair(std::pair<float, float> pair)
		{
			return CreateKLPair(pair.first, pair.second);
		}

		std::vector<KLPair> GetKLPairVector(std::vector<std::pair<float, float>> vector)
		{
			std::vector<KLPair> result;
			for (size_t x = 0; x < vector.size(); x++)
			{
				result.push_back(GetKLPair(vector[x]));
			}
			return result;
		}

		KLInfo GetKLInfo(std::pair<bool, const char*> item) 
		{
			return CreateKLInfo(item.first, item.second);
		}

		KLEngineInfo GetKLEngineInfo(std::pair<const char*, int> pair)
		{
			return CreateKLEngineInfo(pair.first, pair.second);
		}

		std::vector <KLEngineInfo> GetKLEngineInfoVector(std::vector<std::pair<const char*, int>> vector)
		{
			std::vector <KLEngineInfo> result;
			for (size_t x = 0; x < vector.size(); x++)
			{
				result.push_back(GetKLEngineInfo(vector[x]));
			}
			return result;
		}
	}

	namespace ClassConversion 
	{
		KrilloudPlayingTag GetKrilloudPlayingTag(PlayingTag tag) 
		{
			KrilloudPlayingTag new_tag = KrilloudPlayingTag();
			new_tag.id = tag.id;
			new_tag.tagname = tag.tagname.c_str();
			if(tag.parent_object != nullptr)
				new_tag.parent_object_id = tag.parent_object->object_id;
			new_tag.playHandle = tag.playHandle;
			new_tag.channel_id = tag.channel_id;
			new_tag.is3D = tag.is3D;
			new_tag.stop = tag.stop;

			return new_tag;
		}

		KrilloudIngameObject GetKrilloudIngameObject(IngameObject object)
		{
			KrilloudIngameObject new_object = KrilloudIngameObject();
			
			new_object.aVelX = object.aVelX;
			new_object.aVelY = object.aVelY;
			new_object.aVelZ = object.aVelZ;

			new_object.object_id = object.object_id;

			new_object.position_x = object.position_x;
			new_object.position_y = object.position_y;
			new_object.position_z = object.position_z;

			return new_object;
		}

		KrilloudEngineSettings GetEngineSettings_Public(EngineSettings settings) 
		{
			KrilloudEngineSettings new_settings = KrilloudEngineSettings();

			new_settings.backend = settings.backend;
			new_settings.buffer_size = settings.buffer_size;
			new_settings.platform = settings.platform;
			new_settings.sample_rate = settings.sample_rate;

			return new_settings;
		}

		EngineSettings GetEngineSettings_Internal(KrilloudEngineSettings settings)
		{
			EngineSettings new_settings = EngineSettings();

			new_settings.backend = settings.backend;
			new_settings.buffer_size = settings.buffer_size;
			new_settings.platform = settings.platform;
			new_settings.sample_rate = settings.sample_rate;

			return new_settings;
		}

		KrilloudChannel GetKrilloudChannel(Channel* channel) 
		{
			KrilloudChannel new_channel = KrilloudChannel();

			new_channel.channel_id = channel->channel_id;
			new_channel.channel_name = channel->channel_name.c_str();
			new_channel.parent_id = channel->parent_id;
			new_channel.volume = channel->volume;

			return new_channel;
		}

		template <typename T>
		T ToChar(T origin) 
		{
			T new_char;
			new_char = T(origin);

			return new_char;
		}

		KrilloudTag GetKrilloudTag(KrillTag* tag) 
		{
			KrilloudTag new_tag = KrilloudTag();

			new_tag.attenuation = tag->attenuation;
			new_tag.channel_id = tag->channel_id;
			new_tag.distance_delay = tag->distance_delay;
			new_tag.doppler_active = tag->doppler_active;
			new_tag.doppler_factor = tag->doppler_factor;
			new_tag.is3D = tag->is3D;
			new_tag.isLoopable = tag->isLoopable;
			new_tag.max_3D_distance = tag->max_3D_distance;
			new_tag.min_3D_distance = tag->min_3D_distance;
			new_tag.roll_off_factor = tag->roll_off_factor;

			//new_tag.tag_name = tag->tag_name.c_str();
			new_tag.tag_name = ToChar<const char*>(&tag->tag_name[0]);
			//new_tag.tag_name = (const char*)malloc(sizeof(tag->tag_name));
			//new_tag.tag_name = &tag->tag_name[0];

			return new_tag;
		}

		KrilloudVariable GetKrilloudVariable(VariableData* variable) 
		{
			KrilloudVariable new_var = KrilloudVariable();

			new_var.content = variable->content;
			new_var.current_value = variable->current_value;
			new_var.previous_name = variable->previous_name.c_str();
			new_var.range_max = variable->range_max;
			new_var.range_min = variable->range_min;
			new_var.set_only_on_play = variable->set_only_on_play;
			new_var.source = variable->source;
			new_var.type = variable->type;
			new_var.variable_name = variable->variable_name.c_str();
			
			return new_var;
		}

		KrilloudAudioSource GetKrilloudAudioSource(AudioSourceContent source) 
		{
			KrilloudAudioSource new_source = KrilloudAudioSource();

			new_source.filename = source.filename.c_str();
			new_source.id = source.id;
			new_source.volume = source.volume;

			return new_source;
		}

		KrilloudNodeConnection GetKrilloudNodeConnection(ConnectionsSave link) 
		{
			KrilloudNodeConnection new_link = KrilloudNodeConnection();

			new_link.connected_node_id = link.connected_node_id;
			new_link.connected_node_socket_id = link.connected_node_socket_id;
			new_link.my_socket_id = link.my_socket_id;

			return new_link;
		}

		KrilloudNodeSocket GetKrilloudNodeSocket(NodeSocket socket) 
		{
			KrilloudNodeSocket new_nodesocket = KrilloudNodeSocket();

			new_nodesocket.connection_type = socket.connection_type;
			new_nodesocket.data_type = socket.data_type;
			new_nodesocket.socket_id = socket.SocketID;

			std::vector<KrilloudNodeConnection> links;
			for (size_t x = 0; x < socket.links.size(); x++)
			{
				KrilloudNodeConnection new_connection = GetKrilloudNodeConnection(socket.links[x]);
				links.push_back(new_connection);
			}

			new_nodesocket.links_array_length = links.size();

			ArrayVectorOperations::GetKrilloudNodeConnectionArray(links, &new_nodesocket.links, links.size());
			
			return new_nodesocket;
		}

		KrilloudNode GetKrilloudNode(GraphSave node) 
		{
			KrilloudNode new_node = KrilloudNode();

			new_node.can_be_deleted = node.can_be_deleted;
			new_node.id = node.ID;
			new_node.inputs_count = node.inputsCount;
			//new_node.name =  node.Name.c_str();
			
			new_node.name = (const char*)malloc(sizeof(node.Name));
			new_node.name = &node.Name[0];


			new_node.outputs_count = node.outputsCount;
			new_node.pos_x = node.Pos_x;
			new_node.pos_y = node.Pos_y;
			new_node.size_x = node.Size_x;
			new_node.size_y = node.Size_y;
			new_node.type = node.type;

			std::vector<KrilloudNodeSocket> sockets;
			for (size_t x = 0; x < node.sockets.size(); x++) 
			{
				sockets.push_back(GetKrilloudNodeSocket(node.sockets[x]));
			}
			
			new_node.sockets_list_length = sockets.size();
			ArrayVectorOperations::GetKrilloudNodeSocketArray(sockets, &new_node.sockets, new_node.sockets_list_length);
			
			return new_node;
		}

		KrilloudPoolFile GetKrilloudPoolFile(pool_file_data* pfd)
		{
			KrilloudPoolFile new_file = KrilloudPoolFile();
			
			if (pfd == nullptr)
				return new_file;
			
			const char* name = pfd->file_name.c_str();

			auto first = strstr(name,"\/");

			std::vector<const char*> folder_tree;
			while (first != NULL) 
			{
				auto start = first - name + 1;
				first = strstr(&name[start], "\/");

				char* substr = new char[100];

				if (first != NULL) 
				{
					memcpy(substr, &name[start], first - name + 1);
					substr[(first - name) - start] = '\0';
				}
				else
				{
					memcpy(substr, &name[start], pfd->file_name.size() - start);
					substr[pfd->file_name.size() - start] = '\0';
				}

				folder_tree.push_back(substr);
			}

			new_file.folder_tree_length = folder_tree.size() - 1;
			new_file.folder_tree = GetArrayFromVector<const char*>(folder_tree, new_file.folder_tree_length);

			new_file.file_name = folder_tree[new_file.folder_tree_length];
			
			std::vector<std::string> ress;
			for (size_t x = 0; x < new_file.folder_tree_length; x++) 
			{
				ress.push_back(new_file.folder_tree[x]);
			}

			new_file.full_file_name = pfd->file_name.c_str();
			new_file.id = pfd->id;

			return new_file;
		}

		KrilloudPoolFile GetKrilloudPoolFolder(pool_file_data* pfd)
		{
			KrilloudPoolFile new_folder = KrilloudPoolFile();

			if (pfd == nullptr)
				return new_folder;

			const char* name = pfd->file_name.c_str();

			new_folder.folder_tree_length = 0;

			new_folder.full_file_name = pfd->file_name.c_str();
			new_folder.id = pfd->id;

			return new_folder;
		}


		KrilloudPool GetKrilloudPool(directory_system* ds)
		{
			KrilloudPool new_pool = KrilloudPool();

			new_pool.folder_name = ds->folder_name.c_str();

			for (size_t x = 0; x < ds->files.size(); x++) 
			{
				new_pool.files.push_back(ds->files[x].c_str());
			}

			for (size_t x = 0; x < ds->directories.size(); x++)
			{
				new_pool.directories.push_back(GetKrilloudPool(ds->directories[x]));
			}

			return new_pool;
		}
	}

	namespace VectorConversion 
	{
		template <typename T>
		void GetArrayFromVector(std::vector<T> vector, T** result_array, int* length)
		{
			*length = vector.size();

			*result_array = (T*)malloc(sizeof(T) * *length);

			for (size_t x = 0; x < vector.size(); x++)
			{
				*result_array[x] = *vector[x];
			}
		}
	}
}

namespace Krilloud 
{
	namespace ProjectManagement 
	{
		KLInfo NewProject(const char* project_path, const char* game_path, const char* project_name)
		{
			KrilloudProjectInfo info = { project_path, game_path, project_name };
			KLInfo check = Util::Check(info);

			//Util::SetEngineSettings(&info);

			if (check.first)
			{
				CloseCurrentProject();

				std::stringstream subFolderPath;
				subFolderPath << &project_path[0] << "//Sound Files";

				Util::CreateNewSubdirectory(subFolderPath.str());

				currentKHandler = new KrilloudProjectHandler(project_path, game_path, project_name);

				//Tags::CreateNewTag("New Tag");

				SaveCurrentProject();

				//Save(&project_path[0], info);

				std::stringstream full_path;

				full_path << project_path << "\\" << project_name << ".kpj";

				OpenProject(full_path.str().c_str());
			}

			return check;
		}

		KLInfo OpenProject(const char* load_path_cstr)
		{
			std::string load_path(load_path_cstr);
			KrilloudProjectInfo newInfo = Load(load_path);

			if (newInfo.channels.size() <= 0 || newInfo.tags.size() <= 0) 
			{
				KLInfo check = Util::CreateKLInfo(false, "Error while loading project.");
				return check;
			}

			KLInfo check = Util::CreateKLInfo(true, "");

			//ERROR CONTROL?
			CloseCurrentProject();

			int last_bar_position = -1;
			last_bar_position = load_path.find_last_of("\\/");

			std::string project_filename;

			if (last_bar_position > -1)
			{
				std::string current_path = load_path.substr(0, last_bar_position);

				newInfo.project_path = current_path;

				project_filename = load_path.substr(last_bar_position + 1);
			}

			krilloudHandlers.push_back(new KrilloudProjectHandler(newInfo, project_filename.substr(0, project_filename.size() - 4)));
			currentKHandler = krilloudHandlers[krilloudHandlers.size() - 1];

			return check;
		}

		KLInfo OpenProject(KrilloudProjectInfo project_info)
		{
			KLInfo check = Util::CreateKLInfo(false, "Other Project is already opened.");

			if (currentKHandler == nullptr)
			{
				check.first = true;
				check.second = "";

				krilloudHandlers.push_back(new KrilloudProjectHandler(project_info, project_info.project_name));
				currentKHandler = krilloudHandlers[krilloudHandlers.size() - 1];
			}

			return check;
		}

		void ExportCurrentProject(bool also_save)
		{
			if (currentKHandler != nullptr)
			{
				currentKHandler->ExportProject(also_save);
			}
		}

		void SaveCurrentProject()
		{
			if (currentKHandler != nullptr)
			{
				currentKHandler->SaveProject();
			}
		}

		void SaveCurrentProjectAs(const char* new_file_name)
		{
			if (currentKHandler != nullptr)
			{
				currentKHandler->SaveAsProject(new_file_name);
			}
		}

		void CloseCurrentProject()
		{
			if (currentKHandler != nullptr)
			{
				for (size_t x = 0; x < krilloudHandlers.size(); x++)
				{
					if (krilloudHandlers[x] == currentKHandler)
					{
						krilloudHandlers.erase(krilloudHandlers.begin() + x);
						break;
					}
				}

				currentKHandler->~KrilloudProjectHandler();

				if (krilloudHandlers.size() > 0)
				{
					currentKHandler = krilloudHandlers[0];
				}
				else
				{
					currentKHandler = nullptr;
				}

				if (KrilloudRT::IsServerActive())
				{
					KrilloudRT::StopServer();
				}
				if (KrilloudRT::IsConectionActive())
				{
					KrilloudRT::Disconnect();
				}
			}
		}

		bool HasOpenedProject()
		{
			return (currentKHandler != nullptr);
		}

		bool NoOpenedProject()
		{
			return (currentKHandler == nullptr);
		}
	}

	namespace ProjectOptions
	{
		const char* GetProjectPath()
		{
			if (ProjectManagement::NoOpenedProject())
				return "";

			return currentKHandler->GetProjectPath();
		}

		bool SetNewProjectPath(const char* new_project_path)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetNewProjectPath(new_project_path);
		}

		const char* GetGamePath()
		{
			if (ProjectManagement::NoOpenedProject())
				return "";

			return currentKHandler->GetGamePath();
		}

		bool SetNewGamePath(const char* new_game_path)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetNewGamePath(new_game_path);
		}

		const char* GetProjectName()
		{
			if (ProjectManagement::NoOpenedProject())
				return "";

			return currentKHandler->GetProjectName();
		}

		bool SetNewProjectName(const char* new_project_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetNewProjectName(new_project_name);
		}

		std::vector<KLEngineInfo> GetEngineAvailableBackends(SupportedPlatform platform)
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			return Util::PairConversion::GetKLEngineInfoVector(currentKHandler->GetEngineAvailableBackends(platform));
		}

		std::vector<KLEngineInfo> GetEngineAvailableBufferSizes(SupportedPlatform platform)
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			return Util::PairConversion::GetKLEngineInfoVector(currentKHandler->GetEngineAvailableBufferSizes(platform));
		}

		std::vector<KLEngineInfo> GetEngineAvailableChannels(SupportedPlatform platform)
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			return Util::PairConversion::GetKLEngineInfoVector(currentKHandler->GetEngineAvailableChannels(platform));
		}

		std::vector<KLEngineInfo> GetEngineAvailableSampleRates(SupportedPlatform platform)
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			return Util::PairConversion::GetKLEngineInfoVector(currentKHandler->GetEngineAvailableSampleRates(platform));
		}

		bool SetEngineSettings(std::vector<KrilloudEngineSettings> new_settings)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			std::vector<EngineSettings> settings;

			for (size_t x = 0; x < new_settings.size(); x++)
			{
				settings.push_back(Util::ClassConversion::GetEngineSettings_Internal(new_settings[x]));
			}

			return currentKHandler->SetEngineSettings(settings);
		}

		std::vector<KrilloudEngineSettings> GetEngineSettings()
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			std::vector<EngineSettings> new_settings = currentKHandler->GetEngineSettings();
			std::vector<KrilloudEngineSettings> settings;

			for (size_t x = 0; x < new_settings.size(); x++)
			{
				settings.push_back(Util::ClassConversion::GetEngineSettings_Public(new_settings[x]));
			}

			return settings;
		}
	}
	
	namespace Channels
	{
		bool CreateNewChannel(const char* channel_name, int parent_id)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->CreateNewChannel(channel_name, parent_id);
		}

		void RemoveChannel(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->RemoveChannel(position);
		}

		KrilloudChannel GetChannel(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudChannel();

			return Util::ClassConversion::GetKrilloudChannel(currentKHandler->GetChannel(position));
		}

		KrilloudChannel GetChannelByID(int channel_id) 
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudChannel();

			return Util::ClassConversion::GetKrilloudChannel(currentKHandler->GetChannelByID(channel_id));
		}

		int GetChannelCount()
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetChannelCount();
		}

		const char* GetChannelName(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return "Error";
			return currentKHandler->GetChannelName(position);
		}

		float GetChannelVolume(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetChannelVolume(position);
		}

		float GetChannelOverallVolume(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetChannelOverallVolume(position);
		}

		void ChangeChannelParent(int channel_id, int new_parent)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->ChangeChannelParent(channel_id, new_parent);
		}

		void ChangeChannelName(int channel_id, const char* new_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->ChangeChannelName(channel_id, new_name);
		}

		void ChangeChannelVolume(int channel_id, float volume)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->ChangeChannelVolume(channel_id, volume);
		}
	}

	namespace Selections
	{
		void CopySelection(size_t position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->CopySelection(position, nodes_selection);
		}

		void CopySelection(const char* tag_name, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->CopySelection(tag_name, nodes_selection);
		}

		void PasteSelection(size_t position, KLPair start_position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->PasteSelection(position, Util::PairConversion::GetKPair(start_position), nodes_selection);
		}

		void PasteSelection(const char* tag_name, KLPair start_position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->PasteSelection(tag_name, Util::PairConversion::GetKPair(start_position), nodes_selection);
		}

		void DuplicateSelection(size_t position, KLPair start_position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->DuplicateSelection(position, Util::PairConversion::GetKPair(start_position), nodes_selection);
		}

		void DuplicateSelection(const char* tag_name, KLPair start_position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->DuplicateSelection(tag_name, Util::PairConversion::GetKPair(start_position), nodes_selection);
		}

		void DeleteSelectedNodes(size_t position, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->DeleteSelectedNodes(position, nodes_selection);
		}

		void DeleteSelectedNodes(const char* tag_name, std::vector<int> nodes_selection)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->DeleteSelectedNodes(tag_name, nodes_selection);
		}
	}

	namespace Tags
	{
		KrilloudTag GetTag(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudTag();

			KrillTag* tag = currentKHandler->GetTag(position);

			if (tag == nullptr)
				return KrilloudTag();

			return Util::ClassConversion::GetKrilloudTag(tag);
		}

		KrilloudTag GetTag(const char* tagname)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudTag();

			KrillTag* tag = currentKHandler->GetTag(tagname);

			if (tag == nullptr) 
			{
				return KrilloudTag();
			}	

			return Util::ClassConversion::GetKrilloudTag(tag);
		}

		int GetTagIndex(const char* tag_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;

			return currentKHandler->GetTagIndex(tag_name);
		}

		const char* GetTagName(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return "";

			return currentKHandler->GetTagName(position);
		}

		int PlayTag(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return -102;
			return currentKHandler->PlayTag(position);
		}

		int PlayTag(const char* tag_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return -102;
			
			if (GetTagCount() <= 0)
				return -103;
			
			for (size_t x = 0; x < GetTagCount(); x++)
			{
				auto tag = GetTag(x);
				if (strcmp(tag.tag_name, tag_name) == 0)
				{
					return PlayTag(x);
					//break;
				}
			}
			return -104;
		}

		void StopTag(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->StopTag(position);
		}

		void StopTag(const char* tag_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			for (size_t x = 0; x < GetTagCount(); x++)
			{
				if (strcmp(GetTag(x).tag_name, tag_name) == 0)
				{
					StopTag(x);
					break;
				}
			}
		}

		void CloneTag(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->CloneTag(position);
		}

		void CloneTag(const char* tag_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			for (size_t x = 0; x < GetTagCount(); x++)
			{
				if (strcmp(GetTag(x).tag_name, tag_name) == 0)
				{
					CloneTag(x);
					break;
				}
			}
		}

		size_t GetTagCount()
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetTagCount();
		}

		bool IsTagPlaying(const char* tagname)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->IsTagPlaying(tagname);
		}

		int TagExists(const char* tagname)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->TagExists(tagname);
		}

		bool RenameTag(const char* oldname, const char* newname)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->RenameTag(oldname, newname);
		}

		void CreateNewTag(const char* name)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->CreateNewTag(name);
		}

		void RemoveTag(const char* name)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->RemoveTag(name);
		}

		bool SetTagLoopable(const char* name, bool new_value)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagLoopable(name, new_value);
		}

		bool SetTagChannel(const char* name, int new_value)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagChannel(name, new_value);
		}

		bool SetTagAttenuation(const char* tag_name, int attenuation_type)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagAttenuation(tag_name, attenuation_type);
		}

		bool SetTag3D(const char* tag_name, bool is_tag_3d)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTag3D(tag_name, is_tag_3d);
		}

		bool SetTagDistanceDelay(const char* tag_name, bool has_distance_delay)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagDistanceDelay(tag_name, has_distance_delay);
		}

		bool SetTagDopplerEffect(const char* tag_name, bool is_doppler_active)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagDopplerEffect(tag_name, is_doppler_active);
		}

		bool SetTagDopplerFactor(const char* tag_name, float doppler_factor)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagDopplerFactor(tag_name, doppler_factor);
		}

		bool SetTagMinMaxDistance(const char* tag_name, float min_distance, float max_distance)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagMinMaxDistance(tag_name, min_distance, max_distance);
		}

		bool SetTagRollOffFactor(const char* tag_name, float roll_off_factor)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->SetTagRollOffFactor(tag_name, roll_off_factor);
		}
	}
	
	namespace Variables
	{
		size_t GetKVarCount()
		{
			if (ProjectManagement::NoOpenedProject())
				return 0;
			return currentKHandler->GetKVarCount();
		}

		void CreateNewVariable(const char* newName)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->CreateNewVariable(newName);
		}

		void RemoveVariable(int position)
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->RemoveVariable(position);
		}

		const char* GetVariableName(int variable_position)
		{
			if (ProjectManagement::NoOpenedProject())
				return "";

			return currentKHandler->GetKVariableName(variable_position);
		}

		int GetVariablePosition(const char* variable_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;

			return currentKHandler->GetKVariablePosition(variable_name);
		}

		bool SetVariableName(int position, const char* new_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableName(position, new_name);
		}

		bool SetVariableName(const char* variable_name, const char* new_name)
		{
			return SetVariableName(GetVariablePosition(variable_name), new_name);
		}

		bool SetVariableCurrentValue(int position, float new_value)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableCurrentValue(position, new_value);
		}

		bool SetVariableCurrentValue(const char* variable_name, float new_value)
		{
			return SetVariableCurrentValue(GetVariablePosition(variable_name), new_value);
		}

		bool SetVariableType(int position, VarType new_type)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableType(position, new_type);
		}

		bool SetVariableType(const char* variable_name, VarType new_type)
		{
			return SetVariableType(GetVariablePosition(variable_name), new_type);
		}

		bool SetVariableSource(int position, VarSourceType new_source_type)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableSource(position, new_source_type);
		}

		bool SetVariableSource(const char* variable_name, VarSourceType new_source_type)
		{
			return SetVariableSource(GetVariablePosition(variable_name), new_source_type);
		}

		bool SetVariableContent(int position, VarContentType new_content_type)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableContent(position, new_content_type);
		}

		bool SetVariableContent(const char* variable_name, VarContentType new_content_type)
		{
			return SetVariableContent(GetVariablePosition(variable_name), new_content_type);
		}

		bool SetVariableRangeMin(int position, float new_range_min)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableRangeMin(position, new_range_min);
		}

		bool SetVariableRangeMin(const char* variable_name, float new_range_min)
		{
			return SetVariableRangeMin(GetVariablePosition(variable_name), new_range_min);
		}

		bool SetVariableRangeMax(int position, float new_range_max)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableRangeMax(position, new_range_max);
		}

		bool SetVariableRangeMax(const char* variable_name, float new_range_max)
		{
			return SetVariableRangeMax(GetVariablePosition(variable_name), new_range_max);
		}

		bool SetVariableOnPlay(int position, bool new_set_on_play)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;

			return currentKHandler->SetVariableOnPlay(position, new_set_on_play);
		}

		bool SetVariableOnPlay(const char* variable_name, bool new_set_on_play)
		{
			return SetVariableOnPlay(GetVariablePosition(variable_name), new_set_on_play);
		}

		KrilloudVariable GetKVariable(size_t position)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudVariable();

			return Util::ClassConversion::GetKrilloudVariable(currentKHandler->GetKVariable(position));
		}

		KrilloudVariable GetKVariable(const char* variable_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudVariable();

			return Util::ClassConversion::GetKrilloudVariable(currentKHandler->GetKVariable(variable_name));
		}
	}

	namespace AudioPool
	{
		void RefreshPool()
		{
			if (ProjectManagement::NoOpenedProject())
				return;
			currentKHandler->RefreshPool();
		}

		size_t GetPoolFileCount()
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetPoolFileCount();
		}

		size_t GetPoolFoldersCount()
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->GetPoolFoldersCount();
		}


		KrilloudPool GetPoolTree()
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudPool();

			directory_system* ds = currentKHandler->GetPoolTree();
			
			KrilloudPool pool = Util::ClassConversion::GetKrilloudPool(ds);

			return pool;
		}

		KrilloudPoolFile GetFileData(const char* file_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudPoolFile();

			pool_file_data* result = currentKHandler->GetPoolFileData(file_name);

			if (result == nullptr)
				return KrilloudPoolFile();

			return Util::ClassConversion::GetKrilloudPoolFile(result);
		}

		KrilloudPoolFile GetFileData(int file_id)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudPoolFile();

			pool_file_data* result = currentKHandler->GetPoolFileData(file_id);
			if (result == nullptr)
				return KrilloudPoolFile();

			return Util::ClassConversion::GetKrilloudPoolFile(result);
		}

		KrilloudPoolFile GetFileDataByIndex(int file_index)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudPoolFile();

			pool_file_data* result = currentKHandler->GetFileDataByIndex(file_index);
			if (result == nullptr)
				return KrilloudPoolFile();

			return Util::ClassConversion::GetKrilloudPoolFile(result);
		}

		KrilloudPoolFile GetFolderDataByIndex(int file_index)
		{
			if (ProjectManagement::NoOpenedProject())
				return KrilloudPoolFile();

			pool_file_data* result = currentKHandler->GetFolderDataByIndex(file_index);
			if (result == nullptr)
				return KrilloudPoolFile();

			return Util::ClassConversion::GetKrilloudPoolFolder(result);
		}


		int GetPoolItemID(const char* file_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;

			return currentKHandler->GetPoolItemID(file_name);
		}

		void AddFiles(std::vector<const char*> file_paths, const char* target_directory)
		{
			if (ProjectManagement::NoOpenedProject())
				return;

			std::vector<std::string> paths;
			for (size_t x = 0; x < file_paths.size(); x++)
			{
				paths.push_back(file_paths[x]);
			}

			currentKHandler->AddFiles(paths, target_directory);
		}
		bool MovePoolFile(const char* source_file, const char* directory_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->MovePoolFile(source_file, directory_name);
		}
		bool RenamePoolFile(const char* file_name, const char* new_file_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->RenamePoolFile(file_name, new_file_name);
		}
		bool RemovePoolFile(const char* file_name, bool update_file_data)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->RemovePoolFile(file_name, update_file_data);
		}

		bool CreateNewSubdirectory(const char* path_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->CreateNewSubdirectory(path_name);
		}
		bool MovePoolDirectory(const char* source_path, const char* output_path)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->MovePoolDirectory(source_path, output_path);
		}
		bool DeleteDirectory(const char* path_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->DeleteDirectory(path_name);
		}
	}

	namespace Nodes
	{
		int AddNode(NodeClass node_class, const char* target_tag_name, KLPair position)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->AddNode(node_class, target_tag_name, Util::PairConversion::GetKPair(position));
		}

		int AddNode(NodeClass node_class, size_t target_tag_position, KLPair position)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;
			return currentKHandler->AddNode(node_class, target_tag_position, Util::PairConversion::GetKPair(position));
		}

		int AddNodeCopy(const char* target_tag_name, int node_id)
		{
			return AddNodeCopy(Tags::GetTagIndex(target_tag_name), node_id);
		}

		int AddNodeCopy(size_t target_tag_position, int node_id)
		{
			if (ProjectManagement::NoOpenedProject())
				return -1;

			return currentKHandler->AddNodeCopy(target_tag_position, node_id);
		}

		bool RemoveNode(int iD, const char* target_tag_name)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->RemoveNode(iD, target_tag_name);
		}

		bool RemoveNode(int iD, size_t target_tag_position)
		{
			if (ProjectManagement::NoOpenedProject())
				return false;
			return currentKHandler->RemoveNode(iD, target_tag_position);
		}
		
		std::vector<int> GetNodesIDList(size_t target_tag_position)
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			return currentKHandler->GetNodesIDList(target_tag_position);
		}
		
		std::vector<int> GetNodesIDList(const char* target_tag_name)
		{
			return GetNodesIDList(Tags::GetTagIndex(target_tag_name));
		}

		namespace Graphnodes
		{

			KrilloudNode GetNodeData(const char* target_tag_name, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return KrilloudNode();

				return Util::ClassConversion::GetKrilloudNode(currentKHandler->GetNodeData(Tags::GetTagIndex(target_tag_name), node_id));
			}

			NodeType GetNodeType(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return NodeType::Node_Normal;

				return currentKHandler->GetNodeType(target_tag_position, node_id);
			}

			NodeType GetNodeType(const char* target_tag_name, int node_id)
			{
				return GetNodeType(Tags::GetTagIndex(target_tag_name), node_id);
			}

			NodeClass GetNodeClass(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return NodeClass::Class_ERROR;

				return currentKHandler->GetNodeClass(target_tag_position, node_id);
			}

			NodeClass GetNodeClass(const char* target_tag_name, int node_id)
			{
				return GetNodeClass(Tags::GetTagIndex(target_tag_name), node_id);
			}

			bool CanBeDeleted(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->CanBeDeleted(target_tag_position, node_id);
			}

			bool CanBeDeleted(const char* target_tag_name, int node_id)
			{
				return CanBeDeleted(Tags::GetTagIndex(target_tag_name), node_id);
			}

			KLPair GetNodeSize(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return { 0,0 };

				return Util::PairConversion::GetKLPair(currentKHandler->GetNodeSize(target_tag_position, node_id));
			}

			KLPair GetNodeSize(const char* target_tag_name, int node_id)
			{
				return GetNodeSize(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetNodeSize(size_t target_tag_position, int node_id, KLPair new_size)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetNodeSize(target_tag_position, node_id, Util::PairConversion::GetKPair(new_size));
			}

			void SetNodeSize(const char* target_tag_name, int node_id, KLPair new_size)
			{
				SetNodeSize(Tags::GetTagIndex(target_tag_name), node_id, new_size);
			}

			KLPair GetNodePosition(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return { 0,0 };

				return Util::PairConversion::GetKLPair(currentKHandler->GetNodePosition(target_tag_position, node_id));
			}

			KLPair GetNodePosition(const char* target_tag_name, int node_id)
			{
				return GetNodePosition(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetNodePosition(size_t target_tag_position, int node_id, KLPair new_position)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetNodePosition(target_tag_position, node_id, Util::PairConversion::GetKPair(new_position));
			}

			void SetNodePosition(const char* target_tag_name, int node_id, KLPair new_position)
			{
				SetNodePosition(Tags::GetTagIndex(target_tag_name), node_id, new_position);
			}

			const char* GetNodeName(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return "";

				return currentKHandler->GetNodeName(target_tag_position, node_id);
			}

			const char* GetNodeName(const char* target_tag_name, int node_id)
			{
				return GetNodeName(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetNodeName(size_t target_tag_position, int node_id, const char* new_name)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetNodeName(target_tag_position, node_id, new_name);
			}

			void SetNodeName(const char* target_tag_name, int node_id, const char* new_name)
			{
				SetNodeName(Tags::GetTagIndex(target_tag_name), node_id, new_name);
			}

			int GetNodeOutputsCount(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->GetNodeOutputsCount(target_tag_position, node_id);
			}

			int GetNodeOutputsCount(const char* target_tag_name, int node_id)
			{
				return GetNodeOutputsCount(Tags::GetTagIndex(target_tag_name), node_id);
			}

			int GetNodeInputsCount(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->GetNodeInputsCount(target_tag_position, node_id);
			}

			int GetNodeInputsCount(const char* target_tag_name, int node_id)
			{
				return GetNodeInputsCount(Tags::GetTagIndex(target_tag_name), node_id);
			}
		}

		namespace NodeSockets
		{
			bool CreateNodeSocket(size_t target_tag_position, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->CreateNodeSocket(target_tag_position, node_id, position, socket_id, sType, sDType, isDeletable, multipleLinks);
			}

			bool CreateNodeSocket(const char* target_tag_name, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
			{
				return CreateNodeSocket(Tags::GetTagIndex(target_tag_name), node_id, position, socket_id, sType, sDType, isDeletable, multipleLinks);
			}

			bool DeleteNodeSocket(size_t target_tag_position, int node_id, size_t socket_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->DeleteNodeSocket(target_tag_position, node_id, socket_id);
			}

			bool DeleteNodeSocket(const char* target_tag_name, int node_id, size_t socket_id)
			{
				return DeleteNodeSocket(Tags::GetTagIndex(target_tag_name), node_id, socket_id);
			}

			int NodeSocketHasConnection(size_t target_tag_position, int node_id, int index)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->NodeSocketHasConnection(target_tag_position, node_id, index);
			}

			int NodeSocketHasConnection(const char* target_tag_name, int node_id, int index)
			{
				return NodeSocketHasConnection(Tags::GetTagIndex(target_tag_name), node_id, index);
			}

			int GetNodeSocketCount(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return 0;

				return currentKHandler->GetNodeSocketCount(target_tag_position, node_id);
			}

			int GetNodeSocketCount(const char* target_tag_name, int node_id)
			{
				return GetNodeSocketCount(Tags::GetTagIndex(target_tag_name), node_id);
			}

			float GetNodeSocketPosition(size_t target_tag_position, int node_id, int index)
			{
				if (ProjectManagement::NoOpenedProject())
					return 0;

				return currentKHandler->GetNodeSocketPosition(target_tag_position, node_id, index);
			}

			float GetNodeSocketPosition(const char* target_tag_name, int node_id, int index)
			{
				return GetNodeSocketPosition(Tags::GetTagIndex(target_tag_name), node_id, index);
			}

			SocketDataType GetNodeSocketDataType(size_t target_tag_position, int node_id, int index)
			{
				if (ProjectManagement::NoOpenedProject())
					return SocketDataType::Normal;

				return currentKHandler->GetNodeSocketDataType(target_tag_position, node_id, index);
			}

			SocketDataType GetNodeSocketDataType(const char* target_tag_name, int node_id, int index)
			{
				return GetNodeSocketDataType(Tags::GetTagIndex(target_tag_name), node_id, index);
			}

			SocketType GetNodeSocketType(size_t target_tag_position, int node_id, int index)
			{
				if (ProjectManagement::NoOpenedProject())
					return SocketType::Input;

				return currentKHandler->GetSocketType(target_tag_position, node_id, index);
			}

			SocketType GetNodeSocketType(const char* target_tag_name, int node_id, int index)
			{
				return GetNodeSocketType(Tags::GetTagIndex(target_tag_name), node_id, index);
			}
		}

		namespace NodeSocketLinks
		{
			bool NodeCanHaveLinks(size_t target_tag_position, int node_id, size_t socketIndex)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->NodeCanHaveLinks(target_tag_position, node_id, socketIndex);
			}

			bool NodeCanHaveLinks(const char* target_tag_name, int node_id, size_t socketIndex)
			{
				return NodeCanHaveLinks(Tags::GetTagIndex(target_tag_name), node_id, socketIndex);
			}

			size_t GetNodeSocketLinksCount(size_t target_tag_position, int node_id, size_t socketIndex)
			{
				if (ProjectManagement::NoOpenedProject())
					return 0;

				return currentKHandler->GetNodeSocketLinksCount(target_tag_position, node_id, socketIndex);
			}

			size_t GetNodeSocketLinksCount(const char* target_tag_name, int node_id, size_t socketIndex)
			{
				return GetNodeSocketLinksCount(Tags::GetTagIndex(target_tag_name), node_id, socketIndex);
			}

			bool CreateNodeLink(size_t target_tag_position, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->CreateNodeLink(target_tag_position, node_id, socketIndex, nodeToLink_id, linkedNodeSocketIndex);
			}

			bool CreateNodeLink(const char* target_tag_name, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex)
			{
				return CreateNodeLink(Tags::GetTagIndex(target_tag_name), node_id, socketIndex, nodeToLink_id, linkedNodeSocketIndex);
			}

			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->RemoveNodeLink(target_tag_position, node_id, socket);
			}

			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket)
			{
				return RemoveNodeLink(Tags::GetTagIndex(target_tag_name), node_id, socket);
			}

			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, size_t link_index)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->RemoveNodeLink(target_tag_position, node_id, socket, link_index);
			}

			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, size_t link_index)
			{
				return RemoveNodeLink(Tags::GetTagIndex(target_tag_name), node_id, socket, link_index);
			}

			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, int link_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->RemoveNodeLink(target_tag_position, node_id, socket, link_id);
			}

			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, int link_id)
			{
				return RemoveNodeLink(Tags::GetTagIndex(target_tag_name), node_id, socket, link_id);
			}

			bool RemoveAllNodeLinks(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->RemoveAllNodeLinks(target_tag_position, node_id);
			}

			bool RemoveAllNodeLinks(const char* target_tag_name, int node_id)
			{
				return RemoveAllNodeLinks(Tags::GetTagIndex(target_tag_name), node_id);
			}

			const char* GetLinkedNodeName(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
			{
				if (ProjectManagement::NoOpenedProject())
					return "";

				return currentKHandler->GetLinkedNodeName(target_tag_position, node_id, socketIndex, link_index);
			}

			const char* GetLinkedNodeName(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
			{
				return GetLinkedNodeName(Tags::GetTagIndex(target_tag_name), node_id, socketIndex, link_index);
			}

			int GetLinkedID(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->GetLinkedID(target_tag_position, node_id, socketIndex, link_index);
			}

			int GetLinkedID(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
			{
				return GetLinkedID(Tags::GetTagIndex(target_tag_name), node_id, socketIndex, link_index);
			}

			int GetLinkedIDSocket(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->GetLinkedIDSocket(target_tag_position, node_id, socketIndex, link_index);
			}

			int GetLinkedIDSocket(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
			{
				return GetLinkedIDSocket(Tags::GetTagIndex(target_tag_name), node_id, socketIndex, link_index);
			}
		}

		namespace AudioContainers
		{

			int GetAudioSourceSelectionMode(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return 0;

				return currentKHandler->GetAudioSourceSelectionMode(target_tag_position, node_id);
			}

			int GetAudioSourceSelectionMode(const char* target_tag_name, int node_id)
			{
				return GetAudioSourceSelectionMode(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetAudioSourceSelectionMode(size_t target_tag_position, int node_id, int new_value)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetAudioSourceSelectionMode(target_tag_position, node_id, new_value);
			}

			void SetAudioSourceSelectionMode(const char* target_tag_name, int node_id, int new_value)
			{
				SetAudioSourceSelectionMode(Tags::GetTagIndex(target_tag_name), node_id, new_value);
			}

			void SetAudioContainerLoopable(size_t target_tag_position, int node_id, bool loopable)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetAudioContainerLoopable(target_tag_position, node_id, loopable);
			}

			void SetAudioContainerLoopable(const char* target_tag_name, int node_id, bool loopable)
			{
				SetAudioContainerLoopable(Tags::GetTagIndex(target_tag_name), node_id, loopable);
			}

			bool GetAudioContainerLoopable(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return false;

				return currentKHandler->GetAudioContainerLoopable(target_tag_position, node_id);
			}

			bool GetAudioContainerLoopable(const char* target_tag_name, int node_id)
			{
				return GetAudioContainerLoopable(Tags::GetTagIndex(target_tag_name), node_id);
			}

			int RemoveSound(size_t target_tag_position, int node_id, int sound_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->RemoveSound(target_tag_position, node_id, sound_id);
			}

			int RemoveSound(const char* target_tag_name, int node_id, int sound_id)
			{
				return RemoveSound(Tags::GetTagIndex(target_tag_name), node_id, sound_id);
			}

			int AddSound(size_t target_tag_position, int node_id, int sound_id, const char* file_full_path, int volume)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->AddSound(target_tag_position, node_id, sound_id, file_full_path, volume);
			}

			int AddSound(const char* target_tag_name, int node_id, int sound_id, const char* file_full_path, int volume)
			{
				return AddSound(Tags::GetTagIndex(target_tag_name), node_id, sound_id, file_full_path, volume);
			}

			std::vector<KrilloudAudioSource> GetContainedIDs(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return {};

				std::vector<AudioSourceContent> internal_sources = currentKHandler->GetContainedIDs(target_tag_position, node_id);
				std::vector<KrilloudAudioSource> sources;

				for (size_t x = 0; x < internal_sources.size(); x++)
				{
					sources.push_back(Util::ClassConversion::GetKrilloudAudioSource(internal_sources[x]));
				}

				return sources;
			}

			std::vector<KrilloudAudioSource> GetContainedIDs(const char* target_tag_name, int node_id)
			{
				return GetContainedIDs(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetContainedClipVolume(size_t target_tag_position, int node_id, int audioclip_id, float volume)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetContainedClipVolume(target_tag_position, node_id, audioclip_id, volume);
			}

			void SetContainedClipVolume(const char* target_tag_name, int node_id, int audioclip_id, float volume)
			{
				SetContainedClipVolume(Tags::GetTagIndex(target_tag_name), node_id, audioclip_id, volume);
			}
		}

		namespace Variables
		{
			KrilloudVariable GetNodeAssociatedKVar(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return KrilloudVariable();

				return Util::ClassConversion::GetKrilloudVariable(currentKHandler->GetNodeAssociatedKVar(target_tag_position, node_id));
			}

			KrilloudVariable GetNodeAssociatedKVar(const char* target_tag_name, int node_id)
			{
				return GetNodeAssociatedKVar(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetSimulatedValue(size_t target_tag_position, int node_id, float newValue)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetSimulatedValue(target_tag_position, node_id, newValue);
			}

			void SetSimulatedValue(const char* target_tag_name, int node_id, float newValue)
			{
				SetSimulatedValue(Tags::GetTagIndex(target_tag_name), node_id, newValue);
			}

			void SetOnlyOnPlay(size_t target_tag_position, int node_id, bool newValue)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetOnlyOnPlay(target_tag_position, node_id, newValue);
			}

			void SetOnlyOnPlay(const char* target_tag_name, int node_id, bool newValue)
			{
				SetOnlyOnPlay(Tags::GetTagIndex(target_tag_name), node_id, newValue);
			}

			void SetVariableSelection(size_t target_tag_position, int node_id, int new_value)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetVariableSelection(target_tag_position, node_id, new_value);
			}

			void SetVariableSelection(const char* target_tag_name, int node_id, int new_value)
			{
				SetVariableSelection(Tags::GetTagIndex(target_tag_name), node_id, new_value);
			}

			int GetVariableSelection(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return -1;

				return currentKHandler->GetVariableSelection(target_tag_position, node_id);
			}

			int GetVariableSelection(const char* target_tag_name, int node_id)
			{
				return GetVariableSelection(Tags::GetTagIndex(target_tag_name), node_id);
			}
		}

		namespace Filters 
		{
			std::vector<FilterParameter> GetNodeFilterParameters(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return {};

				return currentKHandler->GetNodeFilterParameters(target_tag_position, node_id);
			}

			std::vector<FilterParameter> GetNodeFilterParameters(const char* target_tag_name, int node_id)
			{
				return GetNodeFilterParameters(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetNodeFilterParameter(size_t target_tag_position, int node_id, int parameter_index, float new_value)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetNodeFilterParameter(target_tag_position, node_id, parameter_index, new_value);
			}

			void SetNodeFilterParameter(const char* target_tag_name, int node_id, int parameter_index, float new_value)
			{
				SetNodeFilterParameter(Tags::GetTagIndex(target_tag_name), node_id, parameter_index, new_value);
			}
		}

		namespace Mixers 
		{
			std::vector<float> GetMixerCurrentVolumes(size_t target_tag_position, int node_id)
			{
				if (ProjectManagement::NoOpenedProject())
					return {};

				return currentKHandler->GetMixerCurrentVolumes(target_tag_position, node_id);
			}

			std::vector<float> GetMixerCurrentVolumes(const char* target_tag_name, int node_id)
			{
				return GetMixerCurrentVolumes(Tags::GetTagIndex(target_tag_name), node_id);
			}

			void SetMixerSocketCurrentVolume(size_t target_tag_position, int node_id, int parameter_index, float new_value)
			{
				if (ProjectManagement::NoOpenedProject())
					return;

				currentKHandler->SetMixerSocketCurrentVolume(target_tag_position, node_id, parameter_index, new_value);
			}

			void SetMixerSocketCurrentVolume(const char* target_tag_name, int node_id, int parameter_index, float new_value)
			{
				SetMixerSocketCurrentVolume(Tags::GetTagIndex(target_tag_name), node_id, parameter_index, new_value);
			}
		}
	}

	namespace RealTime
	{
		//Server basics
		KLInfo StartServer() 
		{
			return Util::PairConversion::GetKLInfo(KrilloudRT::StartServer());
		}

		KLInfo StopServer()
		{
			return Util::PairConversion::GetKLInfo(KrilloudRT::StopServer());
		}

		bool SetServerPort(int port) 
		{
			return KrilloudRT::SetServerPort(port);
		}

		bool IsServerActive()
		{
			return KrilloudRT::IsServerActive();
		}

		int GetServerPort() 
		{
			return KrilloudRT::GetServerPort();
		}

		//Client basics
		void Connect(const char* ip, const char* port)
		{
			KrilloudRT::Connect(ip, port);
		}

		void Disconnect()
		{
			KrilloudRT::Disconnect();
		}

		bool IsConectionActive()
		{
			return KrilloudRT::IsConectionActive();
		}

		//INTERNAL
		KrilloudProjectInfo GetContractDataFromServer()
		{
			return KrilloudRT::GetContractData();
		}

		bool ConnectionStablished() 
		{
			if (connection_stablished) 
			{
				return true;
			}
			else
			{
				KrilloudProjectInfo newInfo = Krilloud::RealTime::GetContractDataFromServer();

				if (ProjectManagement::NoOpenedProject())
				{
					if (newInfo.channels.size() > 0)
					{
						KLInfo result = ProjectManagement::OpenProject(newInfo);

						if (result.first)
						{
							connection_stablished = true;
							return true;
						}
						else
						{
							Disconnect();
						}
					}
					return false;
				}
				else
				{
					if (newInfo.channels.size() > 0)
					{
						if (newInfo.project_name == ProjectOptions::GetProjectName())
						{
							connection_stablished = true;
							return true;
						}
						else
						{
							Disconnect();
						}
					}
					return false;
				}
			}
		}

		// -- Real time info
		std::vector<KrilloudIngameObject> GetIngameObjectList()
		{
			if (ProjectManagement::NoOpenedProject())
				return std::vector<KrilloudIngameObject>();

			std::vector<IngameObject> objects = currentKHandler->GetIngameObjectList();
			std::vector<KrilloudIngameObject> result;

			for (size_t x = 0; x < objects.size(); x++)
			{
				result.push_back(Util::ClassConversion::GetKrilloudIngameObject(objects[x]));
			}

			return result;
		}

		std::vector<KrilloudPlayingTag> GetPlayingTagsList()
		{
			if (ProjectManagement::NoOpenedProject())
				return {};

			std::vector<PlayingTag> tags = currentKHandler->GetPlayingTagsList();
			std::vector<KrilloudPlayingTag> result;

			for (size_t x = 0; x < tags.size(); x++)
			{
				result.push_back(Util::ClassConversion::GetKrilloudPlayingTag(tags[x]));
			}

			return result;
		}
	}
}