#pragma once
#include <sstream>
#include "SaveSystem.h"
#include "NodeGraphSystemLevel.h"
#include "AudioPool.h"

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

//#include <gdiplus.h>
#undef max
#undef min

class KrilloudProjectHandler
{
	std::string krilloud_path;
	std::string project_filename;
	std::string game_path;
	std::string krilloud_project_name;

	AudioPool* treeColumn;

	// --- Engine options
	std::vector<EngineSettings> engine_settings;

	// --- Tag column variables
	struct ProjectTag {
		CompleteKrillTag data;

		NodeGraphSystemLevel* rootLevel;

		NodeGraphSystemLevel* currentLevel;
		NodeGraphSystemLevel* levelRootReader;
	};

	std::vector<ProjectTag> myTags;
	//ProjectTag* currentTag;

	// --- KVars column variables
	std::vector<VariableData*> project_KVars;
	
	const char * KVar_content_type[2] = { "Range", "Raw" };
	const char * KVar_scope[2] = { "Global", "Local" };
	const char * KVar_source[3] = { "Random", "Random no repetition", "InGame" };
	int kvar_scope_selection;
	int kvar_content_selection;
	int kvar_source_selection;

	// -- GraphNode helper
	Node* GetGraphNode(size_t target_tag_position, int node_id);

	// --- Sounbank management
	KrillInfo* ExportAllSoundbanks();
	std::vector<std::string> GetAllFilesInDirectory(directory_system directory);
	std::vector<pool_file_data> GetPoolFileData(std::vector<std::string> full_path_fileNames);
	KrillInfo* ExportSoundbank(const char * soundbank_file_name, std::vector<pool_file_data> files_to_pack);
	void DumpFile(const char* path, int file_id, std::ofstream &ofs, const std::string &outputBankPath, KrillInfo &metadata);
	bool FileExists(const std::string& name);
public:
	// --- Main functions
	KrilloudProjectHandler(std::string krilloud_project_path, std::string game_folder_path, std::string project_name);
	KrilloudProjectHandler(KrilloudProjectInfo loaded_project_info, std::string filename);
	~KrilloudProjectHandler();
	KrilloudProjectInfo load(const char *fileLocation);
	void SaveProject();
	void SaveAsProject(std::string new_file_name);
	void ExportProject(bool also_save);

	// -- Project options
	const char* GetProjectPath();
	bool SetNewProjectPath(std::string new_project_path);
	const char* GetGamePath();
	bool SetNewGamePath(std::string new_game_path);
	const char* GetProjectName();
	bool SetNewProjectName(std::string new_project_name);
	
	std::vector<std::pair<const char*, int>> GetEngineAvailableBackends(SupportedPlatform platform);
	std::vector<std::pair<const char*, int>> GetEngineAvailableBufferSizes(SupportedPlatform platform);
	std::vector<std::pair<const char*, int>> GetEngineAvailableChannels(SupportedPlatform platform);
	std::vector<std::pair<const char*, int>> GetEngineAvailableSampleRates(SupportedPlatform platform);
	bool SetEngineSettings(std::vector<EngineSettings> new_settings);
	std::vector<EngineSettings> GetEngineSettings();

	// -- Selections
	void CopySelection(size_t position, std::vector<int> nodes_selection);
	void CopySelection(std::string tag_name, std::vector<int> nodes_selection);
	void PasteSelection(size_t position, KPair start_position, std::vector<int> nodes_selection);
	void PasteSelection(std::string tag_name, KPair start_position, std::vector<int> nodes_selection);
	void DuplicateSelection(size_t position, KPair start_position, std::vector<int> nodes_selection);
	void DuplicateSelection(std::string tag_name, KPair start_position, std::vector<int> nodes_selection);
	void DeleteSelectedNodes(size_t position, std::vector<int> nodes_selection);
	void DeleteSelectedNodes(std::string tag_name, std::vector<int> nodes_selection);

	KrilloudProjectInfo GetCurrentContractData();

	// -- Channels
	bool CreateNewChannel(std::string channel_name, int parent_id);
	void RemoveChannel(int channel_id);
	Channel* GetChannel(int position);
	Channel* GetChannelByID(int channel_id);
	int GetChannelCount();
	const char* GetChannelName(int position);
	float GetChannelVolume(int position);
	float GetChannelOverallVolume(int position);
	void ChangeChannelParent(int channel_id, int new_parent);
	void ChangeChannelName(int channel_id, std::string new_name);
	void ChangeChannelVolume(int channel_id, float volume);

	// -- Audio Pool
	void RefreshPool();
	size_t GetPoolFileCount();
	size_t GetPoolFoldersCount();
	directory_system* GetPoolTree();
	pool_file_data* GetPoolFileData(std::string file_name);
	pool_file_data* GetPoolFileData(int id);
	pool_file_data* GetFileDataByIndex(int file_index);
	pool_file_data* GetFolderDataByIndex(int file_index);
	int GetPoolItemID(std::string file_name);

	/// --- File functions
	void AddFiles(std::vector<std::string> file_paths, std::string target_directory);
	bool MovePoolFile(std::string source_file, std::string directory_name);
	bool RenamePoolFile(std::string file_name, std::string new_file_name);
	bool RemovePoolFile(std::string file_name, bool update_file_data);

	/// --- Directory functions
	bool CreateNewSubdirectory(std::string path_name);
	bool MovePoolDirectory(std::string source_path, std::string output_path);
	bool DeleteDirectory(std::string path_name);

	// -- Tags
	KrillTag* GetTag(size_t position);
	KrillTag* GetTag(std::string tagname);

	int GetTagIndex(std::string tag_name);
	const char* GetTagName(size_t position);

	size_t GetTagCount();

	bool IsTagPlaying(std::string tagname);

	int PlayTag(size_t position);
	void StopTag(size_t position);

	void CreateNewTag(std::string newName, int tries = 0);
	void CreateNewTag(LevelResult tag_info);
	void CloneTag(int selection);

	int TagExists(std::string tagname);
	bool RenameTag(std::string oldname, std::string newname);
	void RemoveTag(std::string name);
	bool SetTagLoopable(std::string name, bool new_value);
	bool SetTagChannel(std::string name, int new_value);
	bool SetTagAttenuation(std::string tag_name, int attenuation_type);
	bool SetTag3D(std::string tag_name, bool is_tag_3d);
	bool SetTagDistanceDelay(std::string tag_name, bool has_distance_delay);
	bool SetTagDopplerEffect(std::string tag_name, bool is_doppler_active);
	bool SetTagDopplerFactor(std::string tag_name, float doppler_factor);
	bool SetTagMinMaxDistance(std::string tag_name, float min_distance, float max_distance);
	bool SetTagRollOffFactor(std::string tag_name, float roll_off_factor);

	// -- Nodes
	int AddNode(NodeClass node_class, std::string target_tag_name, KPair position);
	int AddNode(NodeClass node_class, size_t target_tag_position, KPair position);
	int AddNodeCopy(size_t target_tag_position, int node_id);
	bool RemoveNode(int iD, std::string target_tag_name);
	bool RemoveNode(int iD, size_t target_tag_position);

	std::vector<int> GetNodesIDList(size_t target_tag_position);

	///GraphNodes
	GraphSave GetNodeData(size_t target_tag_position, int node_id);
	NodeType GetNodeType(size_t target_tag_position, int node_id);
	NodeClass GetNodeClass(size_t target_tag_position, int node_id);
	KPair GetNodeSize(size_t target_tag_position, int node_id);
	void SetNodeSize(size_t target_tag_position, int node_id, KPair new_size);
	KPair GetNodePosition(size_t target_tag_position, int node_id);
	void SetNodePosition(size_t target_tag_position, int node_id, KPair new_position);
	bool CanBeDeleted(size_t target_tag_position, int node_id);

	const char* GetNodeName(size_t target_tag_position, int node_id);
	void SetNodeName(size_t target_tag_position, int node_id, std::string new_name);
	int GetNodeOutputsCount(size_t target_tag_position, int node_id);
	int GetNodeInputsCount(size_t target_tag_position, int node_id);
	bool NodeCanHaveLinks(size_t target_tag_position, int node_id, size_t socketIndex);
	size_t GetNodeSocketLinksCount(size_t target_tag_position, int node_id, size_t socketIndex);
	bool CreateNodeLink(size_t target_tag_position, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex);
	bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket);
	bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, size_t link);
	bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, int link_id);
	bool RemoveAllNodeLinks(size_t target_tag_position, int node_id);
	const char* GetLinkedNodeName(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
	int GetLinkedID(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
	int GetLinkedIDSocket(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
	
	bool CreateNodeSocket(size_t target_tag_position, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks);
	bool DeleteNodeSocket(size_t target_tag_position, int node_id, size_t socket_id);
	int NodeSocketHasConnection(size_t target_tag_position, int node_id, int index);
	int GetNodeSocketCount(size_t target_tag_position, int node_id);

	float GetNodeSocketPosition(size_t target_tag_position, int node_id, int index);
	SocketDataType GetNodeSocketDataType(size_t target_tag_position, int node_id, int index);
	SocketType GetSocketType(size_t target_tag_position, int node_id, int index);
	std::vector<FilterParameter> GetNodeFilterParameters(size_t target_tag_position, int node_id);
	void SetNodeFilterParameter(size_t target_tag_position, int node_id, int parameter_index, float new_value);

	///Mixer specifics
	std::vector<float> GetMixerCurrentVolumes(size_t target_tag_position, int node_id);
	void SetMixerSocketCurrentVolume(size_t target_tag_position, int node_id, int parameter_index, float new_value);

	/// Audio source specifics
	int GetAudioSourceSelectionMode(size_t target_tag_position, int node_id);
	void SetAudioSourceSelectionMode(size_t target_tag_position, int node_id, int new_value);
	void SetAudioContainerLoopable(size_t target_tag_position, int node_id, bool loopable);
	bool GetAudioContainerLoopable(size_t target_tag_position, int node_id);
	int RemoveSound(size_t target_tag_position, int node_id, int sound_id);
	int AddSound(size_t target_tag_position, int node_id, int sound_id, std::string file_full_path, int volume);
	std::vector<AudioSourceContent> GetContainedIDs(size_t target_tag_position, int node_id);
	void SetContainedClipVolume(size_t target_tag_position, int node_id, int audioclip_id, float volume);

	/// Variable node specifics
	void SetSimulatedValue(size_t target_tag_position, int node_id, float newValue);
	void SetOnlyOnPlay(size_t target_tag_position, int node_id, bool newValue);
	void SetVariableSelection(size_t target_tag_position, int node_id, int new_value);
	int GetVariableSelection(size_t target_tag_position, int node_id);
	//void UpdateKVariableData(size_t target_tag_position, int node_id);
	VariableData* GetNodeAssociatedKVar(size_t target_tag_position, int node_id);

	// -- KrillVariables
	size_t GetKVarCount();
	void CreateNewVariable(std::string newName, int tries = 0);
	void CreateVariable(VariableData* data);
	void RemoveVariable(int position);
	const char* GetKVariableName(int variable_position);
	int GetKVariablePosition(std::string variable_name);

	bool SetVariableName(int position, std::string new_name);
	bool SetVariableCurrentValue(int position, float new_value);
	bool SetVariableType(int position, VarType new_type);
	bool SetVariableSource(int position, VarSourceType new_source_type);
	bool SetVariableContent(int position, VarContentType new_content_type);
	bool SetVariableRangeMin(int position, float new_range_min);
	bool SetVariableRangeMax(int position, float new_range_max);
	bool SetVariableOnPlay(int position, bool new_set_on_play);

	VariableData* GetKVariable(size_t position);
	VariableData* GetKVariable(std::string variable_name);

	// -- Real time info
	std::vector<IngameObject> GetIngameObjectList();
	std::vector<PlayingTag> GetPlayingTagsList();
};

