#include "OutputNode.h"

OutputNode::OutputNode(int id, KPair pos)
{
	iD = id;

	name = "Output";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 0;
	
	myClass = Class_Output;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, initial, 0);

	nodeType = Node_Output;

	this->size = KPair(100, 50);

	CreateSocket(25, 0, Input, Normal, false, false);
}

OutputNode::~OutputNode()
{
	RemoveAllLinks();

	((KrillOutput*)myKClass)->~KrillOutput();
}

TagInfo* OutputNode::DoTheThing()
{
	if (myKClass != nullptr) 
	{
		return myKClass->GetInfo();
	}
	return nullptr;
}
