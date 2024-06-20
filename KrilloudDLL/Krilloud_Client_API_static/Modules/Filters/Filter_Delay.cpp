#include "Filter_Delay.h"

Filter_Delay::Filter_Delay(int id, KPair pos)
{
	iD = id;

	name = "Delay";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Delay", 0.1f, 0.01f, 1),
		CreateFilterParameter("Decay", 0, 0.01f, 1),
		CreateFilterParameter("Filter", 0, 0.01f, 1)
	};

	myClass = Class_F_Delay;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_Delay::~Filter_Delay()
{
	RemoveAllLinks();
}