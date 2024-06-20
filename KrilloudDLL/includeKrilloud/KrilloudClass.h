#pragma once
//#include "KrilloudEnumerables.h"
//#include <vector>
//
//class AudioEngineSource;
//class AudioEngineFilter;
//class KrilloudVar;
//class KrilloudClass;
//
//struct TagInfo
//{
//	AudioEngineSource* source;
//	int wave_id;
//	float volume;
//	std::vector<AudioEngineFilter*> filters;
//	std::vector<KrilloudVar*> kvars;
//};
//
//struct KLink
//{
//	int SocketID;
//	SocketType connection_type;
//	SocketDataType data_type;
//	std::vector<KrilloudClass*> connections;
//};
//
//class KrilloudClass
//{
//public:
//	enum KrilloudFilterType { BandPass, BassBoost, BQR, DC_Remover, Delay, Echo, Flanger, HighPass, LoFi, LowPass, Pitch, Robotizer, WaveShaper, Freeverb };
//
//	//ID of the object
//	int internal_id;
//
//	//class of the object
//	NodeClass node_class;
//
//	//Object current sockets
//	std::vector<KLink> myKLinks;
//
//	//Object link socket control
//	int CreateNewSocket(int ID, SocketType type, SocketDataType data);
//	bool RemoveSocket(int id);
//
//	//Links between two objects
//	bool AddLink(int socketID, KrilloudClass* class_to_link);
//	bool RemoveLink(int socketID, KrilloudClass* class_to_unlink);
//
//	//Virtual functions
//	virtual float GetVariableValue();
//	virtual TagInfo* GetInfo() = 0;
//};