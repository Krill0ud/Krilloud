#include "Filter_LowPass.h"

Filter_LowPass::Filter_LowPass(int id, KPair pos)
{
	filter_parameters = {
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Frequency", 8000, 20, 20000),
		CreateFilterParameter("Resonance", 2, 0.1, 20)
	};

	iD = id;

	name = "Low pass";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_LowPass;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_LowPass::~Filter_LowPass()
{
	RemoveAllLinks();
}