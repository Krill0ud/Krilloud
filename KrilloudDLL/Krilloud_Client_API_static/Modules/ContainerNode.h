#pragma once
#include "GraphNode.h"
#include "../NodeGraphSystemLevel.h"

class NodeGraphSystemLevel;

class ContainerNode : public Node
{
	NodeGraphSystemLevel* myGraphLevel;
public:
	int numberOfOutputs;

	ContainerNode(int id, KPair pos, NodeGraphSystemLevel* mylevel);
	~ContainerNode();

	virtual TagInfo* DoTheThing();
};

