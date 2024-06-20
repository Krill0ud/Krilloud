#include "Filter_Pitch.h"

Filter_Pitch::Filter_Pitch(int id, KPair pos)
{
	iD = id;

	name = "Pitch";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	filter_parameters = {
		CreateFilterParameter("Tune", 0, -24, 24),
	};

	myClass = Class_F_Pitch;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_Pitch::~Filter_Pitch()
{
	RemoveAllLinks();
}