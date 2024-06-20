#pragma once
#include "GraphNode.h"

class InputNode : public Node
{
public:
	InputNode(int id, KPair pos);
	~InputNode();

	virtual TagInfo* DoTheThing();
};

