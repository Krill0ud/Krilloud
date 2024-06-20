#pragma once
#include <vector>
#include <thread>

#include "KrilloudClasses.h"

#include "NodeInfoStructures.h"
#include "PublicStaticKrilloud.h"

namespace KrilloudEngine 
{
	// --- Internal functions

	// --- Thread functions -----------------------------------------------------------------------------------------------------------------------------
	void Thread_VolumeUpdater(std::vector<PlayingTag*>* playing_tags_list);
	void Thread_PlayUpdater(std::vector<PlayingTag*>* playing_tags_list);

	// --- Tag functions --------------------------------------------------------------------------------------------------------------------------------
	int UnloadTag(const char * tagname);
	int StopInstancedTag(std::string tagname);
	CompleteKrillTag* GetTag(std::string tagname);

	// --- KLevels and Nodes functions ------------------------------------------------------------------------------------------------------------------
	TagInfo * GetInfoFromKLevel(KLevel* level);
	bool LevelsNotRelated(KLevel* new_level, KLevel* level);

	// -- Krilloud vars functions -----------------------------------------------------------------------------------------------------------------------
	VariableData* GetKVariableData(int position);
	VariableData* GetKVariableData(std::string var_name);

	// --- GameObjects functions ------------------------------------------------------------------------------------------------------------------------
	IngameObject* FindIngameObject(int object_id);

	// --- Channel functions ----------------------------------------------------------------------------------------------------------------------------
	bool ChannelHasChildName(int parent_id, std::string new_name);
	bool ChannelIDExists(int channel_id);
	bool ChannelIsDescendant(int channel_id, int parent_id);

	// --- KClass functions -----------------------------------------------------------------------------------------------------------------------------
	KrilloudClass* GetKClass(int id, std::vector<KrilloudClass*> class_container);

	// --- Loaded audios list functions -----------------------------------------------------------------------------------------------------------------

	std::vector<LoadedSound*>* GetLoadedSoundsList();
	bool AudioIsLoaded(int audio_id);
	void UnloadAudio(int id);
	void UnloadSoundList();
	bool AddLoadedAudioOwner(int audio_id, std::string tag_name);
	bool RemoveLoadedAudioOwner(int audio_id, std::string tag_name);
	void LoadAudioFromSoundbank(int id);
	int LoadAudioFromDisk(int id, std::string filename);
	int LoadLevelAudioSources(KLevel* level, std::string owner_tag);

	// --- Instantiation --------------------------------------------------------------------------------------------------------------------------------
	KLevel* InstantiateTagLevel(LevelResult* graphLevel_info, int object_id);
	void InstantiateTag(LevelResult* graphLevel_info, int object_id);

	// --- Auxiliary functions --------------------------------------------------------------------------------------------------------------------------
	bool IsPathCorrect(std::string path);
	int KrilloudIsClean();
	KrilloudProjectInfo GetContractData();
	bool LoadContract(const char* contract_path);
}