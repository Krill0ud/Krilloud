#include "FilterNode.h"
#include "AudioEngineFramework/AudioEngineFilter.h"

std::vector<FilterParameter> FilterNode::GetParameters()
{
	if (myKClass != nullptr)
	{
		std::vector<float> internal_parameters = ((AudioEngineFilter*)myKClass)->GetParameters();
		
		for (size_t x = 0; x < internal_parameters.size(); x++) 
		{
			if (filter_parameters.size() > x) 
			{
				filter_parameters[x].value = internal_parameters[x];
			}
		}

		return filter_parameters;
	}
	return {};
}

void FilterNode::SetFilterSockets()
{
	CreateSocket(15, 0, Input, Normal, false, false);
	CreateSocket(50, 1, Output, Normal, false, false);

	for (size_t x = 0; x < filter_parameters.size(); x++) 
	{
		CreateSocket(32 + (19 * x), (x + 2), Input, Variable, false, false);
	}
}

std::vector<float> FilterNode::GetInternalParameterValues() 
{
	std::vector<float> internal_parameters;

	for (size_t x = 0; x < filter_parameters.size(); x++)
	{
		internal_parameters.push_back(filter_parameters[x].value);
	}

	return internal_parameters;
}

FilterParameter FilterNode::CreateFilterParameter(const char* new_name, float new_value, float new_min_value, float new_max_value)
{
	FilterParameter parameter;

	parameter.name = new_name;
	parameter.value = new_value;
	parameter.min_value = new_min_value;
	parameter.max_value = new_max_value;

	return parameter;
}

bool FilterNode::SetParameters()
{
	if (myKClass != nullptr)
	{
		std::vector<float> internal_parameters;

		for (size_t x = 0; x < filter_parameters.size(); x++) 
		{
			internal_parameters.push_back(filter_parameters[x].value);
		}

		((AudioEngineFilter*)myKClass)->SetParameters(internal_parameters);
		return true;
	}
	return false;
}

bool FilterNode::SetParameter(int index, float value)
{
	size_t count = filter_parameters.size();
	if (count > 0 && index < count) 
	{
		filter_parameters[index].value = value;
		if (myKClass != nullptr)
		{
			((AudioEngineFilter*)myKClass)->SetParameter(index, value);
			return true;
		}
		return false;
	}
	return false;
}

TagInfo * FilterNode::DoTheThing()
{
	return nullptr;
}

GraphSave FilterNode::SaveNodeInfo()
{
	GraphSave myInfoToSave = Node::SaveNodeInfo();
	
	for (size_t x = 0; x < filter_parameters.size(); x++)
	{
		myInfoToSave.values.push_back(filter_parameters[x].value);
	}
	
	return myInfoToSave;
}

void FilterNode::LoadNodeInfo(GraphSave* node_info)
{
	Node::LoadNodeInfo(node_info);
	
	/*filter_parameters.clear();

	for (size_t x = 0; x < node_info->values.size(); x++)
	{
		filter_parameters.push_back(node_info->values[x]);
	}*/

	for (size_t x = 0; x < node_info->values.size(); x++)
	{
		if (filter_parameters.size() > x) 
		{
			filter_parameters[x].value = node_info->values[x];
		}
	}

	SetParameters();
}