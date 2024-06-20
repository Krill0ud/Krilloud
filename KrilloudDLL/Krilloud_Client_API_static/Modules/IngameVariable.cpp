#include "IngameVariable.h"

IngameVariable::IngameVariable(int id, KPair pos, std::vector<VariableData*>* Kvars_list)
{
	iD = id;

	name = "Ingame variable";

	this->pos = pos;
	//inputsCount = 0;
	//outputsCount = 1;
	can_be_deleted = true;

	project_kvars_list = Kvars_list;

	myClass = Class_IngameVariable;
	
	scope = Global;
	source_type = InGame;
	content_type = Raw;

	variable_parameters.push_back((float)scope);
	variable_parameters.push_back((float)source_type);
	variable_parameters.push_back((float)content_type);
	variable_parameters.push_back(0);
	variable_parameters.push_back(1);

	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, variable_parameters, 0);

	this->size = KPair(175, 110);

	CreateSocket(25, 0, Output, Variable, false, true);

	nodeType = Node_Variable;

	initialized = false;
}

IngameVariable::~IngameVariable()
{
	RemoveAllLinks();
}

void IngameVariable::SetSimulatedValue(float newValue) 
{
	if ((newValue <= (*project_kvars_list)[list_selection]->range_max) || (newValue >= (*project_kvars_list)[list_selection]->range_min)) 
	{
		simulated_current_value = newValue;
		(*project_kvars_list)[list_selection]->current_value = newValue;
	}
}

void IngameVariable::SetOnlyOnPlay(bool newValue)
{
	(*project_kvars_list)[list_selection]->set_only_on_play = newValue;
}

void IngameVariable::SetVariableSelection(int new_value)
{
	list_selection = new_value;

	name = (*project_kvars_list)[list_selection]->variable_name;

	if (myKClass != nullptr) 
	{
		((KrilloudVar*)myKClass)->SetVariableData((*project_kvars_list)[list_selection]);
	}
}

int IngameVariable::GetVariableSelection()
{
	return list_selection;
}

int IngameVariable::GiveMeSomething(int min_range_value, int max_range_value) {
	
	return simulated_current_value;
}

void IngameVariable::LoadNodeInfo(GraphSave* node_info)
{
	VariableSelectorNode::LoadNodeInfo(node_info);

	for (size_t x = 0; x < project_kvars_list->size(); x++) 
	{
		if ((*project_kvars_list)[x]->variable_name == this->name) 
		{
			list_selection = x;
			break;
		}
	}
}

//void IngameVariable::UpdateKVariableData() 
//{
//	if (project_kvars_list->size() > 0) 
//	{
//		VariableData* updated_data = new VariableData();
//
//		updated_data->variable_name = (*project_kvars_list)[list_selection]->variable_name;
//		updated_data->content = (*project_kvars_list)[list_selection]->content;
//		updated_data->type = (*project_kvars_list)[list_selection]->type;
//		updated_data->source = (*project_kvars_list)[list_selection]->source;
//		updated_data->current_value = (*project_kvars_list)[list_selection]->current_value;
//		updated_data->range_min = (*project_kvars_list)[list_selection]->range_min;
//		updated_data->range_max = (*project_kvars_list)[list_selection]->range_max;
//		updated_data->set_only_on_play = (*project_kvars_list)[list_selection]->set_only_on_play;
//
//		((KrilloudVar*)myKClass)->SetVariableData(updated_data);
//	}
//}

VariableData* IngameVariable::GetAssociatedData()
{
	return (*project_kvars_list)[list_selection];
}