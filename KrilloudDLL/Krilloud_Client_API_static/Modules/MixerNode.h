#pragma once
#include "GraphNode.h"
#include "VariableSelectorNode.h"

class MixerNode : public Node
{
public:
	MixerNode(int id, KPair pos);
	~MixerNode();

	virtual void CreateSocket(float position, int iD, SocketType sType, SocketDataType sDType = Normal, bool isDeletable = true, bool multipleLinks = false);
	virtual void DeleteSocket(size_t iD);

	std::vector<float> GetCurrentVolumes();
	void SetVolume(int socket, float volume);

	virtual TagInfo* DoTheThing();
	
	GraphSave SaveNodeInfo();
	void LoadNodeInfo(GraphSave* node_info);
};