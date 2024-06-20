#pragma once
#include <vector>
#include "NodeInfoStructures.h"
#include "PublicStaticKrilloud.h"
#include "KrilloudClasses.h"
#include "KrilloudAPIStructures.h"

class Node
{
protected:
	NodeClass myClass;
	NodeType nodeType = Node_Normal;
	KrilloudClass * myKClass;
	
	int iD;
	std::string name;
	KPair  pos, size;
	int inputsCount, outputsCount;
	std::string filePath;
public:
	struct NodeLink
	{
		int connectedNodeSocket;
		Node* connectedNode;
	};

	struct Socket {
		float position;
		int socketID;
		SocketType type;
		SocketDataType dataType;
		bool deletable;
		bool allowMultipleLinks;
		std::vector<NodeLink> socketLinks;
	};
	std::vector<Socket> mySockets;

	bool	can_be_deleted = false;
	bool	requiresFluxConnection = false;
	bool	hasVariableSocket = false;

	bool show_special_info;

	~Node();
	int GetID();
	void SetID(int new_id);
	
	KrilloudClass* GetKrilloudClass();

	KPair GetSize();
	void SetSize(KPair new_size);
	KPair GetPosition();
	void SetPosition(KPair new_position);
	NodeType GetType();
	NodeClass GetNodeClass();
	bool CanBeDeleted();

	const char* GetName();
	void SetName(std::string new_name);
	int GetOutputsCount();
	int GetInputsCount();

	Node& operator=(const Node & newNode);
		
	bool CanHaveLinks(size_t socketIndex);
	int GetLinkedID(size_t socketIndex, size_t link_index);
	int GetLinkedIDSocket(size_t socketIndex, size_t link_index);
	size_t SocketLinksCount(size_t socketIndex);
	int GetSocketCount();
	bool CreateLink(size_t socketIndex, Node *nodeToLink, size_t linkedNodeSocketIndex);
	bool RemoveLink(size_t socket);
	bool RemoveLink(size_t socket, size_t link);
	bool RemoveLink(size_t socket, Node *link);
	bool RemoveAllLinks();

	virtual void CreateSocket(float position, int iD, SocketType sType, SocketDataType sDType = Normal, bool isDeletable = true, bool multipleLinks = false);
	virtual void DeleteSocket(size_t iD);
	// Returns the number of connections in the socket. -1 if error.
	int SocketHasConnection(int index);
	float GetSocketPosition(int index);
	SocketDataType GetSocketDataType(int index);
	SocketType GetSocketType(int index);
	
	// --- Virtual functions ---
	virtual TagInfo* DoTheThing() = 0;

	virtual GraphSave SaveNodeInfo();
	virtual void LoadNodeInfo(GraphSave* node_info);
};
