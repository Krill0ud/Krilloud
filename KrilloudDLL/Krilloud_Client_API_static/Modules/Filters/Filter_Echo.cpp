#include "Filter_Echo.h"

Filter_Echo::Filter_Echo(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 1, 0, 1),
		CreateFilterParameter("Delay", 0.3f, 0.01f, 0.3f),
		CreateFilterParameter("Decay", 0.7f, 0, 1),
		CreateFilterParameter("Filter", 0, 0, 1)
	};

	iD = id;

	name = "Echo";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_Echo;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_Echo::~Filter_Echo()
{
	RemoveAllLinks();
}