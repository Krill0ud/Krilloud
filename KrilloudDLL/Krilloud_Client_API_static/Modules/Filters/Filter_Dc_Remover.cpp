#include "Filter_Dc_Remover.h"

Filter_Dc_Remover::Filter_Dc_Remover(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1)
	};

	iD = id;

	name = "DC Remover";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_DC_Remover;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 50);

	SetFilterSockets();
}

Filter_Dc_Remover::~Filter_Dc_Remover()
{
	RemoveAllLinks();
}