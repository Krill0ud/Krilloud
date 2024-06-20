#pragma once
#include <string>
#include <vector>
#ifdef __ANDROID__
#include "AndroidStructures.h"
#endif
#include "KrilloudSerialized.h"
struct TagInfo;
struct LoadedSound;
struct Channel;
struct KrillTag;
struct KLevel;
struct KrilloudProjectInfo;
enum NodeClass;
class KrilloudClass;

namespace KrilloudEngine
{
	// --- Engine functions --------------------------------------------------------------------------------------------------------------------------

	//Starts the krilloud instance.
	//Must be called before Krilloud can be used. It needs Krilloud_init after this call.
	//Returns: true if the operation was successful. False otherwise.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bool Krilloud_create(bool from_game, std::string contract_path, std::string app_guid = "");
#elif __ANDROID__
	int Krilloud_create(bool from_game, std::string app_guid, jobject asset_manager);
#endif

	//Destroys the krilloud instance.
	//Returns: 0 if the operation was successful. An error code otherwise.
	int Krilloud_destroy();

	//Initializes Krilloud. Must be called after Krilloud_create and before Krilloud can be used.
	//Returns: 0 if the operation was successful. -1 otherwise.
	int Krilloud_init(unsigned int flags, unsigned int backend, unsigned int sample_rate, unsigned int buffer_size, unsigned int channels);

	//Deprecated. Use Krilloud_destroy only.
	void Krilloud_deinit();

	// --- Contract functions ------------------------------------------------------------------------------------------------------------------------ 
	
	//TEMPORARY function. Sets the internal contract.
	void SetContractData(KrilloudProjectInfo data);

	// --- Tag functions -----------------------------------------------------------------------------------------------------------------------------

	//Loads a tag referenced to an object. The object is automatically created if it doesn't already exist.
	//Returns: number of loaded audio sources. If the tag is not found, returns -1 when working for the game, and -2 otherwise.
	int LoadTag(const char * tag_name, int object_id);
 
	//Starts playing a tag referenced to an object.
	//Returns: id of the new playing tag. Returns an error id otherwise.
	int PlayTag(const char * tag_name, int object_id);

	//Stops the tag with the given id.
	//Returns: 0 if the tag was found and stopped. -1 otherwise.
	int StopInstancedTag(int tag_id);

	//Stops the first instance tag with the given name for the reference object
	//Returns: 0 if the tag was found and stopped. -1 otherwise.
	int StopInstancedTag(std::string tag_name, int object_id);

	//Stops all the instanced tags with the given name for the reference object
	//Returns: the number of stopped instances.
	int StopAllInstancesOfTag(std::string tag_name, int object_id);

	//Stops all instanced tags in the system
	//Returns: the number of stopped instances.
	int StopAllInstancedTags();

	//Determines if a tag is being played
	//Returns: true if the tag is found and playing. False otherwise.
	bool isTagPlaying(std::string tag_name, int object_id);

	//Determines whether a tag with the given name is loaded or not
	//Returns: the tag index in the vector of loaded tags. -1 otherwise.
	int IsTagLoaded(std::string tag_name);

	//Renames a loaded tag
	//Returns: true if the tag was found and renamed. False otherwise.
	bool RenameTag(std::string old_name, std::string new_name);

	//Creates a new tag. If level is null, it creates a new one.
	//Returns: true if the tag was successfully created and loaded. False otherwise.
	bool CreateNewTag(std::string tag_name, KLevel* level_ptr = nullptr);

	//Creates a new tag.
	//Returns: true if the tag was successfully created and loaded. False otherwise.
	bool CreateNewTag(CompleteKrillTag tag_info);

	//Removes a loaded tag
	//Returns: 1 if the tag was successfully removed. 0 otherwise.
	int RemoveTag(std::string tag_name);

	//Sets the loop option of a loaded tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagLoopable(std::string tag_name, bool new_value);
	
	//Sets the channel of a loaded tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagChannel(std::string tag_name, int channel_id);

	//Sets the attenuation type of a loaded tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagAttenuation(std::string tag_name, int attenuation_type);

	//Sets the loaded tag to be used for 3D sounds or not
	//Returns: true if it was successfully changed. false otherwise
	bool SetTag3D(std::string tag_name, bool is_tag_3d);

	//Sets the distance delay of a loaded 3D tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagDistanceDelay(std::string tag_name, bool has_distance_delay);

	//Sets the doppler effect of a loaded 3D tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagDopplerEffect(std::string tag_name, bool is_doppler_active);

	//Sets the doppler factor of a loaded 3D tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagDopplerFactor(std::string tag_name, float doppler_factor);

	//Sets the minimum and maximum distance of a loaded 3D tag for it to be listened
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagMinMaxDistance(std::string tag_name, float min_distance, float max_distance);

	//Sets the roll off factor of a loaded 3D tag
	//Returns: true if it was successfully changed. false otherwise
	bool SetTagRollOffFactor(std::string tag_name, float roll_off_factor);

	//Retrieves the list of current playing tags
	//Returns: the list of current playing tags.
	//	If the system is using Real Time connection, it returns the list of tags being played in the server.
	std::vector<PlayingTag> GetPlayingTagsList();

	//Gets the total number of tags currently loaded
	//Returns: the current loaded tags count.
	int GetLoadedTagsCount();

	//Gets the total number of tags currently being player
	//Returns: the current playing tags count.
	int GetPlayingTagsCount();

	// --- KLevels and Nodes functions ---------------------------------------------------------------------------------------------------------------
	
	//TEMPORARY function. Creates a new level for a tag to be used.
	//Returns: the new created level.
	KLevel* CreateKLevel();

	//Adds a new node to a level
	//Returns: true if success. False otherwise.
	bool AddNode(KrilloudClass* class_to_add, KLevel* target_level);

	//Removes a node from a level
	//Returns: true if success. False otherwise.
	bool RemoveNode(KrilloudClass* class_to_add, KLevel* target_level);

	//Adds a new sub-level to a level
	//Returns: true if success. False otherwise.
	bool AddKLevel(KLevel* new_level, KLevel* target_level);

	//Removes a sub-level from a level
	//Returns: true if success. False otherwise.
	bool RemoveKLevel(KLevel* level_to_remove, KLevel* target_level);

	// -- Krilloud vars functions --------------------------------------------------------------------------------------------------------------------
	
	//Gets the value of a variable
	//Returns: the variable's current value. If it doesn't find it, it returns 0.
	float GetKrillVar(const char * variable_name, int owner_object_id);

	//Sets the value of a variable
	//Returns: true if the variable was successfully changed. False otherwise.
	bool SetKrillVar(const char * variable_name, int owner_object_id, float value);
	
	//Creates a new Krilloud Variable
	//Returns: the data structure of the created variable. Return null pointer if a variable with the same name already exists.
	VariableData* CreateNewKVariable(std::string var_name);

	//Creates a new Krilloud Variable
	//Returns: the data structure of the created variable. Return null pointer if a variable with the same name already exists.
	VariableData* CreateNewKVariable(VariableData var_data);

	//Removes a variable
	//Returns: true if the variable was successfully removed. False otherwise.
	bool RemoveKVariable(std::string var_name);
	
	//Removes a variable
	//Returns: true if the variable was successfully removed. False otherwise.
	bool RemoveKVariable(int var_index);
	
	//Sets a variable's name
	//Returns: true if success. False otherwise.
	bool SetVariableName(int var_index, std::string new_name);

	//Sets a variable's current value
	//Returns: true if success. False otherwise.
	bool SetVariableCurrentValue(int var_index, float new_value);

	//Sets a variable's type
	//Returns: true if success. False otherwise.
	bool SetVariableType(int var_index, VarType new_type);

	//Sets a variable's source type
	//Returns: true if success. False otherwise.
	bool SetVariableSource(int var_index, VarSourceType new_source_type);

	//Sets a variable's content type
	//Returns: true if success. False otherwise.
	bool SetVariableContent(int var_index, VarContentType new_content_type);

	//Sets a variable's minimum range value
	//Returns: true if success. False otherwise.
	bool SetVariableRangeMin(int var_index, float new_range_min);

	//Sets a variable's maximum range value
	//Returns: true if success. False otherwise.
	bool SetVariableRangeMax(int var_index, float new_range_max);

	//Sets a variable to be set by the system only once when the tag starts to be played
	//Returns: true if success. False otherwise.
	bool SetVariableOnPlay(int var_index, bool new_set_on_play);

	// --- GameObjects functions ---------------------------------------------------------------------------------------------------------------------
	
	//Sets the listener's current parameters.
	void SetListenerPosition(float position_x, float position_y, float position_z,
		float at_x = 0.0F, float at_y = 0.0F, float at_z = 0.0F,
		float up_x = 0.0F, float up_y = 0.0F, float up_z = 0.0F,
		float velocity_x = 0.0F, float velocity_y = 0.0F, float velocity_z = 0.0F);

	//Sets the object's position.
	//If the object doesn't exist, it creates it first.
	void SetObjectPosition(int object_id, float position_x, float position_y, float position_z, float velocity_x = 0.0F, float velocity_y = 0.0F, float velocity_z = 0.0F);

	//Creates a new object
	void CreateIngameObject(int object_id, float position_x = 0.0F, float position_y = 0.0F, float position_z = 0.0F, float velocity_x = 0.0F, float velocity_y = 0.0F, float velocity_z = 0.0F);

	//Destroys an object and all its associated tags.
	//Returns: the number of associated tags removed. -1 if the object wasn't found.
	int UnloadObject(int object_id);

	//Gets a list of current objects
	//Returns: the list of current objects in the system.
	//	If the system is using Real Time connection, it returns the list of objects found in the server.
	std::vector<IngameObject> GetIngameObjectList();

	// --- Channels functions ------------------------------------------------------------------------------------------------------------------------

	//Create a new audio channel
	//Returns: true if success. False otherwise.
	bool CreateNewChannel(std::string channel_name, int channel_id, int parent_id = 0);

	//Remove an audio channel
	//Returns: true if success. False otherwise.
	bool RemoveChannel(int channel_id);

	//Gets data from audio channel in the given index
	//Returns: pointer to channel data. Null pointer otherwise
	Channel* GetChannel(int channel_index);

	//Gets data from audio channel with the given id
	//Returns: pointer to channel data. Null pointer otherwise
	Channel* GetChannelByID(int channel_id);

	//Gets the current total number of channels
	//Returns: channel list count
	int GetChannelCount();

	//Gets the name of a channel
	//Return: The channel's name. Empty string otherwise.
	std::string GetChannelName(int channel_index);

	//Gets the volume of a channel
	//Returns: the current channel's volume. 0 otherwise.
	float GetChannelVolume(int channel_index);

	//Gets the volume of a channel
	//Returns: the current channel's volume. 0 otherwise.
	float GetChannelVolume(std::string channel_name);

	//Gets the overall volume of a channel
	//Returns: the current overall channel's volume. 0 otherwise.
	float GetChannelOverallVolume(int channel_index);

	//Changes the parent id of a channel
	//Returns: true if success. False otherwise.
	bool ChangeChannelParent(int channel_id, int parent_id);
	
	//Changes the name of a channel
	//Returns: true if success. False otherwise.
	bool ChangeChannelName(int channel_id, std::string new_name);

	//Changes the volume of a channel
	void ChangeChannelVolume(int channel_id, float volume);

	//Changes the volume of a channel
	void ChangeChannelVolume(std::string channel_name, float volume);
	
	// --- KClass functions -----------------------------------------------------------------------------------------------------------------------------

	//Creates a new internal node class
	//Returns: a pointer to the created class. Null pointer otherwise.
	KrilloudClass* CreateKClass(int class_type, std::string class_name, int class_id, std::vector<float> initial_parameters, int object_id);

	// --- Auxiliary functions --------------------------------------------------------------------------------------------------------------------------
	
	//Gets the current status of Krilloud
	//Returns: true if an instance of Krilloud is active and ready to be used. False otherwise.
	bool IsKrilloudOn();

	//Changes the pause state of the system
	void SwitchSystemPause();

	//Gets the play status of Krilloud
	//Returns: true if krilloud is paused. False otherwise.
	bool IsKrilloudPaused();

	//Gets the number of current active (playing) voices
	//Returns: the current number of playing voices. -1 otherwise.
	int GetActiveVoicesCount();

	//Gets the number of current voices
	//Returns: the current number of voices. -1 otherwise.
	int GetVoiceCount();
	
	//Gets the current maximum allowed number of voices
	//Returns: the current maximum allowed number of voices. -1 otherwise.
	int GetMaxVoices();

	//Gets the contract's information in JSON format
	//Returns: a string with the contract data in JSON format. If there's any error, it returns a string with the error data.
	std::string GetContractJSON(std::string contract_path);
}

namespace KrilloudSerial 
{
	bool Validation(std::string token, std::string token_path, int response);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bool ReadKrilloudValidation(std::string token_path, std::string guid);
#elif __ANDROID__
	bool ReadKrilloudValidation(const char* token_path, std::string guid, AAssetManager* mgr);
#endif
}

namespace KrilloudDebug 
{
	//Sets the active status of the debug mode
	void ActivateDebug(bool active);

	//Gets the current status of the debug mode
	//Returns: true if the debug mode is active. False otherwise.
	bool GetDebugStatus();
}

namespace KrilloudRT
{
	// --- General --------------------------------------------------------------------------------------------------------------------------------------

	//Retrieves whether the system is currently using Real time or not
	//Returns: true if the system is connected as client or as a server. False otherwise.
	bool IsRealTimeOn();

	// --- Server functions -----------------------------------------------------------------------------------------------------------------------------
	
	//Sets the server's port for the next time a server is created.
	//Server port can only be a number in the range [0 .. 65535]
	//Returns: true if success. False otherwise.
	bool SetServerPort(int port);

	//Retrieves the current set port for the server.
	//Returns: the current server port.
	int GetServerPort();

	//Starts a new server instance. If there's another server created or a client running, the system won't create a new server.
	//Returns: A data pair containing true if the server was successfully created (false otherwise), and a string with error data, if there was any.
	std::pair<bool, const char*> StartServer();
	
	//Stops the server instance.
	//Returns: A data pair containing true if the server was successfully stopped (false otherwise), and a string with error data, if there was any.
	std::pair<bool, const char*> StopServer();

	//Retrieves whether a server instance is created or not.
	//Returns: true if the system already has a server created. False otherwise.
	bool IsServerActive();

	//Sets the system to automatically start and stop the server when the system starts. If the system is already started and there's no server active, it starts one.
	//Returns: true if the server is set to be automatically turned on/off. False otherwise.
	bool SetServerOnPlay(bool value);

	//Retrieves whether the system has to automatically start/stop a server or not.
	//Returns: true if the server is set to be automatically turned on/off. False otherwise.
	bool IsServerSetOnPlay();

	// --- Client functions -----------------------------------------------------------------------------------------------------------------------------
	
	//Tries to connect to the given ip and port
	//Port has to be a number in the range [0 .. 65535]
	void Connect(std::string ip, std::string port);

	//Closes the current connection, if there's any
	void Disconnect();

	//Retrieves whether the client is connected or not
	//Returns: true if the client is currently connected. False otherwise.
	bool IsConectionActive();

	// --- Network updates ------------------------------------------------------------------------------------------------------------------------------
	
	//Retrieves the last updated contract obtained from the server
	//Returns: The contract from the server. An empty contract otherwise.
	KrilloudProjectInfo GetContractData();
	
	//Sends a notification about a filter parameter being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyFilterParameter(int target_tag_index, int target_node_id, int parameter_index, float new_value);

	//Sends a notification about a channel's name being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyChannelName(int channel_id, std::string new_channel_name);

	//Sends a notification about a channel's parent id being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyChannelParentID(int channel_id, int new_parent_id);

	//Sends a notification about a channel's volume being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyChannelVolume(int channel_id, float new_volume);

	//Sends a notification about a variable name being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableName(int variable_position, std::string new_variable_name);

	//Sends a notification about a variable's current value being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableCurrentValue(int variable_position, float new_current_value);

	//Sends a notification about a variable's type being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableType(int variable_position, VarType new_variable_type);

	//Sends a notification about a variable's source type being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableSource(int variable_position, VarSourceType new_source_type);

	//Sends a notification about a variable's content type being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableContent(int variable_position, VarContentType new_content_type);

	//Sends a notification about a variable's minimum range being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableRangeMin(int variable_position, float new_range_min);

	//Sends a notification about a variable's maximum range being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableRangeMax(int variable_position, float new_range_max);

	//Sends a notification about a variable's "set on play" parameter being changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyVariableOnlyOnPlay(int variable_position, bool new_set_only_on_play);

	//Send a notification about a mixer's volume parameter changed
	//Returns: true if the notification was delivered. False otherwise.
	bool NotifyMixerVolumeChange(int target_tag_index, int target_node_id, int parameter_index, float new_volume);
}