#pragma once
#include <thread>
#include <set>

#include <boost/asio.hpp>

#include "KrilloudDebug.h"
#include "KrillNetMessage.h"

#include "Main.h"
#include "AudioEngineFilter.h"

namespace KrilloudRT
{
	//Server functions
	void NotifyListener(IngameObject listener);
	void NotifyNewObject(IngameObject object);
	void NotifyObjectRemove(IngameObject object);
	void NotifyObjectChange(IngameObject object);

	void NotifyNewPlayingTag(PlayingTag tag);
	void NotifyPlayingTagRemove(PlayingTag tag);
	void NotifyNewPlayingTagChange(PlayingTag tag);

	//Internal functions
	NodeType GetNodeType(NodeClass source_class);
	void PerformCommand(KCommand command);
	void RefreshGameObjectInfo(SystemObjectInfo new_info);
	bool DeliverMessage(SerializedMessage sMsg);
}
