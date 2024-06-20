//#pragma once
#include "GraphNode.h"
#include "VariableSelectorNode.h"

class SelectorNode : public Node
{
public:
	SelectorNode(int id, KPair pos);
	~SelectorNode();
	
	virtual TagInfo* DoTheThing();
	void LoadNodeInfo(GraphSave* node_info);
};

