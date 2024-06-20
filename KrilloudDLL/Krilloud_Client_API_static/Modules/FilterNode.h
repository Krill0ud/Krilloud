#pragma once
#include "GraphNode.h"

class FilterNode : public Node
{
protected:
	std::vector<FilterParameter> filter_parameters; //tiene que ser un struct o una pareja con el nombre de cada parametro
	void SetFilterSockets();
	std::vector<float> GetInternalParameterValues();
	FilterParameter CreateFilterParameter(const char* new_name, float new_value, float new_min_value, float new_max_value);
public:
	std::vector<FilterParameter> GetParameters();
	bool SetParameters();
	bool SetParameter(int index, float value);

	virtual TagInfo* DoTheThing();

	GraphSave SaveNodeInfo();
	void LoadNodeInfo(GraphSave* node_info);
};

