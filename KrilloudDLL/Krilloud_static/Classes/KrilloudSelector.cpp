//#include "KrilloudSelector.h"
//#include "KrilloudVar.h"
#include "KrilloudClasses.h"
#include <random>

KrilloudSelector::KrilloudSelector()
{
}

KrilloudSelector::~KrilloudSelector()
{
}

//NOTE: Deprecated
int KrilloudSelector::GetSelectionValue() 
{
	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Variable && myKLinks[x].connections.size() > 0 && myKLinks[x].connections[0] != nullptr)
		{
			KrilloudVar* variable_node = dynamic_cast<KrilloudVar*>(myKLinks[x].connections[0]);

			if (variable_node != nullptr)
			{
				return (int)variable_node->GetVariableValue();
			}
		}
	}
	return 0;
}

TagInfo* KrilloudSelector::GetInfo()
{
	KrilloudVar* connected_var = nullptr;

	//Get connected KrillVariable
	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Variable && myKLinks[x].connections.size() > 0 && myKLinks[x].connections[0] != nullptr)
		{
			connected_var = dynamic_cast<KrilloudVar*>(myKLinks[x].connections[0]);
			break;
		}
	}

	int selection = 0;
	
	//Get connected KrillVariable current value
	if (connected_var != nullptr)
	{
		selection = connected_var->GetVariableValue();
	}

	std::vector<KrilloudClass*> inputs;
	TagInfo* result = nullptr;

	//Get normal input sockets that can have something connected to them
	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Normal)
		{
			if (myKLinks[x].connections.size() > 0)
			{
				inputs.push_back(myKLinks[x].connections[0]);
			}
			else
			{
				inputs.push_back(nullptr);
			}
		}
	}

	if (inputs.size() > 0)
	{
		if (selection >= 0)
		{
			if (selection < (int)inputs.size())
			{
				if (inputs[selection] != nullptr)
				{
					result = inputs[selection]->GetInfo();
				}
			}
			else
			{
				if (inputs[((int)inputs.size()) - 1] != nullptr)
				{
					result= inputs[((int)inputs.size()) - 1]->GetInfo();
				}
			}
		}
		else //This should never happen, but safety first
		{	
			if (inputs[0] != nullptr)
			{
				result = inputs[0]->GetInfo();
			}
		}
	}

	//Add variable to result
	if (connected_var != nullptr && result != nullptr)
	{
		result->kvars.push_back(connected_var);
	}
	
	return result;
}