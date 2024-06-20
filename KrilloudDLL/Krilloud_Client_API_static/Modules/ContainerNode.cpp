#include "ContainerNode.h"

ContainerNode::ContainerNode(int id, KPair pos, NodeGraphSystemLevel* mylevel)
{
	iD = id;

	name = "Macro";

	this->pos = pos;
	//inputsCount = 1;
	//outputsCount = 1;

	myClass = Class_Macro;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(Class_Macro, name, id, initial, 0);

	if (myKClass != nullptr && mylevel != nullptr) 
	{
		((KrilloudMacro*)myKClass)->SetLevel(mylevel->myKLevel);
		NodeGraphSystemLevel* myParent = mylevel->GetParent();

		if (myParent != nullptr) 
		{
			bool ok = KrilloudEngine::AddKLevel(mylevel->myKLevel, myParent->myKLevel);
		}
	}

	requiresFluxConnection = false;
	hasVariableSocket = true;
	can_be_deleted = true;

	this->size = KPair(100, 100);

	myGraphLevel = mylevel;

	CreateSocket(50, 0, Input, SocketDataType::Normal, false, false);
	CreateSocket(50, 1, Output, SocketDataType::Normal, false, true);
}

ContainerNode::~ContainerNode()
{
	RemoveAllLinks();
}

TagInfo * ContainerNode::DoTheThing()
{
	//return myGraphLevel->DoTheThing();
	return nullptr;
}