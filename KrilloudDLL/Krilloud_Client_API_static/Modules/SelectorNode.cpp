#include "SelectorNode.h"

SelectorNode::SelectorNode(int id, KPair pos)
{
	iD = id;

	name = "Selector";

	this->pos = pos;
	//inputsCount = 2;
	//outputsCount = 1;

	myClass = Class_Selector;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, initial, 0);

	requiresFluxConnection = false;
	hasVariableSocket = true;
	can_be_deleted = true;

	nodeType = Node_Selector;

	this->size = KPair(200, 100);

	CreateSocket(25, 0, Input, Variable, false, false);
	CreateSocket(50, 3, Output, Normal, false, true);
	CreateSocket(50, 1, Input, Normal, false, false);
	CreateSocket(65, 2, Input, Normal, false, false);
	
}


SelectorNode::~SelectorNode()
{
	RemoveAllLinks();
}

TagInfo * SelectorNode::DoTheThing()
{
	if (mySockets[0].socketLinks.size() > 0) 
	{
		int variable_result = ((VariableSelectorNode*)mySockets[0].socketLinks[0].connectedNode)->GiveMeSomething(1, inputsCount);

		if (variable_result >= 1 && variable_result < ((int)mySockets.size()-1)) 
		{
			if (mySockets[variable_result + 1].socketLinks.size() > 0) {
				if (mySockets[variable_result + 1].socketLinks[0].connectedNode != nullptr) 
				{
					return mySockets[variable_result + 1].socketLinks[0].connectedNode->DoTheThing();
				}
			}
		}
	}

	return nullptr;
}

void SelectorNode::LoadNodeInfo(GraphSave* node_info)
{
	Node::LoadNodeInfo(node_info);

	int newCountDiff = node_info->inputsCount - 3;

	for (int x = 0; x < newCountDiff; x++)
	{
		CreateSocket(65 + (15 * (x + 1)), (int)mySockets.size(), Input, Normal, true, false);
	}
}
