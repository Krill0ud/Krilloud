#include "KrilloudAPIStructures.h"
#define EXPORT extern "C" __declspec(dllexport)

// ProjectManagement
EXPORT KLInfo NewProject(const char* project_path, const char* game_path, const char* project_name);
EXPORT KLInfo OpenProject(const char* load_path);
EXPORT void ExportCurrentProject(bool also_save);
EXPORT void SaveCurrentProject();
EXPORT void SaveCurrentProjectAs(const char* new_file_name);
EXPORT void CloseCurrentProject();
EXPORT bool HasOpenedProject();
EXPORT bool NoOpenedProject();

// ProjectOptions
EXPORT const char* GetProjectPath();
EXPORT bool SetNewProjectPath(const char* new_project_path);
EXPORT const char* GetGamePath();
EXPORT bool SetNewGamePath(const char* new_game_path);
EXPORT const char* GetProjectName();
EXPORT bool SetNewProjectName(const char* new_project_name);
EXPORT KLEngineInfo* GetEngineAvailableBackends(SupportedPlatform platform, int* array_length);
EXPORT KLEngineInfo* GetEngineAvailableBufferSizes(SupportedPlatform platform, int* array_length);
EXPORT KLEngineInfo* GetEngineAvailableChannels(SupportedPlatform platform, int* array_length);
EXPORT KLEngineInfo* GetEngineAvailableSampleRates(SupportedPlatform platform, int* array_length);
EXPORT bool SetEngineSettings(KrilloudEngineSettings* new_settings, int length);
EXPORT KrilloudEngineSettings* GetEngineSettings(int* array_length);


// Channels
EXPORT bool CreateNewChannel(const char* channel_name, int parent_id);
EXPORT void RemoveChannel(int position);
EXPORT KrilloudChannel GetChannel(int position);
EXPORT KrilloudChannel GetChannelByID(int channel_id);
EXPORT int GetChannelCount();
EXPORT const char* GetChannelName(int position);
EXPORT float GetChannelVolume(int position);
EXPORT float GetChannelOverallVolume(int position);
EXPORT void ChangeChannelParent(int channel_id, int new_parent);
EXPORT void ChangeChannelName(int channel_id, const char* new_name);
EXPORT void ChangeChannelVolume(int channel_id, float volume);

// Selections
EXPORT void CopySelection(const char* tag_name, int* nodes_selection, int length);
EXPORT void PasteSelection(const char* tag_name, KLPair start_position, int* nodes_selection, int length);
EXPORT void DuplicateSelection(const char* tag_name, KLPair start_position, int* nodes_selection, int length);
EXPORT void DeleteSelectedNodes(const char* tag_name, int* nodes_selection, int length);

// Tags
EXPORT KrilloudTag GetTag(const char* tagname);

EXPORT int GetTagIndex(const char* tag_name);
EXPORT const char* GetTagName(size_t position);

EXPORT int PlayTag(const char* tag_name);
EXPORT void StopTag(const char* tag_name);
EXPORT void CloneTag(const char* tag_name);

EXPORT size_t GetTagCount();

EXPORT bool IsTagPlaying(const char* tagname);
EXPORT int TagExists(const char* tagname);
EXPORT bool RenameTag(const char* oldname, const char* newname);
EXPORT void CreateNewTag(const char* name);
EXPORT void RemoveTag(const char* name);

EXPORT bool SetTagLoopable(const char* name, bool new_value);
EXPORT bool SetTagChannel(const char* name, int new_value);
EXPORT bool SetTagAttenuation(const char* tag_name, int attenuation_type);
EXPORT bool SetTag3D(const char* tag_name, bool is_tag_3d);
EXPORT bool SetTagDistanceDelay(const char* tag_name, bool has_distance_delay);
EXPORT bool SetTagDopplerEffect(const char* tag_name, bool is_doppler_active);
EXPORT bool SetTagDopplerFactor(const char* tag_name, float doppler_factor);
EXPORT bool SetTagMinMaxDistance(const char* tag_name, float min_distance, float max_distance);
EXPORT bool SetTagRollOffFactor(const char* tag_name, float roll_off_factor);

// Variables
EXPORT size_t GetKVarCount();
EXPORT void CreateNewVariable(const char* newName);
EXPORT void RemoveVariable(int position);
EXPORT const char* GetVariableName(int variable_position);
EXPORT int GetVariablePosition(const char* variable_name);

EXPORT bool SetVariableName(const char* variable_name, const char* new_name);
EXPORT bool SetVariableCurrentValue(const char* variable_name, float new_value);
EXPORT bool SetVariableType(const char* variable_name, VarType new_type);
EXPORT bool SetVariableSource(const char* variable_name, VarSourceType new_source_type);
EXPORT bool SetVariableContent(const char* variable_name, VarContentType new_content_type);
EXPORT bool SetVariableRangeMin(const char* variable_name, float new_range_min);
EXPORT bool SetVariableRangeMax(const char* variable_name, float new_range_max);
EXPORT bool SetVariableOnPlay(const char* variable_name, bool new_set_on_play);
EXPORT KrilloudVariable GetKVariable(const char* variable_name);

// AudioPool
EXPORT void RefreshPool();
EXPORT KrilloudPoolFile* GetPoolTree(int* files_lenght);
EXPORT KrilloudPoolFile* GetPoolTree2(int* files_lenght);
EXPORT size_t GetPoolFileCount();
EXPORT KrilloudPoolFile GetFileData(int file_id);
EXPORT int GetPoolItemID(const char* file_name);
EXPORT void AddFiles(const char** file_paths_list, int length, const char* target_directory);
EXPORT bool MovePoolFile(const char* source_file, const char* directory_name);
EXPORT bool RenamePoolFile(const char* file_name, const char* new_file_name);
EXPORT bool RemovePoolFile(const char* file_name, bool update_file_data);

EXPORT bool CreateNewSubdirectory(const char* path_name);
EXPORT bool MovePoolDirectory(const char* source_path, const char* output_path);
EXPORT bool DeleteDirectory(const char* path_name);


// Nodes
EXPORT int AddNode(NodeClass node_class, const char* target_tag_name, KLPair position);
EXPORT int AddNodeCopy(const char* target_tag_name, int node_id);
EXPORT bool RemoveNode(int iD, const char* target_tag_name);
EXPORT int* __stdcall GetNodesIDList(const char* target_tag_name, int* array_length);

// Graphnodes
EXPORT KrilloudNode GetNodeData(const char* target_tag_name, int node_id);
EXPORT NodeType GetNodeType(const char* target_tag_name, int node_id);
EXPORT NodeClass GetNodeClass(const char* target_tag_name, int node_id);
EXPORT bool CanBeDeleted(const char* target_tag_name, int node_id);
EXPORT KLPair GetNodeSize(const char* target_tag_name, int node_id);
EXPORT void SetNodeSize(const char* target_tag_name, int node_id, KLPair new_size);
EXPORT KLPair GetNodePosition(const char* target_tag_name, int node_id);
EXPORT void SetNodePosition(const char* target_tag_name, int node_id, KLPair new_position);

EXPORT const char* GetNodeName(const char* target_tag_name, int node_id);
EXPORT void SetNodeName(const char* target_tag_name, int node_id, const char* new_name);
EXPORT int GetNodeOutputsCount(const char* target_tag_name, int node_id);
EXPORT int GetNodeInputsCount(const char* target_tag_name, int node_id);


// NodeSockets
EXPORT bool CreateNodeSocket(const char* target_tag_name, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks);
EXPORT bool DeleteNodeSocket(const char* target_tag_name, int node_id, size_t socket_id);
EXPORT int NodeSocketHasConnection(const char* target_tag_name, int node_id, int index);
EXPORT int GetNodeSocketCount(const char* target_tag_name, int node_id);
EXPORT float GetNodeSocketPosition(const char* target_tag_name, int node_id, int index);
EXPORT SocketDataType GetNodeSocketDataType(const char* target_tag_name, int node_id, int index);
EXPORT SocketType GetNodeSocketType(const char* target_tag_name, int node_id, int index);

// NodeSocketLinks
EXPORT bool NodeCanHaveLinks(const char* target_tag_name, int node_id, size_t socketIndex);
EXPORT size_t GetNodeSocketLinksCount(const char* target_tag_name, int node_id, size_t socketIndex);
EXPORT bool CreateNodeLink(const char* target_tag_name, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex);
EXPORT bool RemoveNodeLinks(const char* target_tag_name, int node_id, size_t socket);
EXPORT bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, int link_id);
EXPORT bool RemoveAllNodeLinks(const char* target_tag_name, int node_id);
EXPORT const char* GetLinkedNodeName(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);
EXPORT int GetLinkedID(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);
EXPORT int GetLinkedIDSocket(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);

// AudioContainers
EXPORT int GetAudioSourceSelectionMode(const char* target_tag_name, int node_id);
EXPORT void SetAudioSourceSelectionMode(const char* target_tag_name, int node_id, int new_value);
EXPORT void SetAudioContainerLoopable(const char* target_tag_name, int node_id, bool loopable);
EXPORT bool GetAudioContainerLoopable(const char* target_tag_name, int node_id);
EXPORT int RemoveSound(const char* target_tag_name, int node_id, int sound_id);
EXPORT int AddSound(const char* target_tag_name, int node_id, int sound_id, const char* file_full_path, int volume);
EXPORT KrilloudAudioSource* GetContainedIDs(const char* target_tag_name, int node_id, int* array_length);
EXPORT void SetContainedClipVolume(const char* target_tag_name, int node_id, int audioclip_id, float volume);


// Variables
EXPORT KrilloudVariable GetNodeAssociatedKVar(const char* target_tag_name, int node_id);
EXPORT void SetSimulatedValue(const char* target_tag_name, int node_id, float newValue);
EXPORT void SetOnlyOnPlay(const char* target_tag_name, int node_id, bool newValue);
EXPORT void SetVariableSelection(const char* target_tag_name, int node_id, int new_value);
EXPORT int GetVariableSelection(const char* target_tag_name, int node_id);

// Filters
EXPORT FilterParameter* GetNodeFilterParameters(const char* target_tag_name, int node_id, int* array_length);
EXPORT void SetNodeFilterParameter(const char* target_tag_name, int node_id, int parameter_index, float new_value);

// Mixers
EXPORT float* GetMixerCurrentVolumes(const char* target_tag_name, int node_id, int* array_length);
EXPORT void SetMixerSocketCurrentVolume(const char* target_tag_name, int node_id, int parameter_index, float new_value);

// RealTime
//Server basics
EXPORT KLInfo StartServer();
EXPORT KLInfo StopServer();
EXPORT bool IsServerActive();
EXPORT int GetServerPort();

//Client basics
EXPORT void Connect(const char* ip, const char* port);
EXPORT void Disconnect();
EXPORT bool IsConectionActive();
EXPORT bool ConnectionStablished();

// -- Real time info
EXPORT KrilloudIngameObject* GetIngameObjectList(int* array_length);
EXPORT KrilloudPlayingTag* GetPlayingTagsList(int* array_length);
