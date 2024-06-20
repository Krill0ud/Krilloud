#include "Filter_Waveshaper.h"

Filter_Waveshaper::Filter_Waveshaper(int id, KPair pos)
{
	filter_parameters = { 
		CreateFilterParameter("Wet", 0, 0, 1),
		CreateFilterParameter("Amount", 0, -1, 1)
	};

	iD = id;

	name = "Waveshaper";

	this->pos = pos;
	inputsCount = 1;
	outputsCount = 1;
	can_be_deleted = true;

	myClass = Class_F_WaveShaper;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, GetInternalParameterValues(), 0);

	nodeType = NodeType::Node_Filter;

	this->size = KPair(250, 100);

	SetFilterSockets();
}

Filter_Waveshaper::~Filter_Waveshaper()
{
	RemoveAllLinks();
}