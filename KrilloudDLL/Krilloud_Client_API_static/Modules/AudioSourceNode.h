#include "GraphNode.h"
//#include <Windows.h>
//#include <ShObjIdl.h>
//#include <tchar.h>
#include <random>
#include "../AudioPool.h"

class AudioSourceNode : public Node
{
	const char *fileName = "";
	std::wstring file_w_string;
	std::string filepath = "";
	
	int element_to_delete = -1;

	const char * selectionModes[2] = { "Random", "Sequential" };
	int selectionMode = 0;
	int lastSelection = -1;

	std::vector<int> tree_data_ids;
	AudioPool * tree_data_pointer;

	void CheckNodeSize();
	
	std::vector<AudioSourceContent*>* containedIDs;

	bool change_name_mode = false;
	std::string new_name;
public:
	
	AudioSourceNode(int id, KPair pos, AudioPool * tree_pointer, std::string special_name = "");
	~AudioSourceNode();

	int GetSelectionMode();
	void SetSelectionMode(int new_mode);

	void SetLoop(bool loopable);
	bool GetLoopable();

	int RemoveSound(int sound_id);
	int AddSound(int sound_id, std::string file_full_path, int volume);

	std::vector<AudioSourceContent> GetContainedIDs();
	void SetContainedClipVolume(int audioclip_id, float volume);

	virtual TagInfo* DoTheThing();
	
	void AddSelectedTreeItem();
	void AddTreeFolder(directory_system* directory);

	GraphSave SaveNodeInfo();
	void LoadNodeInfo(GraphSave* node_info);
	
};

