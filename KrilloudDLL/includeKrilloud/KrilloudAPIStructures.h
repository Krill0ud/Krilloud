#ifndef KRILLOUDAPI_STRUCTURES_H
#define KRILLOUDAPI_STRUCTURES_H
//#include <vector>
#include "KrilloudDefinitions.h"
#include <cstddef>

// --- Filter parameters
struct KLInfo 
{
	bool first;
	const char* second;
};

struct KLEngineInfo 
{
	const char* item_name;
	int item_id;
};

struct KLPair 
{
	float first;
	float second;
};

struct FilterParameter
{
	const char* name;
	float value;
	float min_value;
	float max_value;
};

// --- Audio Pool
struct KrilloudPoolFile
{
	int id;
	const char* full_file_name;
	const char* file_name;
	const char** folder_tree;
	int folder_tree_length;
};

//struct KrilloudPool
//{
//	const char* folder_name;
//	std::vector<const char*> files;
//	std::vector<KrilloudPool> directories;
//};

struct KrilloudIngameObject
{
	int object_id;
	float position_x;
	float position_y;
	float position_z;
	float aVelX;
	float aVelY;
	float aVelZ;
};

struct KrilloudPlayingTag
{
	size_t id;
	int parent_object_id;
	const char* tagname;
	int playHandle;
	int channel_id;
	bool is3D;
	bool stop;
};

struct KrilloudEngineSettings
{
	SupportedPlatform platform;
	AudioEngineBackend backend;
	AudioEngineSampleRate sample_rate;
	AudioEngineBuffer buffer_size;
};

struct KrilloudChannel
{
	int channel_id;
	const char* channel_name;
	int parent_id;
	float volume;
};

struct KrilloudTag 
{
	const char* tag_name;

	bool is3D;
	float min_3D_distance;
	float max_3D_distance;
	float roll_off_factor;
	int attenuation;
	bool distance_delay;
	bool doppler_active;
	float doppler_factor;

	int channel_id;

	bool isLoopable;
};

struct KrilloudVariable
{
	const char* variable_name;
	float current_value;
	VarType type;
	VarSourceType source;
	VarContentType content;
	float range_min;
	float range_max;
	bool set_only_on_play;

	const char* previous_name;
};

struct KrilloudAudioSource
{
	int id;
	const char* filename;
	float volume;
};

struct KrilloudNodeConnection
{
	int my_socket_id;
	int connected_node_id;
	int connected_node_socket_id;
};

struct KrilloudNodeSocket
{
	int socket_id;
	SocketType connection_type;
	SocketDataType data_type;
	int links_array_length;
	KrilloudNodeConnection* links;	
};

struct KrilloudNode {
	int			type;
	int			id;
	const char*	name;
	float		pos_x;
	float		pos_y;
	float		size_x;
	float		size_y;
	bool		can_be_deleted;
	int			inputs_count;
	int			outputs_count;

	int sockets_list_length;
	KrilloudNodeSocket* sockets;
};

#endif //KRILLOUDAPI_STRUCTURES_H
