#include "Filter_HighPass.h"

Filter_HighPass::Filter_HighPass(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Frequency", 8000, 20, 20000),
		CreateFilterParameter("Resonance", 2, 0.1, 20)
	};

	iD = id;

	name = "High pass";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_HighPass;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_HighPass::~Filter_HighPass()
{
	RemoveAllLinks();
}