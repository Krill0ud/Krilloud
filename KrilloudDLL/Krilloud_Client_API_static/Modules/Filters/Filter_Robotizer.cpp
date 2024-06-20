#include "Filter_Robotizer.h"

Filter_Robotizer::Filter_Robotizer(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 1, 0, 1),
		CreateFilterParameter("Frequency", 30, 0.1, 100),
		CreateFilterParameter("Waveform", 0, 0, 6)
	};

	iD = id;

	name = "Robotizer";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_Robotizer;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 75);

	SetFilterSockets();
}

Filter_Robotizer::~Filter_Robotizer()
{
	RemoveAllLinks();
}