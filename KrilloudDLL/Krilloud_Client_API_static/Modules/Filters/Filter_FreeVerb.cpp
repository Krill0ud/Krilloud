#include "Filter_FreeVerb.h"

Filter_FreeVerb::Filter_FreeVerb(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 1, 0, 1),
		CreateFilterParameter("Freeze", 0, 0, 1),
		CreateFilterParameter("Room Size", 0.5f, 0, 1),
		CreateFilterParameter("Damp", 0.5f, 0, 1),
		CreateFilterParameter("Width", 1, 0, 1)
	};

	iD = id;

	name = "FreeVerb";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_Freeverb;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_FreeVerb::~Filter_FreeVerb()
{
	RemoveAllLinks();
}