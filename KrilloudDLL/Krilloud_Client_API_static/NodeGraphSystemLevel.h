#pragma once
//#include <math.h>
#include <vector>

#include "GraphNode.h"
#include "ContainerNode.h"
#include "InputNode.h"
#include "OutputNode.h"
#include "AudioSourceNode.h"
#include "VariableSelectorNode.h"
#include "SelectorNode.h"
#include "IngameVariable.h"
#include "MixerNode.h"

#include "Filter_Flanger.h"
#include "Filter_Dc_Remover.h"
#include "Filter_Robotizer.h"
#include "Filter_BQR.h"
#include "Filter_Waveshaper.h"
#include "Filter_LowPass.h"
#include "Filter_HighPass.h"
#include "Filter_BandPass.h"
#include "Filter_Bassboost.h"
#include "Filter_LoFi.h"
#include "Filter_Echo.h"
#include "Filter_Pitch.h"
#include "Filter_Delay.h"
#include "Filter_FreeVerb.h"

#include "AudioPool.h"

//#include "SaveSystem.h"

class NodeGraphSystemLevel
{
	std::vector<Node*> nodes;
	char    LevelName[32];
	
	int gBusHandle;
	
	bool show_grid = true;
	int node_selected = -1;
	
	//std::vector<int> nodes_selection;

	// --- Mouse behaviour variables
	bool mouseLinking = false;
	bool mouseLinkingErase = false;
	Node* startingNode;
	
	bool startingNodeLink_inputType;
	bool operating_inside_grid = false;

	int lastGivenID = 0;
	
	bool mouseDraggingRect = false;
	bool mouseDragging = false;
	bool mouse_dragging_outside = false;

	int node_being_moved = -1;

	// --- Play buttons
	bool play_active = false;

	// --- Shortcuts variables
	bool allow_shorcuts = true;

	bool spacebar_pressed;
	bool del_pressed;
	bool ctrl_pressed;
	bool c_pressed;
	bool v_pressed;
	bool d_pressed;

	bool spacebar_shortcut_ready = true;
	bool delete_shortcut_ready = true;
	bool copyNode_shortcut_ready = true;
	bool pasteNode_shortcut_ready = true;
	bool duplicateNode_shortcut_ready = true;

	

	// --- Node helper functions
	bool AddModuleObject(Node* node);
	bool RemoveModuleObject(Node* node);
	bool RemoveModuleObject(int node_position);
	
public:
	NodeGraphSystemLevel *parent;
	int associatedNode;
	std::string my_associated_tag;
	
	int node_hovered_in_scene = -1;
	std::vector<NodeGraphSystemLevel> newLevelInstances;

	NodeGraphSystemLevel(AudioPool * tree_pointer, std::string associated_tag, std::vector<VariableData*>* Kvars_list, KLevel* level);
	NodeGraphSystemLevel(const char* name, int associatedID, std::string associated_tag, NodeGraphSystemLevel *parentGraph, AudioPool * tree_pointer, std::vector<VariableData*>* Kvars_list, KLevel* level);
	~NodeGraphSystemLevel();
	
	void CleanLevel();

	char *GetName();
	NodeGraphSystemLevel* GetParent();

	bool LinkExists(Node* fromNode, size_t fromSocket, Node* toNode, size_t toSocket);
	void CreateLink(Node* fromNode, size_t fromSocket, Node* toNode, size_t toSocket);
	void DestroyLink(Node* node, size_t socket);
	
	int PlayCurrentTag();
	void StopPlayingTag();

	AudioPool * tree_data_pointer;

	NodeGraphSystemLevel* ReturnNGSL(int index);

	void ChangeAssociatedTag(std::string new_tag_name);
	// ----- Do the thing (GraphLevel edition)

	TagInfo* DoTheThing();

	// ---- GraphInfoSaver ------------------

	virtual LevelResult* SaveGraphLevel();
	virtual void LoadGraphLevel(LevelResult* graphLevel_info);

	// ----KLevel
	KLevel* myKLevel;

	// ---- KVars
	std::vector<VariableData*>* project_Kvars_list;
	
	// --- Copy/Paste functions
	void CopySelection(std::vector<int> nodes_selection);
	void PasteSelection(KPair start_position, std::vector<int> nodes_selection);
	void DuplicateSelection(KPair start_position, std::vector<int> nodes_selection);
	void DeleteSelectedNodes(std::vector<int> nodes_selection);

	// -- Nodes
	int AddNewNode(NodeClass node_class, KPair position, std::string special_name = "");
	int AddNodeCopy(int node_id);
	bool RemoveNode(int iD);
	Node* GetNode(int node_id);

	std::vector<Node*> GetNodeList();
};

