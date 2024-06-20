#pragma once
#include "NodeInfoStructures.h"
#include "KrilloudSerialized.h"
#include <cmath>

using KPair = std::pair<float, float>;

inline KPair operator+(const KPair& lhs, const KPair& rhs) { return std::pair<float, float>(lhs.first + rhs.first, lhs.second + rhs.second); }
inline KPair operator-(const KPair& lhs, const KPair& rhs) { return std::pair<float, float>(lhs.first - rhs.first, lhs.second - rhs.second); }
inline bool operator==(const KPair& lhs, const KPair& rhs) { return ((fabsf(lhs.first - rhs.first) < 4) && (fabsf(lhs.second - rhs.second) < 4)); }

class AudioEngine;
class KrilloudClass;

struct KLink //API y lib
{
	int SocketID;
	SocketType connection_type;
	SocketDataType data_type;
	std::vector<KrilloudClass*> connections;
};

class KrilloudClass
{
public:
	enum KrilloudFilterType { BandPass, BassBoost, BQR, DC_Remover, Delay, Echo, Flanger, HighPass, LoFi, LowPass, Pitch, Robotizer, WaveShaper, Freeverb };

	//ID of the object
	int internal_id;

	//class of the object
	NodeClass node_class;

	//Object current sockets
	std::vector<KLink> myKLinks;

	//Object link socket control
	virtual int CreateNewSocket(int ID, SocketType type, SocketDataType data);
	virtual bool RemoveSocket(int id);

	//Links between two objects
	bool AddLink(int socketID, KrilloudClass* class_to_link);
	bool RemoveLink(int socketID, KrilloudClass* class_to_unlink);

	//Virtual functions
	//virtual float GetVariableValue();
	virtual TagInfo* GetInfo() = 0;
};

class AudioContainer : public KrilloudClass
{
	size_t last_selection_from_list = 0;
	TagInfo* myTagInfo;
	AudioEngine* engine_instance;
	bool loopable_audio;
public:
	int selection_mode = 0;

	//list to store ids of contained sounds
	std::vector<AudioSourceContent*> content_list;
	//pointer to global loaded audios
	std::vector<LoadedSound*> * audios_list_pointer;

	//Constructor & destructor
	AudioContainer(std::vector<LoadedSound*>* loaded_audios_list, AudioEngine* engine);
	~AudioContainer();

	//Change the container selection mode
	void ChangeSelection(int new_mode);

	//Set the container's loop mode
	void SetLoop(bool loopable);

	//Get the container's loop mode
	bool GetLoop();

	//Get info package to play sounds
	virtual TagInfo* GetInfo();

	//Stored ids management
	std::vector<AudioSourceContent*>* GetContainedIDs();
	int AddSoundID(int id, float volume = 1);
	int AddSoundID(int id, std::string, float volume);
	int AddSoundID(std::vector<AudioSourceContent*>* sound_ids_list);
	int RemoveSoundID(int id);
};

class KrilloudMacro : public KrilloudClass
{
	KLevel* myLevel;
public:
	KrilloudMacro();
	~KrilloudMacro();

	void SetLevel(KLevel* level);
	KLevel* GetLevel();

	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo();
};

class KrillOutput : public KrilloudClass
{
public:
	KrillOutput();
	~KrillOutput();

	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo();
};

class KrilloudVar : public KrilloudClass
{
	VariableData* myData;
	float lastValue = 0;
	std::vector<int> played_values;
	bool alreadySet = false;

public:
	KrilloudVar();
	~KrilloudVar();

	void SetValue(float value);
	void SetName(std::string new_name);
	void SetType(VarType new_type);
	void SetSource(VarSourceType new_source_type);
	void SetContent(VarContentType new_content_type);
	void SetRangeMin(float new_range_min);
	void SetRangeMax(float new_range_max);
	void SetOnPlay(bool new_set_on_play);


	void SetVariableData(VariableData* data);
	VariableData* GetVariableData();

	float GetVariableValue();
	void Reset();

	//Get info package to play sounds
	virtual TagInfo* GetInfo();
};

class KrilloudSelector : public KrilloudClass
{
	int GetSelectionValue();
public:
	KrilloudSelector();
	~KrilloudSelector();

	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo();
};

class KrilloudMixer : public KrilloudClass
{
	std::vector<float> volumes;
public:
	KrilloudMixer();
	~KrilloudMixer();

	virtual int CreateNewSocket(int ID, SocketType type, SocketDataType data);
	virtual bool RemoveSocket(int id);

	std::vector<float> GetVolumesList();
	void SetVolume(int selection, float volume);
	void UpdateVolumeValues();
	//Overriden from KrilloudClass
	virtual TagInfo* GetInfo();
};

class KrilloudLevel
{
public:
	std::vector<KrilloudClass*> nodes;
	std::vector<KrilloudLevel*> subLevels;

	TagInfo* GetInfo();
};