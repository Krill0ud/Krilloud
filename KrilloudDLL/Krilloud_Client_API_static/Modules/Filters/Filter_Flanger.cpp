#include "Filter_Flanger.h"

Filter_Flanger::Filter_Flanger(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Delay", 0.005f, 0.001f, 0.1f),
		CreateFilterParameter("Frequency", 10, 0.001f, 100)
	};

	iD = id;

	name = "Flanger";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_Flanger;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_Flanger::~Filter_Flanger()
{
	RemoveAllLinks();
}