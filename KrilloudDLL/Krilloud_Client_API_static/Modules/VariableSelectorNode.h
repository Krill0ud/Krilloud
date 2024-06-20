#pragma once
#include "GraphNode.h"

class VariableSelectorNode : public Node
{
protected:
	void SetScope(VarType scope);
	void SetSource(VarSourceType scope);
	void SetContent(VarContentType scope);
	void SetRange(float min, float max);
	void SetValue(float value);
public:
	VarSourceType source_type;
	VarContentType content_type;
	VarType scope;

	int selectorValue = 0;
	std::vector<float> variable_parameters;
	
	virtual TagInfo* DoTheThing();
	virtual int GiveMeSomething(int min_range_value, int max_range_value) = 0;

	GraphSave SaveNodeInfo();
	void LoadNodeInfo(GraphSave* node_info);
};

