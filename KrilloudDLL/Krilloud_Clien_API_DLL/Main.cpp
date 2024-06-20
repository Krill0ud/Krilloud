#include "Main.h"
#include "KrilloudAPI.h"
#include <sstream>

#include <chrono>

int *ret_ext;

namespace Util 
{
	template <typename T>
	std::vector<T> GetVectorFromArray(T* origin_array, int length) 
	{
		std::vector<T> result;

		for (int x = 0; x < length; x++) 
		{
			result.push_back(origin_array[x]);
		}

		return result;
	}

	/*template <typename T>
	void GetArrayFromVector(std::vector<T> origin_vector, T** result, int* length)
	{
		*result = new T[origin_vector.size()];

		for (size_t x = 0; x < origin_vector.size(); x++)
		{
			(*result)[x] = T(origin_vector[x]);
		}

		*length = origin_vector.size();
	}*/
	
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
		

	template <typename T>
	void GetArrayFromVector2(std::vector<T> origin_vector, T** result)
	{
		T res[100];

		for (size_t x = 0; x < origin_vector.size(); x++)
		{
			//memcpy(res[x], new T(origin_vector[x]), sizeof(T*));
			res[x] = T(origin_vector[x]);
		}

		*result = res;
	}

	/*int *createMatrix(int n, int arr[], int size) {
		sp_mat A = zeros<sp_mat>(n, n);
		sp_mat B = zeros<sp_mat>(n, n);
		for (int i = 0; i < size; ++i) {
			A(i, i) = arr[i] + 1;
			B(i, i) = arr[i];
		}
		sp_mat C = A * B;
		cout << C.t() << endl;
		for (int i = 0; i < size; ++i) {
			arr[i] = C(i, i);
		}
		return arr;
	}*/

	
	int* GetArrayFromVector3(std::vector<int> origin_vector, int arr[], int size)
	{
		for (int x = 0; x < size; x++)
		{
			arr[x] = int(origin_vector[x]);
		}

		return arr;
	}
	
	std::vector<const char*> GetInnerTree(KrilloudPool tree)
	{
		std::vector<const char*> result;

		for (size_t x = 0; x < tree.files.size(); x++)
		{
			//std::stringstream ss;

			//ss << tree.folder_name << "\\" << tree.files[x];

			result.push_back(tree.files[x]/*ss.str()*/);
		}

		for (size_t x = 0; x < tree.directories.size(); x++)
		{
			std::vector<const char*> subtree = GetInnerTree(tree.directories[x]);

			for (size_t y = 0; y < subtree.size(); y++)
			{
				result.push_back(subtree[x]);
			}
		}

		return result;
	}
}

extern "C" 
{
	// ProjectManagement
	KLInfo NewProject(const char* project_path, const char* game_path, const char* project_name) 
	{
		return Krilloud::ProjectManagement::NewProject(project_path, game_path, project_name);
	}

	KLInfo OpenProject(const char* load_path)
	{
		return Krilloud::ProjectManagement::OpenProject(load_path);
	}

	void ExportCurrentProject(bool also_save)
	{
		Krilloud::ProjectManagement::ExportCurrentProject(also_save);
	}

	void SaveCurrentProject()
	{
		Krilloud::ProjectManagement::SaveCurrentProject();
	}

	void SaveCurrentProjectAs(const char* new_file_name)
	{
		Krilloud::ProjectManagement::SaveCurrentProjectAs(new_file_name);
	}

	void CloseCurrentProject()
	{
		Krilloud::ProjectManagement::CloseCurrentProject();
	}

	bool HasOpenedProject()
	{
		return Krilloud::ProjectManagement::HasOpenedProject();
	}

	bool NoOpenedProject()
	{
		return Krilloud::ProjectManagement::NoOpenedProject();
	}


	// ProjectOptions
	const char* GetProjectPath()
	{
		return Krilloud::ProjectOptions::GetProjectPath();
	}

	bool SetNewProjectPath(const char* new_project_path)
	{
		return Krilloud::ProjectOptions::SetNewProjectPath(new_project_path);
	}

	const char* GetGamePath()
	{
		return Krilloud::ProjectOptions::GetGamePath();
	}

	bool SetNewGamePath(const char* new_game_path)
	{
		return Krilloud::ProjectOptions::SetNewGamePath(new_game_path);
	}

	const char* GetProjectName()
	{
		return Krilloud::ProjectOptions::GetProjectName();
	}

	bool SetNewProjectName(const char* new_project_name)
	{
		return Krilloud::ProjectOptions::SetNewProjectName(new_project_name);
	}
	
	KLEngineInfo* GetEngineAvailableBackends(SupportedPlatform platform, int* array_length)
	{
		std::vector<KLEngineInfo> result = Krilloud::ProjectOptions::GetEngineAvailableBackends(platform);

		*array_length = result.size();

		KLEngineInfo *ret = Util::GetArrayFromVector<KLEngineInfo>(result, *array_length);

		return ret;
	}
	
	KLEngineInfo* GetEngineAvailableBufferSizes(SupportedPlatform platform, int* array_length)
	{
		std::vector<KLEngineInfo> result = Krilloud::ProjectOptions::GetEngineAvailableBufferSizes(platform);

		*array_length = result.size();

		KLEngineInfo *ret = Util::GetArrayFromVector<KLEngineInfo>(result, *array_length);

		return ret;
	}

	KLEngineInfo* GetEngineAvailableChannels(SupportedPlatform platform, int* array_length)
	{
		std::vector<KLEngineInfo> result = Krilloud::ProjectOptions::GetEngineAvailableChannels(platform);

		*array_length = result.size();

		KLEngineInfo *ret = Util::GetArrayFromVector<KLEngineInfo>(result, *array_length);

		return ret;
	}

	KLEngineInfo* GetEngineAvailableSampleRates(SupportedPlatform platform, int* array_length)
	{
		std::vector<KLEngineInfo> result = Krilloud::ProjectOptions::GetEngineAvailableSampleRates(platform);

		*array_length = result.size();

		KLEngineInfo *ret = Util::GetArrayFromVector<KLEngineInfo>(result, *array_length);

		return ret;
	}

	bool SetEngineSettings(KrilloudEngineSettings* new_settings, int length)
	{
		std::vector<KrilloudEngineSettings> result = Util::GetVectorFromArray<KrilloudEngineSettings>(new_settings, length);

		return Krilloud::ProjectOptions::SetEngineSettings(result);
	}

	KrilloudEngineSettings* GetEngineSettings(int* array_length)
	{
		std::vector<KrilloudEngineSettings> result = Krilloud::ProjectOptions::GetEngineSettings();

		*array_length = result.size();

		KrilloudEngineSettings *ret = Util::GetArrayFromVector<KrilloudEngineSettings>(result, *array_length);

		return ret;
	}


	// Channels
	bool CreateNewChannel(const char* channel_name, int parent_id)
	{
		return Krilloud::Channels::CreateNewChannel(channel_name, parent_id);
	}

	void RemoveChannel(int position)
	{
		Krilloud::Channels::RemoveChannel(position);
	}

	KrilloudChannel GetChannel(int position)
	{
		return Krilloud::Channels::GetChannel(position);
	}

	KrilloudChannel GetChannelByID(int channel_id) 
	{
		return Krilloud::Channels::GetChannelByID(channel_id);
	}

	int GetChannelCount()
	{
		return Krilloud::Channels::GetChannelCount();
	}

	const char* GetChannelName(int position)
	{
		return Krilloud::Channels::GetChannelName(position);
	}

	float GetChannelVolume(int position)
	{
		return Krilloud::Channels::GetChannelVolume(position);
	}

	float GetChannelOverallVolume(int position)
	{
		return Krilloud::Channels::GetChannelOverallVolume(position);
	}

	void ChangeChannelParent(int channel_id, int new_parent)
	{
		Krilloud::Channels::ChangeChannelParent(channel_id, new_parent);
	}

	void ChangeChannelName(int channel_id, const char* new_name)
	{
		Krilloud::Channels::ChangeChannelName(channel_id, new_name);
	}

	void ChangeChannelVolume(int channel_id, float volume)
	{
		Krilloud::Channels::ChangeChannelVolume(channel_id, volume);
	}


	// Selections
	void CopySelection(const char* tag_name, int* nodes_selection, int length)
	{
		std::vector<int> result = Util::GetVectorFromArray<int>(nodes_selection, length);

		Krilloud::Selections::CopySelection(tag_name, result);
	}

	void PasteSelection(const char* tag_name, KLPair start_position, int* nodes_selection, int length)
	{
		std::vector<int> result = Util::GetVectorFromArray<int>(nodes_selection, length);

		Krilloud::Selections::PasteSelection(tag_name, start_position, result);
	}

	void DuplicateSelection(const char* tag_name, KLPair start_position, int* nodes_selection, int length)
	{
		std::vector<int> result = Util::GetVectorFromArray<int>(nodes_selection, length);

		Krilloud::Selections::DuplicateSelection(tag_name, start_position, result);
	}

	void DeleteSelectedNodes(const char* tag_name, int* nodes_selection, int length)
	{
		std::vector<int> result = Util::GetVectorFromArray<int>(nodes_selection, length);

		Krilloud::Selections::DeleteSelectedNodes(tag_name, result);
	}


	// Tags
	KrilloudTag GetTag(const char* tagname)
	{
		return Krilloud::Tags::GetTag(tagname);
	}

	int GetTagIndex(const char* tag_name)
	{
		return Krilloud::Tags::GetTagIndex(tag_name);
	}

	const char* GetTagName(size_t position)
	{
		return Krilloud::Tags::GetTagName(position);
	}

	int PlayTag(const char* tag_name)
	{
		return Krilloud::Tags::PlayTag(tag_name);
	}

	void StopTag(const char* tag_name)
	{
		Krilloud::Tags::StopTag(tag_name);
	}

	void CloneTag(const char* tag_name)
	{
		Krilloud::Tags::CloneTag(tag_name);
	}

	size_t GetTagCount()
	{
		return Krilloud::Tags::GetTagCount();
	}

	bool IsTagPlaying(const char* tagname)
	{
		return Krilloud::Tags::IsTagPlaying(tagname);
	}

	int TagExists(const char* tagname)
	{
		return Krilloud::Tags::TagExists(tagname);
	}

	bool RenameTag(const char* oldname, const char* newname)
	{
		return Krilloud::Tags::RenameTag(oldname, newname);
	}

	void CreateNewTag(const char* name)
	{
		Krilloud::Tags::CreateNewTag(name);
	}

	void RemoveTag(const char* name)
	{
		Krilloud::Tags::RemoveTag(name);
	}

	bool SetTagLoopable(const char* name, bool new_value)
	{
		return Krilloud::Tags::SetTagLoopable(name, new_value);
	}

	bool SetTagChannel(const char* name, int new_value)
	{
		return Krilloud::Tags::SetTagChannel(name, new_value);
	}

	bool SetTagAttenuation(const char* tag_name, int attenuation_type)
	{
		return Krilloud::Tags::SetTagAttenuation(tag_name, attenuation_type);
	}

	bool SetTag3D(const char* tag_name, bool is_tag_3d)
	{
		return Krilloud::Tags::SetTag3D(tag_name, is_tag_3d);
	}

	bool SetTagDistanceDelay(const char* tag_name, bool has_distance_delay)
	{
		return Krilloud::Tags::SetTagDistanceDelay(tag_name, has_distance_delay);
	}

	bool SetTagDopplerEffect(const char* tag_name, bool is_doppler_active)
	{
		return Krilloud::Tags::SetTagDopplerEffect(tag_name, is_doppler_active);
	}

	bool SetTagDopplerFactor(const char* tag_name, float doppler_factor)
	{
		return Krilloud::Tags::SetTagDopplerFactor(tag_name, doppler_factor);
	}

	bool SetTagMinMaxDistance(const char* tag_name, float min_distance, float max_distance)
	{
		return Krilloud::Tags::SetTagMinMaxDistance(tag_name, min_distance, max_distance);
	}

	bool SetTagRollOffFactor(const char* tag_name, float roll_off_factor)
	{
		return Krilloud::Tags::SetTagRollOffFactor(tag_name, roll_off_factor);
	}


	// Variables
	size_t GetKVarCount()
	{
		return Krilloud::Variables::GetKVarCount();
	}

	void CreateNewVariable(const char* newName)
	{
		Krilloud::Variables::CreateNewVariable(newName);
	}

	void RemoveVariable(int position)
	{
		Krilloud::Variables::RemoveVariable(position);
	}

	const char* GetVariableName(int variable_position)
	{
		return Krilloud::Variables::GetVariableName(variable_position);
	}

	int GetVariablePosition(const char* variable_name)
	{
		return Krilloud::Variables::GetVariablePosition( variable_name);
	}

	bool SetVariableName(const char* variable_name, const char* new_name)
	{
		return Krilloud::Variables::SetVariableName(variable_name, new_name);
	}

	bool SetVariableCurrentValue(const char* variable_name, float new_value)
	{
		return Krilloud::Variables::SetVariableCurrentValue(variable_name, new_value);
	}

	bool SetVariableType(const char* variable_name, VarType new_type)
	{
		return Krilloud::Variables::SetVariableType(variable_name, new_type);
	}

	bool SetVariableSource(const char* variable_name, VarSourceType new_source_type)
	{
		return Krilloud::Variables::SetVariableSource(variable_name, new_source_type);
	}

	bool SetVariableContent(const char* variable_name, VarContentType new_content_type)
	{
		return Krilloud::Variables::SetVariableContent(variable_name, new_content_type);
	}

	bool SetVariableRangeMin(const char* variable_name, float new_range_min)
	{
		return Krilloud::Variables::SetVariableRangeMin(variable_name, new_range_min);
	}

	bool SetVariableRangeMax(const char* variable_name, float new_range_max)
	{
		return Krilloud::Variables::SetVariableRangeMax(variable_name, new_range_max);
	}

	bool SetVariableOnPlay(const char* variable_name, bool new_set_on_play)
	{
		return Krilloud::Variables::SetVariableOnPlay(variable_name, new_set_on_play);
	}

	KrilloudVariable GetKVariable(const char* variable_name)
	{
		return Krilloud::Variables::GetKVariable(variable_name);
	}


	// AudioPool
	void RefreshPool()
	{
		Krilloud::AudioPool::RefreshPool();
	}

	/*const char** GetPoolTree(int* files_lenght)
	{
		KrilloudPool ds = Krilloud::AudioPool::GetPoolTree();

		std::vector<const char*> files_tree = Util::GetInnerTree(ds);

		*files_lenght = files_tree.size();

		const char* *ret = Util::GetArrayFromVector<const char*>(files_tree, *files_lenght);

		return ret;
	}*/

	KrilloudPoolFile* GetPoolTree(int* files_lenght)
	{
		*files_lenght = Krilloud::AudioPool::GetPoolFileCount();

		KrilloudPoolFile *ret;
		ret = new KrilloudPoolFile[*files_lenght];

		for (size_t x = 0; x < *files_lenght; x++)
		{
			ret[x] = Krilloud::AudioPool::GetFileDataByIndex(x);
		}
		
		return ret;
	}

	KrilloudPoolFile* GetPoolTree2(int* files_lenght)
	{
		int files = Krilloud::AudioPool::GetPoolFileCount();
		int folders = Krilloud::AudioPool::GetPoolFoldersCount();

		KrilloudPoolFile *ret;

		*files_lenght = files + folders;

		ret = new KrilloudPoolFile[*files_lenght];

		for (size_t x = 0; x < folders; x++)
		{
			ret[x] = Krilloud::AudioPool::GetFolderDataByIndex(x);
		}

		for (size_t x = 0; x < files; x++)
		{
			ret[folders + x] = Krilloud::AudioPool::GetFileDataByIndex(x);
		}
		return ret;
	}


	size_t GetPoolFileCount()
	{
		return Krilloud::AudioPool::GetPoolFileCount();
	}

	KrilloudPoolFile GetFileData(int file_id)
	{
		return Krilloud::AudioPool::GetFileData(file_id);
	}
	
	int GetPoolItemID(const char* file_name)
	{
		return Krilloud::AudioPool::GetPoolItemID(file_name);
	}

	void AddFiles(const char** file_paths_list, int length, const char* target_directory)
	{
		std::vector<const char*> result = Util::GetVectorFromArray<const char*>(file_paths_list, length);

		Krilloud::AudioPool::AddFiles(result, target_directory);
	}

	bool MovePoolFile(const char* source_file, const char* directory_name)
	{
		return Krilloud::AudioPool::MovePoolFile(source_file, directory_name);
	}

	bool RenamePoolFile(const char* file_name, const char* new_file_name)
	{
		return Krilloud::AudioPool::RenamePoolFile(file_name, new_file_name);
	}

	bool RemovePoolFile(const char* file_name, bool update_file_data)
	{
		return Krilloud::AudioPool::RemovePoolFile(file_name, update_file_data);
	}

	bool CreateNewSubdirectory(const char* path_name)
	{
		return Krilloud::AudioPool::CreateNewSubdirectory(path_name);
	}

	bool MovePoolDirectory(const char* source_path, const char* output_path)
	{
		return Krilloud::AudioPool::MovePoolDirectory(source_path, output_path);
	}

	bool DeleteDirectory(const char* path_name)
	{
		return Krilloud::AudioPool::DeleteDirectory(path_name);
	}



	// Nodes
	int AddNode(NodeClass node_class, const char* target_tag_name, KLPair position)
	{
		return Krilloud::Nodes::AddNode(node_class, target_tag_name, position);
	}

	int AddNodeCopy(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::AddNodeCopy(target_tag_name, node_id);
	}

	bool RemoveNode(int iD, const char* target_tag_name)
	{
		return Krilloud::Nodes::RemoveNode(iD, target_tag_name);
	}

	////WINNER!!!!!
	//int* GetNodesIDList(const char* target_tag_name, int* array_length)
	//{
	//	std::vector<int> result = Krilloud::Nodes::GetNodesIDList(target_tag_name);

	//	*array_length = result.size();

	//	int *ret;
	//	ret = new int[*array_length];
	//	for (int i = 0; i < *array_length; i++)
	//		ret[i] = result[i];
	//	return ret;
	//}

	int* __stdcall GetNodesIDList(const char* target_tag_name, int* array_length)
	{
		std::vector<int> result = Krilloud::Nodes::GetNodesIDList(target_tag_name);

		*array_length = result.size();

		ret_ext = Util::GetArrayFromVector<int>(result, *array_length);

		return ret_ext;
	}

	// Graphnodes
	KrilloudNode GetNodeData(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeData(target_tag_name, node_id);
	}

	NodeType GetNodeType(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeType(target_tag_name, node_id);
	}

	NodeClass GetNodeClass(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeClass(target_tag_name, node_id);
	}

	bool CanBeDeleted(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::CanBeDeleted(target_tag_name, node_id);
	}

	KLPair GetNodeSize(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeSize(target_tag_name, node_id);
	}

	void SetNodeSize(const char* target_tag_name, int node_id, KLPair new_size)
	{
		Krilloud::Nodes::Graphnodes::SetNodeSize(target_tag_name, node_id, new_size);
	}

	KLPair GetNodePosition(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodePosition(target_tag_name,  node_id);
	}

	void SetNodePosition(const char* target_tag_name, int node_id, KLPair new_position)
	{
		Krilloud::Nodes::Graphnodes::SetNodePosition(target_tag_name, node_id, new_position);
	}

	const char* GetNodeName(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeName(target_tag_name, node_id);
	}

	void SetNodeName(const char* target_tag_name, int node_id, const char* new_name)
	{
		Krilloud::Nodes::Graphnodes::SetNodeName(target_tag_name, node_id, new_name);
	}

	int GetNodeOutputsCount(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeOutputsCount(target_tag_name, node_id);
	}

	int GetNodeInputsCount(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Graphnodes::GetNodeInputsCount(target_tag_name, node_id);
	}


	// NodeSockets
	bool CreateNodeSocket(const char* target_tag_name, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
	{
		return Krilloud::Nodes::NodeSockets::CreateNodeSocket(target_tag_name, node_id, position, socket_id, sType, sDType, isDeletable, multipleLinks);
	}

	bool DeleteNodeSocket(const char* target_tag_name, int node_id, size_t socket_id)
	{
		return Krilloud::Nodes::NodeSockets::DeleteNodeSocket(target_tag_name, node_id, socket_id);
	}

	int NodeSocketHasConnection(const char* target_tag_name, int node_id, int index)
	{
		return Krilloud::Nodes::NodeSockets::NodeSocketHasConnection(target_tag_name, node_id, index);
	}

	int GetNodeSocketCount(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::NodeSockets::GetNodeSocketCount(target_tag_name, node_id);
	}

	float GetNodeSocketPosition(const char* target_tag_name, int node_id, int index)
	{
		return Krilloud::Nodes::NodeSockets::GetNodeSocketPosition(target_tag_name, node_id, index);
	}

	SocketDataType GetNodeSocketDataType(const char* target_tag_name, int node_id, int index)
	{
		return Krilloud::Nodes::NodeSockets::GetNodeSocketDataType(target_tag_name, node_id, index);
	}

	SocketType GetNodeSocketType(const char* target_tag_name, int node_id, int index)
	{
		return Krilloud::Nodes::NodeSockets::GetNodeSocketType(target_tag_name, node_id, index);
	}


	// NodeSocketLinks
	bool NodeCanHaveLinks(const char* target_tag_name, int node_id, size_t socketIndex)
	{
		return Krilloud::Nodes::NodeSocketLinks::NodeCanHaveLinks(target_tag_name, node_id, socketIndex);
	}

	size_t GetNodeSocketLinksCount(const char* target_tag_name, int node_id, size_t socketIndex)
	{
		return Krilloud::Nodes::NodeSocketLinks::GetNodeSocketLinksCount(target_tag_name, node_id, socketIndex);
	}

	bool CreateNodeLink(const char* target_tag_name, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex)
	{
		return Krilloud::Nodes::NodeSocketLinks::CreateNodeLink(target_tag_name, node_id, socketIndex, nodeToLink_id, linkedNodeSocketIndex);
	}

	bool RemoveNodeLinks(const char* target_tag_name, int node_id, size_t socket)
	{
		return Krilloud::Nodes::NodeSocketLinks::RemoveNodeLink(target_tag_name, node_id, socket);
	}

	bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, int link_id)
	{
		return Krilloud::Nodes::NodeSocketLinks::RemoveNodeLink(target_tag_name, node_id, socket, link_id);
	}

	bool RemoveAllNodeLinks(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::NodeSocketLinks::RemoveAllNodeLinks(target_tag_name, node_id);
	}

	const char* GetLinkedNodeName(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
	{
		return Krilloud::Nodes::NodeSocketLinks::GetLinkedNodeName(target_tag_name, node_id, socketIndex, link_index);
	}

	int GetLinkedID(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
	{
		return Krilloud::Nodes::NodeSocketLinks::GetLinkedID(target_tag_name, node_id, socketIndex, link_index);
	}

	int GetLinkedIDSocket(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index)
	{
		return Krilloud::Nodes::NodeSocketLinks::GetLinkedIDSocket(target_tag_name, node_id, socketIndex, link_index);
	}


	// AudioContainers
	int GetAudioSourceSelectionMode(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::AudioContainers::GetAudioSourceSelectionMode(target_tag_name, node_id);
	}

	void SetAudioSourceSelectionMode(const char* target_tag_name, int node_id, int new_value)
	{
		Krilloud::Nodes::AudioContainers::SetAudioSourceSelectionMode(target_tag_name, node_id, new_value);
	}

	void SetAudioContainerLoopable(const char* target_tag_name, int node_id, bool loopable) 
	{
		Krilloud::Nodes::AudioContainers::SetAudioContainerLoopable(target_tag_name, node_id, loopable);
	}

	bool GetAudioContainerLoopable(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::AudioContainers::GetAudioContainerLoopable(target_tag_name, node_id);
	}

	int RemoveSound(const char* target_tag_name, int node_id, int sound_id)
	{
		return Krilloud::Nodes::AudioContainers::RemoveSound(target_tag_name, node_id, sound_id);
	}

	int AddSound(const char* target_tag_name, int node_id, int sound_id, const char* file_full_path, int volume)
	{
		return Krilloud::Nodes::AudioContainers::AddSound(target_tag_name, node_id, sound_id, file_full_path, volume);
	}

	KrilloudAudioSource* GetContainedIDs(const char* target_tag_name, int node_id, int* array_length)
	{
		std::vector<KrilloudAudioSource> result = Krilloud::Nodes::AudioContainers::GetContainedIDs(target_tag_name, node_id);

		*array_length = result.size();

		KrilloudAudioSource *ret = Util::GetArrayFromVector<KrilloudAudioSource>(result, *array_length);

		return ret;
	}

	void SetContainedClipVolume(const char* target_tag_name, int node_id, int audioclip_id, float volume)
	{
		Krilloud::Nodes::AudioContainers::SetContainedClipVolume(target_tag_name, node_id, audioclip_id, volume);
	}



	// Variables
	KrilloudVariable GetNodeAssociatedKVar(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Variables::GetNodeAssociatedKVar(target_tag_name, node_id);
	}

	void SetSimulatedValue(const char* target_tag_name, int node_id, float newValue)
	{
		Krilloud::Nodes::Variables::SetSimulatedValue(target_tag_name, node_id, newValue);
	}

	void SetOnlyOnPlay(const char* target_tag_name, int node_id, bool newValue)
	{
		Krilloud::Nodes::Variables::SetOnlyOnPlay(target_tag_name, node_id, newValue);
	}

	void SetVariableSelection(const char* target_tag_name, int node_id, int new_value)
	{
		Krilloud::Nodes::Variables::SetVariableSelection(target_tag_name, node_id, new_value);
	}

	int GetVariableSelection(const char* target_tag_name, int node_id)
	{
		return Krilloud::Nodes::Variables::GetVariableSelection(target_tag_name, node_id);
	}


	// Filters
	FilterParameter* GetNodeFilterParameters(const char* target_tag_name, int node_id, int* array_length)
	{
		std::vector<FilterParameter> result = Krilloud::Nodes::Filters::GetNodeFilterParameters(target_tag_name, node_id);
		
		*array_length = result.size();

		FilterParameter *ret = Util::GetArrayFromVector<FilterParameter>(result, *array_length);

		return ret;
	}

	void SetNodeFilterParameter(const char* target_tag_name, int node_id, int parameter_index, float new_value)
	{
		Krilloud::Nodes::Filters::SetNodeFilterParameter(target_tag_name, node_id, parameter_index, new_value);
	}


	// Mixers
	float* GetMixerCurrentVolumes(const char* target_tag_name, int node_id, int* array_length)
	{
		std::vector<float> result = Krilloud::Nodes::Mixers::GetMixerCurrentVolumes(target_tag_name, node_id);

		*array_length = result.size();

		float *ret = Util::GetArrayFromVector<float>(result, *array_length);

		return ret;
	}

	void SetMixerSocketCurrentVolume(const char* target_tag_name, int node_id, int parameter_index, float new_value)
	{
		Krilloud::Nodes::Mixers::SetMixerSocketCurrentVolume(target_tag_name, node_id, parameter_index, new_value);
	}


	// RealTime
	//Server basics
	KLInfo StartServer()
	{
		return Krilloud::RealTime::StartServer();
	}

	KLInfo StopServer()
	{
		return Krilloud::RealTime::StopServer();
	}

	bool IsServerActive()
	{
		return Krilloud::RealTime::IsServerActive();
	}

	int GetServerPort()
	{
		return Krilloud::RealTime::GetServerPort();
	}


	//Client basics
	void Connect(const char* ip, const char* port)
	{
		Krilloud::RealTime::Connect(ip, port);
	}

	void Disconnect()
	{
		Krilloud::RealTime::Disconnect();
	}

	bool IsConectionActive()
	{
		return Krilloud::RealTime::IsConectionActive();
	}

	bool ConnectionStablished()
	{
		return Krilloud::RealTime::ConnectionStablished();
	}


	// -- Real time info
	KrilloudIngameObject* GetIngameObjectList(int* array_length)
	{
		std::vector<KrilloudIngameObject> result = Krilloud::RealTime::GetIngameObjectList();

		*array_length = result.size();

		KrilloudIngameObject *ret = Util::GetArrayFromVector<KrilloudIngameObject>(result, *array_length);

		return ret;
	}

	KrilloudPlayingTag* GetPlayingTagsList(int* array_length)
	{
		std::vector<KrilloudPlayingTag> result = Krilloud::RealTime::GetPlayingTagsList();

		*array_length = result.size();

		KrilloudPlayingTag *ret = Util::GetArrayFromVector<KrilloudPlayingTag>(result, *array_length);

		return ret;
	}

}