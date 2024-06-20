#include "GraphNode.h"

class OutputNode : public Node
{
public:
	OutputNode(int id, KPair pos);
	~OutputNode();

	virtual TagInfo* DoTheThing();
};

