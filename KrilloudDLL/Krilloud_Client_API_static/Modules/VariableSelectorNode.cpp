#include "VariableSelectorNode.h"

void VariableSelectorNode::SetScope(VarType scope) 
{
	if (myKClass != nullptr) 
	{
		VariableData* var_data = ((KrilloudVar*)myKClass)->GetVariableData();

		var_data->type = scope;
	}
	variable_parameters[0] = (float)scope;
}

void VariableSelectorNode::SetSource(VarSourceType source_type)
{
	if (myKClass != nullptr)
	{
		VariableData* var_data = ((KrilloudVar*)myKClass)->GetVariableData();

		var_data->source = source_type;
	}
	variable_parameters[1] = (float)source_type;
}

void VariableSelectorNode::SetContent(VarContentType content_type)
{
	if (myKClass != nullptr)
	{
		VariableData* var_data = ((KrilloudVar*)myKClass)->GetVariableData();

		var_data->content = content_type;
	}
	variable_parameters[2] = (float)content_type;
}

void VariableSelectorNode::SetRange(float min, float max)
{
	if (myKClass != nullptr)
	{
		VariableData* var_data = ((KrilloudVar*)myKClass)->GetVariableData();

		var_data->range_min = min;
		var_data->range_max = max;
	}
	variable_parameters[3] = min;
	variable_parameters[4] = max;
}

void VariableSelectorNode::SetValue(float value) 
{
	if (myKClass != nullptr)
	{
		VariableData* var_data = ((KrilloudVar*)myKClass)->GetVariableData();

		var_data->current_value = value;
	}
}

TagInfo * VariableSelectorNode::DoTheThing()
{
	return nullptr;
}

GraphSave VariableSelectorNode::SaveNodeInfo()
{
	GraphSave myInfoToSave = Node::SaveNodeInfo();

	for (size_t x = 0; x < variable_parameters.size(); x++)
	{
		myInfoToSave.values.push_back(variable_parameters[x]);
	}

	return myInfoToSave;
}

void VariableSelectorNode::LoadNodeInfo(GraphSave* node_info)
{
	Node::LoadNodeInfo(node_info);

	variable_parameters.clear();

	for (size_t x = 0; x < node_info->values.size(); x++)
	{
		variable_parameters.push_back(node_info->values[x]);
	}
}
