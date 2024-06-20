#ifndef KRILLOUD_NODE_INFO_STRUCT
#define KRILLOUD_NODE_INFO_STRUCT

#include "KrilloudDefinitions.h"
#include <vector>
#include <string>
#include <string>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#undef max
#undef min




class KrilloudClass;
class KrilloudFilter;
class KrilloudVar;
class KrilloudMixer;

class AudioEngineSource;
class AudioEngineFilter;

static int current_version = 20210113; // Release 1.0
static int version_1_1_1 = 20201222;
static int version_RT = 20200928;
static int version_pre_RT = 20191030;


//Information structure to store the results of a tag about to be played
struct TagInfo
{
	AudioEngineSource* source;
	std::vector<KrilloudMixer*> mixers;
	std::vector<KrilloudVar*> kvars;
};

// --- Audio Pool
struct pool_file_data
{
	int id;
	std::string file_name;
};

struct directory_system
{
	std::string folder_name;
	std::vector<std::string> files;
	std::vector<directory_system*> directories;
};

// --- Node Levels

// Information and instances of all elements loaded in a tag
struct KLevel
{
	int associated_macro_id;
	std::vector<KrilloudClass*> nodes;
	std::vector<KLevel*> sublevels;
	KLevel* parent;
};

// Tag information
struct KrillTag
{
	std::string tag_name;
	int owner_id;

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

// A tag is complete when it has all its info and elements instanced
struct CompleteKrillTag
{
	KrillTag tag;
	KLevel* tag_content;
};

// Information about a sound file that can be loaded and played
struct AudioSourceContent
{
	int id;
	std::string filename;
	float volume;
};

// Information about a loaded sound file
struct LoadedSound
{
	int id;
	std::vector<std::string> owners;
	AudioEngineSource* source;
};

#endif //KRILLOUD_NODE_INFO_STRUCT