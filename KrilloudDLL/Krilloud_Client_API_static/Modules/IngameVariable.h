#pragma once
#include "VariableSelectorNode.h"

class IngameVariable: public VariableSelectorNode
{
	int simulated_current_value;
	bool integer_simulated_value;
	bool initialized;

	std::vector<VariableData*>* project_kvars_list;
	int list_selection = 0;

	
public:
	IngameVariable(int id, KPair pos, std::vector<VariableData*>* Kvars_list);
	~IngameVariable();

	void SetSimulatedValue(float newValue);
	void SetOnlyOnPlay(bool newValue);
	void SetVariableSelection(int new_value);
	int GetVariableSelection();
	
	//void UpdateKVariableData();

	VariableData* GetAssociatedData();

	virtual int GiveMeSomething(int min_range_value, int max_range_value);

	void LoadNodeInfo(GraphSave* node_info);
};

