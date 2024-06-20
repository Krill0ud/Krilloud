#include "GraphNode.h"

Node& Node::operator=(const Node & newNode)
{
	iD = newNode.iD;
	name = newNode.name;
	pos = newNode.pos;
	inputsCount = newNode.inputsCount;
	outputsCount = newNode.outputsCount;

	requiresFluxConnection = newNode.requiresFluxConnection;
	hasVariableSocket = newNode.hasVariableSocket;
	can_be_deleted = newNode.can_be_deleted;
	
	this->mySockets = newNode.mySockets;

	return *this;
}

Node::~Node()
{
	RemoveAllLinks();
}

int Node::GetID() 
{
	return iD;
}

void Node::SetID(int new_id)
{
	iD = new_id;
}

KrilloudClass* Node::GetKrilloudClass()
{
	return myKClass;
}

KPair Node::GetSize()
{
	return size;
}

void Node::SetSize(KPair new_size)
{
	size = new_size;
}

KPair Node::GetPosition()
{
	return pos;
}

void Node::SetPosition(KPair new_position)
{
	pos = new_position;
}

NodeType Node::GetType() 
{
	return nodeType;
}

NodeClass Node::GetNodeClass()
{
	return myClass;
}

bool Node::CanBeDeleted()
{
	return can_be_deleted;
}

const char* Node::GetName()
{
	return name.c_str();
}

void Node::SetName(std::string new_name)
{
	name = new_name;
}

int Node::GetOutputsCount()
{
	return outputsCount;
}

int Node::GetInputsCount()
{
	return inputsCount;
}

bool Node::CanHaveLinks(size_t socketIndex)
{
	if (socketIndex < mySockets.size())
	{
		return mySockets[socketIndex].type == Output || mySockets[socketIndex].socketLinks.size() <= 0;
	}

	return false;
}

int Node::GetLinkedID(size_t socketIndex, size_t link_index) 
{
	if (socketIndex < mySockets.size())
	{
		if (link_index < mySockets[socketIndex].socketLinks.size()) 
		{
			return mySockets[socketIndex].socketLinks[link_index].connectedNode->GetID();
		}
	}

	return -1;
}

int Node::GetLinkedIDSocket(size_t socketIndex, size_t link_index)
{
	if (socketIndex < mySockets.size())
	{
		if (link_index < mySockets[socketIndex].socketLinks.size())
		{
			return mySockets[socketIndex].socketLinks[link_index].connectedNodeSocket;
		}
	}

	return -1;
}

size_t Node::SocketLinksCount(size_t socketIndex)
{
	if (socketIndex < mySockets.size())
	{
		return mySockets[socketIndex].socketLinks.size();
	}

	return 0;
}

int Node::GetSocketCount() 
{
	return mySockets.size();
}

bool Node::CreateLink(size_t socketIndex, Node *nodeToLink, size_t linkedNodeSocketIndex)
{
	if (socketIndex < mySockets.size()) 
	{
		if (myKClass != nullptr) 
		{
			NodeLink newLink;

			newLink.connectedNode = nodeToLink;
			newLink.connectedNodeSocket = (int)linkedNodeSocketIndex;

			mySockets[socketIndex].socketLinks.push_back(newLink);

			return myKClass->AddLink(mySockets[socketIndex].socketID/*socketIndex*/, newLink.connectedNode->myKClass);
		}
	}
	return false;
}

bool Node::RemoveLink(size_t socket)
{
	if (socket < mySockets.size())
	{
		if (myKClass != nullptr)
		{
			for (size_t x = 0; x < mySockets[socket].socketLinks.size(); x++) 
			{
				myKClass->RemoveLink(socket, mySockets[socket].socketLinks[x].connectedNode->myKClass);
			}

			mySockets[socket].socketLinks.clear();
			return true;
		}	
	}
	return false;
}

bool Node::RemoveLink(size_t socket, size_t link)
{
	if (socket < mySockets.size()) 
	{
		if (link < mySockets[socket].socketLinks.size()) 
		{
			if (myKClass != nullptr)
			{
				mySockets[socket].socketLinks.erase(mySockets[socket].socketLinks.begin() + link);
				return myKClass->RemoveLink(socket, mySockets[socket].socketLinks[link].connectedNode->myKClass);
			}
		}
	}
	return false;
}

bool Node::RemoveLink(size_t socket, Node * link)
{
	if (socket < mySockets.size())
	{
		for (size_t x = 0; x < mySockets[socket].socketLinks.size(); x++)
		{
			if (mySockets[socket].socketLinks[x].connectedNode == link) 
			{
				if (myKClass != nullptr)
				{
					mySockets[socket].socketLinks.erase(mySockets[socket].socketLinks.begin() + x);
					return myKClass->RemoveLink(socket, link->myKClass);
				}
				break;
			}
		}
	}
	return false;
}

bool Node::RemoveAllLinks()
{
	for (size_t x = 0; x < mySockets.size(); x++)
	{
		for (size_t y = 0; y < mySockets[x].socketLinks.size(); y++)
		{
			Node *node = mySockets[x].socketLinks[y].connectedNode;

			if (node != nullptr) 
			{
				if (myKClass != nullptr)
				{
					node->RemoveLink(mySockets[x].socketLinks[y].connectedNodeSocket, this);
					myKClass->RemoveLink(x, mySockets[x].socketLinks[y].connectedNode->myKClass);
				}
			}
		}
		mySockets[x].socketLinks.clear();
	}
	return true;
}

void Node::CreateSocket(float position, int iD, SocketType sType, SocketDataType sDType, bool isDeletable, bool multipleLinks)
{
	Socket newSocket;

	if (myKClass != nullptr)
	{
		myKClass->CreateNewSocket(iD, sType, sDType);
	}

	newSocket.position = position; 
	newSocket.socketID = iD;
	newSocket.type = sType;
	newSocket.dataType = sDType;
	newSocket.deletable = isDeletable;
	newSocket.allowMultipleLinks = multipleLinks;

	NodeLink newNodeLink;
	newNodeLink.connectedNode = nullptr;
	newNodeLink.connectedNodeSocket = 0;

	mySockets.push_back(newSocket);

	if (sType == SocketType::Input) 
	{
		inputsCount++;
	}
	else
	{
		outputsCount++;
	}
}

void Node::DeleteSocket(size_t iD)
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


int Node::SocketHasConnection(int index)
{
	size_t count = mySockets.size();
	if (count > 0 && index < count)
	{
		return mySockets[index].socketLinks.size();
	}
	return -1;
}

float Node::GetSocketPosition(int index)
{
	if (index >= mySockets.size())
		return {};

	return mySockets[index].position;
}

SocketDataType Node::GetSocketDataType(int index)
{
	if (index >= mySockets.size())
		return SocketDataType::Normal;

	return mySockets[index].dataType;
}

SocketType Node::GetSocketType(int index) 
{
	if (index >= mySockets.size())
		return SocketType::Input;

	return mySockets[index].type;
}

GraphSave Node::SaveNodeInfo()
{
	GraphSave myInfoToSave = GraphSave();

	myInfoToSave.type = myClass;

	myInfoToSave.ID = iD;
	myInfoToSave.Name = name;
	myInfoToSave.Pos_x = pos.first;
	myInfoToSave.Pos_y = pos.second;
	myInfoToSave.Size_x = size.first;
	myInfoToSave.Size_y = size.second;
	myInfoToSave.can_be_deleted = can_be_deleted;
	myInfoToSave.inputsCount = inputsCount;
	myInfoToSave.outputsCount = outputsCount;

	
	for (size_t x = 0; x < mySockets.size(); x++)
	{
		NodeSocket saved_node_socket = NodeSocket();

		saved_node_socket.SocketID = mySockets[x].socketID;
		saved_node_socket.data_type = mySockets[x].dataType;
		saved_node_socket.connection_type = mySockets[x].type;
		
		for (size_t y = 0; y < mySockets[x].socketLinks.size(); y++)
		{
			ConnectionsSave connections = ConnectionsSave();

			connections.my_socket_id = mySockets[x].socketID;
			connections.connected_node_id = mySockets[x].socketLinks[y].connectedNode == nullptr ? -1 : mySockets[x].socketLinks[y].connectedNode->iD;
			connections.connected_node_socket_id = mySockets[x].socketLinks[y].connectedNodeSocket;

			//myInfoToSave.links.push_back(connections);
			saved_node_socket.links.push_back(connections);
		}

		myInfoToSave.sockets.push_back(saved_node_socket);
	}

	return myInfoToSave;
}

void Node::LoadNodeInfo(GraphSave* node_info)
{
	if (myKClass != nullptr) 
	{
		myKClass->internal_id = node_info->ID;
	}

	iD = node_info->ID;

	name = node_info->Name;
	pos = KPair(node_info->Pos_x, node_info->Pos_y);
	size = KPair(node_info->Size_x, node_info->Size_y);
	
	//outputsCount = node_info->outputsCount;
	//inputsCount = node_info->inputsCount;

	can_be_deleted = node_info->can_be_deleted;
}
