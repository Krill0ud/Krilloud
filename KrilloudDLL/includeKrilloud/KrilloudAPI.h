#ifndef KRILLOUD_API_H
#define KRILLOUD_API_H
#include "KrilloudAPIStructures.h"
#include <vector>

struct KrilloudPool
{
	const char* folder_name;
	std::vector<const char*> files;
	std::vector<KrilloudPool> directories;
};

namespace Krilloud 
{
	namespace ProjectManagement 
	{
		KLInfo NewProject(const char* project_path, const char* game_path, const char* project_name);
		KLInfo OpenProject(const char* load_path);
		void ExportCurrentProject(bool also_save);
		void SaveCurrentProject();
		void SaveCurrentProjectAs(const char* new_file_name);
		void CloseCurrentProject();
		bool HasOpenedProject();
		bool NoOpenedProject();
	}
	
	namespace ProjectOptions 
	{
		const char* GetProjectPath();
		bool SetNewProjectPath(const char* new_project_path);
		const char* GetGamePath();
		bool SetNewGamePath(const char* new_game_path);
		const char* GetProjectName();
		bool SetNewProjectName(const char* new_project_name);
		std::vector<KLEngineInfo> GetEngineAvailableBackends(SupportedPlatform platform);
		std::vector<KLEngineInfo> GetEngineAvailableBufferSizes(SupportedPlatform platform);
		std::vector<KLEngineInfo> GetEngineAvailableChannels(SupportedPlatform platform);
		std::vector<KLEngineInfo> GetEngineAvailableSampleRates(SupportedPlatform platform);
		bool SetEngineSettings(std::vector<KrilloudEngineSettings> new_settings);
		std::vector<KrilloudEngineSettings> GetEngineSettings();
	}
	
	namespace Channels 
	{
		bool CreateNewChannel(const char* channel_name, int parent_id);
		void RemoveChannel(int position);
		KrilloudChannel GetChannel(int position);
		KrilloudChannel GetChannelByID(int channel_id);
		int GetChannelCount();
		const char* GetChannelName(int position);
		float GetChannelVolume(int position);
		float GetChannelOverallVolume(int position);
		void ChangeChannelParent(int channel_id, int new_parent);
		void ChangeChannelName(int channel_id, const char* new_name);
		void ChangeChannelVolume(int channel_id, float volume);
	}
	
	namespace Selections 
	{
		void CopySelection(size_t position, std::vector<int> nodes_selection);
		void CopySelection(const char* tag_name, std::vector<int> nodes_selection);
		void PasteSelection(size_t position, KLPair start_position, std::vector<int> nodes_selection);
		void PasteSelection(const char* tag_name, KLPair start_position, std::vector<int> nodes_selection);
		void DuplicateSelection(size_t position, KLPair start_position, std::vector<int> nodes_selection);
		void DuplicateSelection(const char* tag_name, KLPair start_position, std::vector<int> nodes_selection);
		void DeleteSelectedNodes(size_t position, std::vector<int> nodes_selection);
		void DeleteSelectedNodes(const char* tag_name, std::vector<int> nodes_selection);
	}
	
	namespace Tags 
	{
		KrilloudTag GetTag(size_t position);
		KrilloudTag GetTag(const char* tagname);

		int GetTagIndex(const char* tag_name);
		const char* GetTagName(size_t position);

		int PlayTag(size_t position);
		int PlayTag(const char* tag_name);
		void StopTag(size_t position);
		void StopTag(const char* tag_name);
		void CloneTag(size_t position);
		void CloneTag(const char* tag_name);

		size_t GetTagCount();

		bool IsTagPlaying(const char* tagname);
		int TagExists(const char* tagname);
		bool RenameTag(const char* oldname, const char* newname);
		void CreateNewTag(const char* name);
		void RemoveTag(const char* name);


		bool SetTagLoopable(const char* name, bool new_value);
		bool SetTagChannel(const char* name, int new_value);
		bool SetTagAttenuation(const char* tag_name, int attenuation_type);
		bool SetTag3D(const char* tag_name, bool is_tag_3d);
		bool SetTagDistanceDelay(const char* tag_name, bool has_distance_delay);
		bool SetTagDopplerEffect(const char* tag_name, bool is_doppler_active);
		bool SetTagDopplerFactor(const char* tag_name, float doppler_factor);
		bool SetTagMinMaxDistance(const char* tag_name, float min_distance, float max_distance);
		bool SetTagRollOffFactor(const char* tag_name, float roll_off_factor);
	}
	
	namespace Variables 
	{
		size_t GetKVarCount();
		void CreateNewVariable(const char* newName);
		void RemoveVariable(int position);
		const char* GetVariableName(int variable_position);
		int GetVariablePosition(const char* variable_name);

		bool SetVariableName(int position, const char* new_name);
		bool SetVariableName(const char* variable_name, const char* new_name);
		bool SetVariableCurrentValue(int position, float new_value);
		bool SetVariableCurrentValue(const char* variable_name, float new_value);
		bool SetVariableType(int position, VarType new_type);
		bool SetVariableType(const char* variable_name, VarType new_type);
		bool SetVariableSource(int position, VarSourceType new_source_type);
		bool SetVariableSource(const char* variable_name, VarSourceType new_source_type);
		bool SetVariableContent(int position, VarContentType new_content_type);
		bool SetVariableContent(const char* variable_name, VarContentType new_content_type);
		bool SetVariableRangeMin(int position, float new_range_min);
		bool SetVariableRangeMin(const char* variable_name, float new_range_min);
		bool SetVariableRangeMax(int position, float new_range_max);
		bool SetVariableRangeMax(const char* variable_name, float new_range_max);
		bool SetVariableOnPlay(int position, bool new_set_on_play);
		bool SetVariableOnPlay(const char* variable_name, bool new_set_on_play);
		KrilloudVariable GetKVariable(size_t position);
		KrilloudVariable GetKVariable(const char* variable_name);
	}

	namespace AudioPool 
	{
		void RefreshPool();
		KrilloudPool GetPoolTree();
		size_t GetPoolFileCount();
		size_t GetPoolFoldersCount();
		KrilloudPoolFile GetFileData(const char* file_name);
		KrilloudPoolFile GetFileData(int file_id);
		KrilloudPoolFile GetFileDataByIndex(int file_index);
		KrilloudPoolFile GetFolderDataByIndex(int file_index);
		int GetPoolItemID(const char* file_name);
		void AddFiles(std::vector<const char*> file_paths, const char* target_directory);
		bool MovePoolFile(const char* source_file, const char* directory_name);
		bool RenamePoolFile(const char* file_name, const char* new_file_name);
		bool RemovePoolFile(const char* file_name, bool update_file_data);

		bool CreateNewSubdirectory(const char* path_name);
		bool MovePoolDirectory(const char* source_path, const char* output_path);
		bool DeleteDirectory(const char* path_name);
	}

	namespace Nodes 
	{
		int AddNode(NodeClass node_class, const char* target_tag_name, KLPair position);
		int AddNode(NodeClass node_class, size_t target_tag_position, KLPair position);
		int AddNodeCopy(const char* target_tag_name, int node_id);
		int AddNodeCopy(size_t target_tag_position, int node_id);
		bool RemoveNode(int iD, const char* target_tag_name);
		bool RemoveNode(int iD, size_t target_tag_position);

		std::vector<int> GetNodesIDList(size_t target_tag_position);
		std::vector<int> GetNodesIDList(const char* target_tag_name);

		namespace Graphnodes 
		{
			KrilloudNode GetNodeData(const char* target_tag_name, int node_id);
			NodeType GetNodeType(size_t target_tag_position, int node_id);
			NodeType GetNodeType(const char* target_tag_name, int node_id);
			NodeClass GetNodeClass(size_t target_tag_position, int node_id);
			NodeClass GetNodeClass(const char* target_tag_name, int node_id);
			bool CanBeDeleted(size_t target_tag_position, int node_id);
			bool CanBeDeleted(const char* target_tag_name, int node_id);
			KLPair GetNodeSize(size_t target_tag_position, int node_id);
			KLPair GetNodeSize(const char* target_tag_name, int node_id);
			void SetNodeSize(size_t target_tag_position, int node_id, KLPair new_size);
			void SetNodeSize(const char* target_tag_name, int node_id, KLPair new_size);
			KLPair GetNodePosition(size_t target_tag_position, int node_id);
			KLPair GetNodePosition(const char* target_tag_name, int node_id);
			void SetNodePosition(size_t target_tag_position, int node_id, KLPair new_position);
			void SetNodePosition(const char* target_tag_name, int node_id, KLPair new_position);

			const char* GetNodeName(size_t target_tag_position, int node_id);
			const char* GetNodeName(const char* target_tag_name, int node_id);
			void SetNodeName(size_t target_tag_position, int node_id, const char* new_name);
			void SetNodeName(const char* target_tag_name, int node_id, const char* new_name);
			int GetNodeOutputsCount(size_t target_tag_position, int node_id);
			int GetNodeOutputsCount(const char* target_tag_name, int node_id);
			int GetNodeInputsCount(size_t target_tag_position, int node_id);
			int GetNodeInputsCount(const char* target_tag_name, int node_id);
		}

		namespace NodeSockets 
		{
			bool CreateNodeSocket(size_t target_tag_position, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks);
			bool CreateNodeSocket(const char* target_tag_name, int node_id, float position, int socket_id, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks);
			bool DeleteNodeSocket(size_t target_tag_position, int node_id, size_t socket_id);
			bool DeleteNodeSocket(const char* target_tag_name, int node_id, size_t socket_id);
			int NodeSocketHasConnection(size_t target_tag_position, int node_id, int index);
			int NodeSocketHasConnection(const char* target_tag_name, int node_id, int index);
			int GetNodeSocketCount(size_t target_tag_position, int node_id);
			int GetNodeSocketCount(const char* target_tag_name, int node_id);
			float GetNodeSocketPosition(size_t target_tag_position, int node_id, int index);
			float GetNodeSocketPosition(const char* target_tag_name, int node_id, int index);
			SocketDataType GetNodeSocketDataType(size_t target_tag_position, int node_id, int index);
			SocketDataType GetNodeSocketDataType(const char* target_tag_name, int node_id, int index);
			SocketType GetNodeSocketType(size_t target_tag_position, int node_id, int index);
			SocketType GetNodeSocketType(const char* target_tag_name, int node_id, int index);
		}

		namespace NodeSocketLinks 
		{
			bool NodeCanHaveLinks(size_t target_tag_position, int node_id, size_t socketIndex);
			bool NodeCanHaveLinks(const char* target_tag_name, int node_id, size_t socketIndex);
			size_t GetNodeSocketLinksCount(size_t target_tag_position, int node_id, size_t socketIndex);
			size_t GetNodeSocketLinksCount(const char* target_tag_name, int node_id, size_t socketIndex);
			bool CreateNodeLink(size_t target_tag_position, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex);
			bool CreateNodeLink(const char* target_tag_name, int node_id, size_t socketIndex, int nodeToLink_id, size_t linkedNodeSocketIndex);
			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket);
			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket);
			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, size_t link_index);
			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, size_t link_index);
			bool RemoveNodeLink(size_t target_tag_position, int node_id, size_t socket, int link_id);
			bool RemoveNodeLink(const char* target_tag_name, int node_id, size_t socket, int link_id);
			bool RemoveAllNodeLinks(size_t target_tag_position, int node_id);
			bool RemoveAllNodeLinks(const char* target_tag_name, int node_id);
			const char* GetLinkedNodeName(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
			const char* GetLinkedNodeName(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);
			int GetLinkedID(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
			int GetLinkedID(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);
			int GetLinkedIDSocket(size_t target_tag_position, int node_id, size_t socketIndex, size_t link_index);
			int GetLinkedIDSocket(const char* target_tag_name, int node_id, size_t socketIndex, size_t link_index);
		}

		namespace AudioContainers 
		{
			int GetAudioSourceSelectionMode(size_t target_tag_position, int node_id);
			int GetAudioSourceSelectionMode(const char* target_tag_name, int node_id);
			void SetAudioSourceSelectionMode(size_t target_tag_position, int node_id, int new_value);
			void SetAudioSourceSelectionMode(const char* target_tag_name, int node_id, int new_value);
			void SetAudioContainerLoopable(size_t target_tag_position, int node_id, bool loopable);
			void SetAudioContainerLoopable(const char* target_tag_name, int node_id, bool loopable);
			bool GetAudioContainerLoopable(size_t target_tag_position, int node_id);
			bool GetAudioContainerLoopable(const char* target_tag_name, int node_id);
			int RemoveSound(size_t target_tag_position, int node_id, int sound_id);
			int RemoveSound(const char* target_tag_name, int node_id, int sound_id);
			int AddSound(size_t target_tag_position, int node_id, int sound_id, const char* file_full_path, int volume);
			int AddSound(const char* target_tag_name, int node_id, int sound_id, const char* file_full_path, int volume);
			std::vector<KrilloudAudioSource> GetContainedIDs(size_t target_tag_position, int node_id);
			std::vector<KrilloudAudioSource> GetContainedIDs(const char* target_tag_name, int node_id);
			void SetContainedClipVolume(size_t target_tag_position, int node_id, int audioclip_id, float volume);
			void SetContainedClipVolume(const char* target_tag_name, int node_id, int audioclip_id, float volume);
		}

		namespace Variables 
		{
			KrilloudVariable GetNodeAssociatedKVar(size_t target_tag_position, int node_id);
			KrilloudVariable GetNodeAssociatedKVar(const char* target_tag_name, int node_id);
			void SetSimulatedValue(size_t target_tag_position, int node_id, float newValue);
			void SetSimulatedValue(const char* target_tag_name, int node_id, float newValue);
			void SetOnlyOnPlay(size_t target_tag_position, int node_id, bool newValue);
			void SetOnlyOnPlay(const char* target_tag_name, int node_id, bool newValue);
			void SetVariableSelection(size_t target_tag_position, int node_id, int new_value);
			void SetVariableSelection(const char* target_tag_name, int node_id, int new_value);
			int GetVariableSelection(size_t target_tag_position, int node_id);
			int GetVariableSelection(const char* target_tag_name, int node_id);
		}

		namespace Filters 
		{
			std::vector<FilterParameter> GetNodeFilterParameters(size_t target_tag_position, int node_id);
			std::vector<FilterParameter> GetNodeFilterParameters(const char* target_tag_name, int node_id);
			void SetNodeFilterParameter(size_t target_tag_position, int node_id, int parameter_index, float new_value);
			void SetNodeFilterParameter(const char* target_tag_name, int node_id, int parameter_index, float new_value);
		}

		namespace Mixers 
		{
			std::vector<float> GetMixerCurrentVolumes(size_t target_tag_position, int node_id);
			std::vector<float> GetMixerCurrentVolumes(const char* target_tag_name, int node_id);
			void SetMixerSocketCurrentVolume(size_t target_tag_position, int node_id, int parameter_index, float new_value);
			void SetMixerSocketCurrentVolume(const char* target_tag_name, int node_id, int parameter_index, float new_value);
		}
	}

	namespace RealTime 
	{
		//Server basics
		KLInfo StartServer();
		KLInfo StopServer();
		bool IsServerActive();
		int GetServerPort();

		//Client basics
		void Connect(const char* ip, const char* port);
		void Disconnect();
		bool IsConectionActive();
		bool ConnectionStablished();

		// -- Real time info
		std::vector<KrilloudIngameObject> GetIngameObjectList();
		std::vector<KrilloudPlayingTag> GetPlayingTagsList();
	}
}
#endif //KRILLOUD_API_H