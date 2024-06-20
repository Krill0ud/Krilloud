#include "InputNode.h"

InputNode::InputNode(int id, KPair pos)
{
	iD = id;

	name = "Input";

	this->pos = pos;
	//inputsCount = 0;
	//outputsCount = 1;
	
	myClass = Class_Input;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, initial, 0);

	this->size = KPair(100, 100);

	CreateSocket(50, 0, Output, Normal, false, false);
}

InputNode::~InputNode()
{
	RemoveAllLinks();
}

TagInfo * InputNode::DoTheThing()
{
	return nullptr;
}
