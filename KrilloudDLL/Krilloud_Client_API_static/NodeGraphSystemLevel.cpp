#include "NodeGraphSystemLevel.h"

extern bool retrigger_active;
extern std::vector<GraphSave> copied_node_data;
extern std::vector<Node*> copied_node_originals;

NodeGraphSystemLevel::NodeGraphSystemLevel(AudioPool * tree_pointer, std::string associated_tag, std::vector<VariableData*>* Kvars_list, KLevel* level)
{
	myKLevel = level; //*KrilloudEngine::CreateKLevel();
	myKLevel->associated_macro_id = -1;
	
	my_associated_tag = associated_tag;

	strncpy_s(LevelName, "Root", 31); LevelName[31] = 0;
	
	AddModuleObject(new OutputNode(this->lastGivenID, KPair(800, 200)));
	
	project_Kvars_list = Kvars_list;

	associatedNode = -1;
	parent = nullptr;
	
	tree_data_pointer = tree_pointer;
}

NodeGraphSystemLevel::NodeGraphSystemLevel(const char* name, int associatedID, std::string associated_tag, NodeGraphSystemLevel *parentGraph, AudioPool * tree_pointer, std::vector<VariableData*>* Kvars_list, KLevel* level)
{
	myKLevel = level; // *KrilloudEngine::CreateKLevel();
	myKLevel->associated_macro_id = associatedID;

	my_associated_tag = associated_tag;

	strncpy_s(LevelName, name, 31); LevelName[31] = 0;
	
	AddModuleObject(new OutputNode(this->lastGivenID, KPair(800, 200)));
	associatedNode = associatedID;
	parent = parentGraph;

	project_Kvars_list = Kvars_list;

	tree_data_pointer = tree_pointer;
}


NodeGraphSystemLevel::~NodeGraphSystemLevel()
{
	//delete myKLevel;
	
	//Remove all objects
	size_t nodesCount = nodes.size();

	for (size_t x = 0; x < nodesCount; x++) 
	{
		RemoveModuleObject(0);
	}
}

void NodeGraphSystemLevel::CleanLevel()
{
	for (int x = 0; x < nodes.size(); x++) 
	{
		nodes[x]->RemoveAllLinks();
	}

	nodes.clear();
}

char * NodeGraphSystemLevel::GetName()
{
	return LevelName;
}

NodeGraphSystemLevel* NodeGraphSystemLevel::GetParent() 
{
	return parent;
}

NodeGraphSystemLevel* NodeGraphSystemLevel::ReturnNGSL(int index)
{
	for (size_t x = 0; x < newLevelInstances.size(); x++) 
	{
		if (newLevelInstances[x].associatedNode == index)
		{
			return &newLevelInstances[x];
		}
	}
	return nullptr;
}

int NodeGraphSystemLevel::PlayCurrentTag() 
{
	int result = -100;
	if (!KrilloudEngine::isTagPlaying(my_associated_tag.c_str(), 0))
	{
		KrilloudEngine::LoadTag(my_associated_tag.c_str(), 0);
		result = KrilloudEngine::PlayTag(my_associated_tag.c_str(), 0);
		play_active = true;
	}
	return result;
}

void NodeGraphSystemLevel::StopPlayingTag() 
{
	KrilloudEngine::StopAllInstancesOfTag(my_associated_tag.c_str(), 0);
	play_active = false;
}

bool NodeGraphSystemLevel::LinkExists(Node* fromNode, size_t fromSocket, Node* toNode, size_t toSocket)
{
	if (fromNode != nullptr && toNode != nullptr && fromNode != toNode)
	{
		if (fromSocket < fromNode->mySockets.size()) 
		{
			for (size_t x = 0; x < fromNode->mySockets[fromSocket].socketLinks.size(); x++)
			{
				if (fromNode->mySockets[fromSocket].socketLinks[x].connectedNode != nullptr) 
				{
					if (fromNode->mySockets[fromSocket].socketLinks[x].connectedNode == toNode) 
					{
						return fromNode->mySockets[fromSocket].socketLinks[x].connectedNodeSocket == toSocket;
					}
				}
			}
		}
	}
	return false;
}

void NodeGraphSystemLevel::CreateLink(Node* fromNode, size_t fromSocket, Node* toNode, size_t toSocket)
{
	if (fromNode != nullptr && toNode != nullptr && fromNode != toNode)
	{
		if (!LinkExists(fromNode, fromSocket, toNode, toSocket)) 
		{
			if (fromNode->CanHaveLinks(fromSocket) && toNode->CanHaveLinks(toSocket))
			{
				if ((fromNode->mySockets[fromSocket].type != toNode->mySockets[toSocket].type) && (fromNode->mySockets[fromSocket].dataType == toNode->mySockets[toSocket].dataType))
				{
					fromNode->CreateLink(fromSocket, toNode, toSocket);
					toNode->CreateLink(toSocket, fromNode, fromSocket);
				}
			}
		}
	}
}

void NodeGraphSystemLevel::DestroyLink(Node* node, size_t socket)
{
	if (node != nullptr) {
		if (socket < node->mySockets.size()) 
		{
			for (size_t x = 0; x < node->mySockets[socket].socketLinks.size(); x++)
			{
				Node* connectedNode = node->mySockets[socket].socketLinks[x].connectedNode;
				int connectedSocket = node->mySockets[socket].socketLinks[x].connectedNodeSocket;

				connectedNode->RemoveLink(connectedSocket, node);
			}
			
			node->RemoveLink(socket);
		}
	}
}

// --- Copy/Paste functions
void NodeGraphSystemLevel::CopySelection(std::vector<int> nodes_selection) 
{
	copied_node_data.clear();
	copied_node_originals.clear();
	if (nodes_selection.size() > 0) 
	{
		for (size_t x = 0; x < nodes_selection.size(); x++) 
		{
			if (nodes_selection[x] < nodes.size() && nodes[nodes_selection[x]]->GetNodeClass() != Class_Output) 
			{
				GraphSave node_save = nodes[nodes_selection[x]]->SaveNodeInfo();

				copied_node_data.push_back(node_save);
				copied_node_originals.push_back(nodes[nodes_selection[x]]);
			}
		}
	}
}

void NodeGraphSystemLevel::PasteSelection(KPair start_position, std::vector<int> nodes_selection)
{
	KPair position_zero = KPair(0, 0);

	std::vector<Node*> instanced_copies;

	if (copied_node_data.size() > 0) 
	{
		nodes_selection.clear();

		for (size_t x = 0; x < copied_node_data.size(); x++) 
		{
			NodeClass this_node_class = (NodeClass)copied_node_data[x].type;
			bool node_instanced = AddNewNode(this_node_class, KPair(0, 0), copied_node_data[x].Name);
			//bool node_instanced = false;

			//switch (this_node_class)
			//{
			//case NodeClass::Class_AudioContainer:
			//	node_instanced = AddModuleObject(new AudioSourceNode(0, KPair(0, 0), tree_data_pointer, copied_node_data[x].Name));
			//	break;
			//case NodeClass::Class_F_BandPass:
			//	node_instanced = AddModuleObject(new Filter_BandPass(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Bassboost:
			//	node_instanced = AddModuleObject(new Filter_Bassboost(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_BQR:
			//	node_instanced = AddModuleObject(new Filter_BQR(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_DC_Remover:
			//	node_instanced = AddModuleObject(new Filter_Delay(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Delay:
			//	node_instanced = AddModuleObject(new Filter_Delay(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Echo:
			//	node_instanced = AddModuleObject(new Filter_Echo(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Flanger:
			//	node_instanced = AddModuleObject(new Filter_Flanger(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_HighPass:
			//	node_instanced = AddModuleObject(new Filter_HighPass(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_LoFi:
			//	node_instanced = AddModuleObject(new Filter_LoFi(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_LowPass:
			//	node_instanced = AddModuleObject(new Filter_LowPass(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Pitch:
			//	node_instanced = AddModuleObject(new Filter_Pitch(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_Robotizer:
			//	node_instanced = AddModuleObject(new Filter_Pitch(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_F_WaveShaper:
			//	node_instanced = AddModuleObject(new Filter_Waveshaper(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_IngameVariable:
			//	node_instanced = AddModuleObject(new IngameVariable(0, KPair(0, 0), project_Kvars_list));
			//	break;
			//case NodeClass::Class_Input:
			//	node_instanced = AddModuleObject(new InputNode(0, KPair(0, 0)));
			//	break;
			//case NodeClass::Class_Macro:
			//{
			//	////////////////////////////////
			//}
			//break;
			//case NodeClass::Class_Output:
			//	//node_instanced = AddModuleObject(new OutputNode(0, KPair(0, 0)));
			//	node_instanced = (nodes.size() == 1);
			//	break;
			//	/*case NodeClass::Class_Randomizer:
			//		node_instanced = AddModuleObject(new Randomizer(0, KPair(0, 0)));
			//		break;*/
			//case NodeClass::Class_Selector:
			//	node_instanced = AddModuleObject(new SelectorNode(0, KPair(0, 0)));
			//	break;
			//}

			if (node_instanced)
			{
				instanced_copies.push_back(nodes[nodes.size() - 1]);
				nodes_selection.push_back(nodes.size() - 1);

				nodes[nodes.size() - 1]->LoadNodeInfo(&copied_node_data[x]);
				//nodes[nodes.size() - 1]->iD = ++this->lastGivenID;


				if (x == 0) 
				{
					position_zero = nodes[nodes.size() - 1]->GetPosition();
					nodes[nodes.size() - 1]->SetPosition(start_position);
				}
				else
				{
					nodes[nodes.size() - 1]->SetPosition((nodes[nodes.size() - 1]->GetPosition() - position_zero) + start_position);
				}
			}
		}
		
		for (size_t x = 0; x < copied_node_originals.size(); x++)
		{
			Node* copy_start = instanced_copies[x];

			for (size_t y = 0; y < copied_node_data[x].sockets.size(); y++)
			{
				for (size_t z = 0; z < copied_node_data[x].sockets[y].links.size(); z++)
				{
					size_t copy_position = -1;
					for (size_t originals = 0; originals < copied_node_originals.size(); originals++)
					{
						if (copied_node_originals[originals]->GetID() == copied_node_data[x].sockets[y].links[z].connected_node_id)
						{
							copy_position = originals;
							break;
						}
					}

					if ((copy_position >= 0) && (copy_position < instanced_copies.size()))
					{
						Node* copy_target = instanced_copies[copy_position];

						CreateLink(copy_start, copied_node_data[x].sockets[y].links[z].my_socket_id, copy_target, copied_node_data[x].sockets[y].links[z].connected_node_socket_id);
					}
				}
			}
		}
	}
}

void NodeGraphSystemLevel::DuplicateSelection(KPair start_position, std::vector<int> nodes_selection)
{
	CopySelection(nodes_selection);
	PasteSelection(start_position, nodes_selection);
}

// --- Node helper functions
bool NodeGraphSystemLevel::AddModuleObject(Node* node)
{
	if (node == nullptr)
		return false;

	nodes.push_back(node);

	KrilloudEngine::AddNode(node->GetKrilloudClass(), myKLevel);

	return true;
}

bool NodeGraphSystemLevel::RemoveModuleObject(Node* node)
{
	if (node != nullptr && node->CanBeDeleted()) 
	{
		int node_position = -1;

		for (int x = 0; x < nodes.size(); x++) 
		{
			if (nodes[x] == node) 
			{
				node_position = x;
			}
		}

		if (node_position > -1) 
		{
			size_t levelInstanceToDelete = -1;

			for (size_t x = 0; x < newLevelInstances.size(); x++)
			{
				if (newLevelInstances[x].associatedNode == node->GetID())
				{
					levelInstanceToDelete = x;
					break;
				}
			}

			if (levelInstanceToDelete > -1)
			{
				//KrilloudST_RemoveKLevel(newLevelInstances[levelInstanceToDelete].myKLevel, myKLevel);
				for (size_t x = 0; x < myKLevel->sublevels.size(); x++)
				{
					if (myKLevel->sublevels[x] == newLevelInstances[levelInstanceToDelete].myKLevel)
					{
						myKLevel->sublevels.erase(myKLevel->sublevels.begin() + x);
						break;
					}
				}


				newLevelInstances[levelInstanceToDelete].CleanLevel();
				newLevelInstances.erase(newLevelInstances.begin() + levelInstanceToDelete);
			}
			node->RemoveAllLinks();

			//KrilloudST_RemoveNode(node->myKClass, myKLevel);
			for (size_t x = 0; x < myKLevel->nodes.size(); x++)
			{
				if (myKLevel->nodes[x] == node->GetKrilloudClass())
				{
					myKLevel->nodes.erase(myKLevel->nodes.begin() + x);
					break;
				}
			}

			nodes.erase(nodes.begin() + node_position);

			return true;
		}
	}
	return false;
}

bool NodeGraphSystemLevel::RemoveModuleObject(int node_position)
{
	if (node_position < nodes.size()) 
	{
		Node* node = nodes[node_position];

		if (node != nullptr && node->CanBeDeleted())
		{
			size_t levelInstanceToDelete = -1;

			for (size_t x = 0; x < newLevelInstances.size(); x++)
			{
				if (newLevelInstances[x].associatedNode == node->GetID())
				{
					levelInstanceToDelete = x;
					break;
				}
			}

			if (levelInstanceToDelete > -1)
			{
				//KrilloudST_RemoveKLevel(newLevelInstances[levelInstanceToDelete].myKLevel, myKLevel);
				for (size_t x = 0; x < myKLevel->sublevels.size(); x++)
				{
					if (myKLevel->sublevels[x] == newLevelInstances[levelInstanceToDelete].myKLevel)
					{
						myKLevel->sublevels.erase(myKLevel->sublevels.begin() + x);
						break;
					}
				}

				newLevelInstances[levelInstanceToDelete].CleanLevel();
				newLevelInstances.erase(newLevelInstances.begin() + levelInstanceToDelete);
			}
			node->RemoveAllLinks();

			//KrilloudST_RemoveNode(node->myKClass, myKLevel);

			nodes.erase(nodes.begin() + node_position);

			return true;
		}
	}
	return false;
}

Node* NodeGraphSystemLevel::GetNode(int node_id)
{
	for (int x = 0; x < nodes.size(); x++)
	{
		if (nodes[x]->GetID() == node_id)
		{
			return nodes[x];
		}
	}
	return nullptr;
}

std::vector<Node*> NodeGraphSystemLevel::GetNodeList()
{
	return nodes;
}

// --- Tag functions

void NodeGraphSystemLevel::ChangeAssociatedTag(std::string new_tag_name)
{
	my_associated_tag = new_tag_name;

	for (size_t x = 0; x < newLevelInstances.size(); x++)
	{
		newLevelInstances[x].ChangeAssociatedTag(new_tag_name);
	}
}

// ----- Do the thing (GraphLevel edition)

TagInfo* NodeGraphSystemLevel::DoTheThing() 
{
	for (int x = 0; x < nodes.size(); x++)
	{
		Node* node = nodes[x];
		OutputNode* outNode = dynamic_cast<OutputNode*>(node);

		if (dynamic_cast<OutputNode*>(node) != nullptr) 
		{
			return outNode->DoTheThing();
		}
	}
	return nullptr;
}

// ---- Graph Info Saver -------------------------------

LevelResult* NodeGraphSystemLevel::SaveGraphLevel()
{
	LevelResult* myGraphInfo = new LevelResult();

	myGraphInfo->tagname = my_associated_tag;

	myGraphInfo->levelName = LevelName;

	myGraphInfo->associated_ID = associatedNode;
		
	myGraphInfo->gids = lastGivenID;

	for (int x = 0; x < nodes.size(); x++)
	{
		myGraphInfo->graphNodes_info.push_back(nodes[x]->SaveNodeInfo());
	}

	for (size_t x = 0; x < newLevelInstances.size(); x++)
	{
		myGraphInfo->graphLevels_info.push_back(*newLevelInstances[x].SaveGraphLevel());
	}

	return myGraphInfo;
}

void NodeGraphSystemLevel::LoadGraphLevel(LevelResult* graphLevel_info)
{
	//lastGivenID = graphLevel_info->gids;

	//load nodes
	if (nodes.size() >= 1) 
	{
		nodes[0]->LoadNodeInfo(&graphLevel_info->graphNodes_info[0]);
	}

	for (size_t x = 1; x < graphLevel_info->graphNodes_info.size(); x++)
	{
//		graphLevel_info->graphNodes_info[x].
		
		NodeClass this_node_class = (NodeClass)graphLevel_info->graphNodes_info[x].type;
		bool node_instanced = AddNewNode(this_node_class, KPair(0, 0), graphLevel_info->graphNodes_info[x].Name);
		
		if (node_instanced) 
		{
			nodes[nodes.size() - 1]->LoadNodeInfo(&graphLevel_info->graphNodes_info[x]);
		}
	}

	lastGivenID = graphLevel_info->gids;

	//load links
	for (size_t x = 0; x < graphLevel_info->graphNodes_info.size(); x++)
	{
		Node* this_node = GetNode(graphLevel_info->graphNodes_info[x].ID);

		if (this_node != nullptr) 
		{
			for (size_t y = 0; y < graphLevel_info->graphNodes_info[x].sockets.size(); y++)
			{
				for (size_t z = 0; z < graphLevel_info->graphNodes_info[x].sockets[y].links.size(); z++)
				{
					Node* connected_node = GetNode(graphLevel_info->graphNodes_info[x].sockets[y].links[z].connected_node_id);

					if (connected_node != nullptr)
					{
						int my_socket_position = 0;

						for (size_t s = 0; s < this_node->mySockets.size(); s++) 
						{
							if (this_node->mySockets[s].socketID == graphLevel_info->graphNodes_info[x].sockets[y].links[z].my_socket_id) 
							{
								my_socket_position = s;
								break;
							}
						}

						CreateLink(this_node, my_socket_position, connected_node, graphLevel_info->graphNodes_info[x].sockets[y].links[z].connected_node_socket_id);
					}
				}
			}
		}
	}

	//load sublevels
	/*for (size_t x = 0; x < graphLevel_info->graphLevels_info.size(); x++)
	{
		NodeGraphSystemLevel newGraphSystemLevel(graphLevel_info->graphLevels_info[x].levelName.c_str(), graphLevel_info->graphLevels_info[x].associated_ID, my_associated_tag, this, tree_data_pointer, project_Kvars_list);
		newGraphSystemLevel.LoadGraphLevel(&graphLevel_info->graphLevels_info[x]);

		newLevelInstances.push_back(newGraphSystemLevel);
	}*/
}


void NodeGraphSystemLevel::DeleteSelectedNodes(std::vector<int> nodes_selection)
{
	if (nodes_selection.size() > 0) 
	{
		std::vector<Node*> nodes_to_delete;

		for (size_t x = 0; x < nodes_selection.size(); x++)
		{
			if (nodes_selection[x] < nodes.size()) 
			{
				nodes_to_delete.push_back(nodes[nodes_selection[x]]);
			}
		}

		for (size_t x = 0; x < nodes_to_delete.size(); x++) 
		{
			if (nodes_to_delete[x] && nodes_to_delete[x]->can_be_deleted) 
			{
				RemoveModuleObject(nodes_to_delete[x]);
			}
		}

		nodes_selection.clear();
	}
}

// -- Nodes
int NodeGraphSystemLevel::AddNodeCopy(int node_id) 
{
	if (node_id <= 0)
		return -1;

	for (size_t x = 0; x < nodes.size(); x++) 
	{
		if (nodes[x]->GetID() == node_id) 
		{
			Node* original = nodes[x];
			int new_node_id = AddNewNode(original->GetNodeClass(), { 0, 0 });

			if (new_node_id >= 0) 
			{
				GraphSave tmp = original->SaveNodeInfo();
				nodes[nodes.size() - 1]->LoadNodeInfo(&tmp);
				nodes[nodes.size() - 1]->SetID(new_node_id);

				return new_node_id;
			}
			else 
			{
				return -1;
			}
		}
	}
	return -1;
}

int NodeGraphSystemLevel::AddNewNode(NodeClass node_class, KPair position, std::string special_name)
{
	bool node_instanced = false;
	
	int id = ++lastGivenID;

	switch (node_class)
	{
	case NodeClass::Class_AudioContainer:
		node_instanced = AddModuleObject(new AudioSourceNode(id, position, tree_data_pointer, special_name));
		break;
	case NodeClass::Class_F_BandPass:
		node_instanced = AddModuleObject(new Filter_BandPass(id, position));
		break;
	case NodeClass::Class_F_Bassboost:
		node_instanced = AddModuleObject(new Filter_Bassboost(id, position));
		break;
	case NodeClass::Class_F_BQR:
		node_instanced = AddModuleObject(new Filter_BQR(id, position));
		break;
	case NodeClass::Class_F_DC_Remover:
		node_instanced = AddModuleObject(new Filter_Dc_Remover(id, position));
		break;
	case NodeClass::Class_F_Delay:
		node_instanced = AddModuleObject(new Filter_Delay(id, position));
		break;
	case NodeClass::Class_F_Echo:
		node_instanced = AddModuleObject(new Filter_Echo(id, position));
		break;
	case NodeClass::Class_F_Flanger:
		node_instanced = AddModuleObject(new Filter_Flanger(id, position));
		break;
	case NodeClass::Class_F_HighPass:
		node_instanced = AddModuleObject(new Filter_HighPass(id, position));
		break;
	case NodeClass::Class_F_LoFi:
		node_instanced = AddModuleObject(new Filter_LoFi(id, position));
		break;
	case NodeClass::Class_F_LowPass:
		node_instanced = AddModuleObject(new Filter_LowPass(id, position));
		break;
	case NodeClass::Class_F_Pitch:
		node_instanced = AddModuleObject(new Filter_Pitch(id, position));
		break;
	case NodeClass::Class_F_Robotizer:
		node_instanced = AddModuleObject(new Filter_Robotizer(id, position));
		break;
	case NodeClass::Class_F_WaveShaper:
		node_instanced = AddModuleObject(new Filter_Waveshaper(id, position));
		break;
	case NodeClass::Class_F_Freeverb:
		node_instanced = AddModuleObject(new Filter_FreeVerb(id, position));
		break;
	case NodeClass::Class_IngameVariable:
		node_instanced = AddModuleObject(new IngameVariable(id, position, project_Kvars_list));
		break;
	case NodeClass::Class_Input:
		node_instanced = AddModuleObject(new InputNode(id, position));
		break;
	case NodeClass::Class_Macro:
	{
		node_instanced = AddModuleObject(new ContainerNode(id, position,
			new NodeGraphSystemLevel("Macro", id, my_associated_tag, this, tree_data_pointer, project_Kvars_list, KrilloudEngine::CreateKLevel())));
		/*int macro_id = graphLevel_info->graphNodes_info[x].ID;

		for (size_t x = 0; x < graphLevel_info->graphLevels_info.size(); x++)
		{
			if (graphLevel_info->graphLevels_info[x].associated_ID == macro_id)
			{
				NodeGraphSystemLevel newGraphSystemLevel(graphLevel_info->graphLevels_info[x].levelName.c_str(), graphLevel_info->graphLevels_info[x].associated_ID, my_associated_tag, this, tree_data_pointer, project_Kvars_list, myKLevel);
				newGraphSystemLevel.LoadGraphLevel(&graphLevel_info->graphLevels_info[x]);

				newLevelInstances.push_back(newGraphSystemLevel);

				ContainerNode *newNode = new ContainerNode(0, KPair(0, 0), &newLevelInstances[newLevelInstances.size() - 1]);

				AddModuleObject(newNode);
			}
		}*/
	}
	break;
	case NodeClass::Class_Mixer:
		node_instanced = AddModuleObject(new MixerNode(id, position));
		break;
	case NodeClass::Class_Output:
		//node_instanced = AddModuleObject(new OutputNode(0, KPair(0, 0)));
		node_instanced = (nodes.size() == 1);
		break;
	case NodeClass::Class_Selector:
		node_instanced = AddModuleObject(new SelectorNode(id, position));
		break;
	}

	return (node_instanced ? id : -1);
}

bool NodeGraphSystemLevel::RemoveNode(int iD)
{
	for (size_t x = 0; x < nodes.size(); x++) 
	{
		if (nodes[x]->GetID() == iD) 
		{
			return RemoveModuleObject(x);
		}
	}
	return false;
}