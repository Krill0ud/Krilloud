#include "Filter_LoFi.h"

Filter_LoFi::Filter_LoFi(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Sample rate", 4000, 100, 22000),
		CreateFilterParameter("Bit depth", 3, 0.5, 16)
	};

	iD = id;

	name = "LoFi";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_LoFi;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_LoFi::~Filter_LoFi()
{
	RemoveAllLinks();
}