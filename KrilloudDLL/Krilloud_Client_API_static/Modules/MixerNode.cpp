#include "MixerNode.h"

MixerNode::MixerNode(int id, KPair pos)
{
	iD = id;

	name = "Mixer";

	this->pos = pos;

	myClass = Class_Mixer;
	std::vector<float> initial;
	myKClass = KrilloudEngine::CreateKClass(myClass, name, id, initial, 0);

	requiresFluxConnection = false;
	hasVariableSocket = true;
	can_be_deleted = true;

	nodeType = Node_Mixer;

	this->size = KPair(200, 120);

	CreateSocket(50, 0, Output, Normal, false, true);
	CreateSocket(50, 1, Input, Normal, false, false);
	CreateSocket(82, 2, Input, Normal, false, false);
}


MixerNode::~MixerNode()
{
	RemoveAllLinks();
}

void MixerNode::CreateSocket(float position, int iD, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
{
	if (sType == SocketType::Input && sDType == SocketDataType::Normal) 
	{
		int current_socket_id = iD * 2;
		
		Node::CreateSocket(position, current_socket_id - 1, Input, Variable, isDeletable, multipleLinks);
		Node::CreateSocket(position + 15, current_socket_id, Input, Normal, isDeletable, multipleLinks);
	}
	else if (sType == SocketType::Output) 
	{
		Node::CreateSocket(position, iD, sType, sDType, isDeletable, multipleLinks);
	}
}

void MixerNode::DeleteSocket(size_t iD)
{
	size_t index = 0;
	bool found = false;

	while (!found && index < mySockets.size())
	{
		if (mySockets[index].socketID == iD)
		{
			found = true;
		}
		else
		{
			index++;
		}
	}

	if (found && mySockets[index].deletable)
	{
		for (size_t x = 0; x <= 1; x++) 
		{
			RemoveLink(index);

			if (myKClass != nullptr)
			{
				myKClass->RemoveSocket(index);
			}

			bool input_type = (mySockets[index].type == SocketType::Input);

			mySockets.erase(mySockets.begin() + index);

			if (input_type)
			{
				inputsCount--;
			}
			else
			{
				outputsCount--;
			}
		}
	}
}

std::vector<float> MixerNode::GetCurrentVolumes()
{
	if (myKClass != nullptr) 
	{
		return ((KrilloudMixer*)myKClass)->GetVolumesList();
	}
	return {};
}

void MixerNode::SetVolume(int socket, float volume)
{
	if (myKClass != nullptr)
	{
		((KrilloudMixer*)myKClass)->SetVolume(socket, volume);
	}
}

TagInfo * MixerNode::DoTheThing()
{
	if (mySockets[0].socketLinks.size() > 0)
	{
		int variable_result = ((VariableSelectorNode*)mySockets[0].socketLinks[0].connectedNode)->GiveMeSomething(1, inputsCount);

		if (variable_result >= 1 && variable_result < ((int)mySockets.size() - 1))
		{
			if (mySockets[variable_result + 1].socketLinks.size() > 0) {
				if (mySockets[variable_result + 1].socketLinks[0].connectedNode != nullptr)
				{
					return mySockets[variable_result + 1].socketLinks[0].connectedNode->DoTheThing();
				}
			}
		}
	}

	return nullptr;
}

GraphSave MixerNode::SaveNodeInfo()
{
	GraphSave myInfoToSave = Node::SaveNodeInfo();

	myInfoToSave.values = GetCurrentVolumes();

	return myInfoToSave;
}

void MixerNode::LoadNodeInfo(GraphSave* node_info)
{
	Node::LoadNodeInfo(node_info);

	int newCountDiff = (node_info->inputsCount - 4)/2;

	for (int x = 0; x < newCountDiff; x++)
	{
		CreateSocket(50 + (16 * (float)inputsCount), (int)mySockets.size() - (((int)mySockets.size() - 1) / 2), Input, Normal, true, false);
	}

	for (size_t x = 0; x < node_info->values.size(); x++) 
	{
		SetVolume(x, node_info->values[x]);
	}
}