#include "Filter_Bassboost.h"

Filter_Bassboost::Filter_Bassboost(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 1, 0, 1),
		CreateFilterParameter("Boost", 0, 0, 10),
	};

	iD = id;

	name = "BassBoost";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_Bassboost;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 75);

	SetFilterSockets();
}

Filter_Bassboost::~Filter_Bassboost()
{
	RemoveAllLinks();
}