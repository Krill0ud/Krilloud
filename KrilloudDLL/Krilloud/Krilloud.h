namespace ExternKrilloud
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define EXPORT __declspec(dllexport)
	extern "C" EXPORT void EX_Krilloud_Create(const char* contract_path, const char* app_guid);
#ifdef _WIN64
	//define something for Windows (64-bit only)
#else
	//define something for Windows (32-bit only)
#endif	
#elif __ANDROID__
#define EXPORT
	extern "C" EXPORT int EX_Krilloud_Create(jobject asset_manager, const char* app_guid);
#elif __APPLE__
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR
	// iOS Simulator
#elif TARGET_OS_IPHONE
	// iOS device
#elif TARGET_OS_MAC
	// Other kinds of Mac OS
#else
#   error "Unknown Apple platform"
#endif
#elif __linux__
	// linux
#elif __unix__ // all unices not caught above
	// Unix
#elif defined(_POSIX_VERSION)
	// POSIX
#else
#   error "Unknown compiler"
	return false;
#endif
	// --- Engine functions
	extern "C" EXPORT int EX_Krilloud_Destroy();
	extern "C" EXPORT int EX_Krilloud_Init(unsigned int aFlags, unsigned int aBackend, unsigned int aSamplerate, unsigned int aBufferSize, unsigned int aChannels);
	extern "C" EXPORT void EX_Krilloud_DeInit();
	
	// --- Tag functions
	extern "C" EXPORT int EX_Krilloud_Load(const char** tag_list, int length, int object_id);
	extern "C" EXPORT int EX_Krilloud_Unload(int object_id);
	extern "C" EXPORT int EX_Krilloud_Play(const char * tagname, int object_id);
	extern "C" EXPORT int EX_Krilloud_StopTagID(int tag_id);
	extern "C" EXPORT int EX_Krilloud_StopTag(const char * tagname, int object_id);
	extern "C" EXPORT int EX_Krilloud_StopAllInstances(const char * tagname, int object_id);
	extern "C" EXPORT int EX_Krilloud_StopAll();
	extern "C" EXPORT bool EX_Krilloud_IsTagPlaying(const char * tagname, int object_id);

	extern "C" EXPORT void EX_Krilloud_Pause();
	extern "C" EXPORT bool EX_Krilloud_IsPaused();

	// --- Krilloud vars functions
	//Variable reading
	extern "C" EXPORT int EX_Krilloud_ReadKVar_INT(const char * variable_name, int object_id);
	extern "C" EXPORT float EX_Krilloud_ReadKVar_FLOAT(const char * variable_name, int object_id);
	extern "C" EXPORT bool EX_Krilloud_ReadKVar_BOOL(const char * variable_name, int object_id);

	//Variable updating
	extern "C" EXPORT void EX_Krilloud_UpdateKVarValue_INT(const char * variable_name, int object_id, int value);
	extern "C" EXPORT void EX_Krilloud_UpdateKVarValue_FLOAT(const char * variable_name, int object_id, float value);
	extern "C" EXPORT void EX_Krilloud_UpdateKVarValue_BOOL(const char * variable_name, int object_id, bool value);

	// --- GameObjects position functions
	extern "C" EXPORT void EX_Krilloud_UpdateListenerPosition(float aPosX, float aPosY, float aPosZ,
																		     float aAtX, float aAtY, float aAtZ,
																			 float aUpX, float aUpY, float aUpZ,
																			 float aVelocityX, float aVelocityY, float aVelocityZ);
	extern "C" EXPORT void EX_Krilloud_UpdateObjectPosition(int object_id, float position_x, float position_y, float position_z,
																		   float aVelX, float aVelY, float aVelZ);
	// --- Channels functions
	extern "C" EXPORT int EX_GetChannelCount();
	extern "C" EXPORT float EX_GetChannelVolume(int position);
	extern "C" EXPORT float EX_GetChannelVolumeByName(const char * channel_name);
	extern "C" EXPORT void EX_ChangeChannelVolume(int position, float volume);
	extern "C" EXPORT void EX_ChangeChannelVolumeByName(const char * channel_name, float volume);

	// --- CEREAL functions
	extern "C" EXPORT const char* EX_GetContractData(const char* contract_path);

	// --- Debug functions
	extern "C" EXPORT void EX_SetDebugStatus(bool debug_on);
	extern "C" EXPORT bool EX_GetDebugStatus();


	// --- Real Time
	extern "C" EXPORT bool EX_StartServer();
	extern "C" EXPORT bool EX_StoptServer();
	extern "C" EXPORT bool EX_IsKrilloudServerActive();
	extern "C" EXPORT bool EX_SetServerPort(int server_port);
	extern "C" EXPORT int EX_GetServerPort();
	extern "C" EXPORT bool EX_KeepServerOnlyOnPlay(bool only_on_play);

}