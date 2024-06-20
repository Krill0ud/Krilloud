#ifndef __ANDROID__
#include "Krilloud.h"
#endif
#include "NodeInfoStructures.h"
#include "PublicStaticKrilloud.h"

using namespace KrilloudEngine;

extern "C" 
{
	namespace ExternKrilloud
	{
		// --- Engine functions
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
		void EX_Krilloud_Create(const char* contract_path, const char* app_guid)
		{
			Krilloud_create(true, contract_path, app_guid);
		}
#elif __ANDROID__
		int EX_Krilloud_Create(jobject asset_manager, const char* app_guid)
		{
			return Krilloud_create(true, app_guid, asset_manager);
		}
#endif
		int EX_Krilloud_Destroy()
		{
			return Krilloud_destroy();
		}

		int EX_Krilloud_Init(unsigned int aFlags, unsigned int aBackend, unsigned int aSamplerate, unsigned int aBufferSize, unsigned int aChannels)
		{
			return Krilloud_init(aFlags, aBackend, aSamplerate, aBufferSize, aChannels);
		}

		void EX_Krilloud_DeInit() 
		{
			Krilloud_deinit();
		}

		// --- Tag functions
		int EX_Krilloud_Load(const char** tag_list, int length, int object_id)
		{
			int index = 0;

			for (int i = 0; i < length; ++i) 
			{
				index += LoadTag(tag_list[i], object_id);
			}

			return index;
		}

		int EX_Krilloud_Unload(int object_id)
		{
			return UnloadObject(object_id);
		}

		int EX_Krilloud_Play(const char * tagname, int object_id)
		{
			return PlayTag(tagname, object_id);
		}

		int EX_Krilloud_StopTagID(int tag_id)
		{
			return StopInstancedTag(tag_id);
		}

		int EX_Krilloud_StopTag(const char * tagname, int object_id)
		{
			return StopInstancedTag(tagname, object_id);
		}

		int EX_Krilloud_StopAllInstances(const char * tagname, int object_id)
		{
			return StopAllInstancesOfTag(tagname, object_id);
		}

		int EX_Krilloud_StopAll()
		{
			return StopAllInstancedTags();
		}

		bool EX_Krilloud_IsTagPlaying(const char * tagname, int object_id) 
		{
			return isTagPlaying(tagname, object_id);
		}

		void EX_Krilloud_Pause() 
		{
			SwitchSystemPause();
		}

		bool EX_Krilloud_IsPaused() 
		{
			return IsKrilloudPaused();
		}

		// --- Krilloud vars functions

		//Variable reading
		int EX_Krilloud_ReadKVar_INT(const char * variable_name, int object_id)
		{
			return ((int)GetKrillVar(variable_name, object_id));
		}

		float EX_Krilloud_ReadKVar_FLOAT(const char * variable_name, int object_id)
		{
			return GetKrillVar(variable_name, object_id);
		}

		bool EX_Krilloud_ReadKVar_BOOL(const char * variable_name, int object_id)
		{
			return ((int)GetKrillVar(variable_name, object_id) == 1);
		}

		//Variable updating
		void EX_Krilloud_UpdateKVarValue_INT(const char * variable_name, int object_id, int value)
		{
			SetKrillVar(variable_name, object_id, (float)value);
		}

		void EX_Krilloud_UpdateKVarValue_FLOAT(const char * variable_name, int object_id, float value)
		{
			SetKrillVar(variable_name, object_id, value);
		}
				
		void EX_Krilloud_UpdateKVarValue_BOOL(const char * variable_name, int object_id, bool value)
		{
			SetKrillVar(variable_name, object_id, value ? 1 : 0);
		}

		// --- GameObjects position functions
		void EX_Krilloud_UpdateListenerPosition(float aPosX, float aPosY, float aPosZ,
												float aAtX, float aAtY, float aAtZ,
												float aUpX, float aUpY, float aUpZ,
												float aVelocityX, float aVelocityY, float aVelocityZ)
		{
			SetListenerPosition(aPosX, aPosY, aPosZ, aAtX, aAtY, aAtZ, aUpX, aUpY, aUpZ, aVelocityX, aVelocityY, aVelocityZ);
		}

		void EX_Krilloud_UpdateObjectPosition(int object_id, float position_x, float position_y, float position_z,
											  float aVelX, float aVelY, float aVelZ)
		{
			SetObjectPosition(object_id, position_x, position_y, position_z, aVelX,  aVelY, aVelZ);
		}

		// --- Channels functions

		int EX_GetChannelCount() 
		{
			return GetChannelCount();
		}

		float EX_GetChannelVolume(int position) 
		{
			return GetChannelVolume(position);
		}
		
		float EX_GetChannelVolumeByName(const char * channel_name)
		{
			return GetChannelVolume(channel_name);
		}

		void EX_ChangeChannelVolume(int position, float volume) 
		{
			if (position >= 0 && position < GetChannelCount())
			{
				Channel* channel = GetChannel(position);
				
				if (channel == nullptr)
					return;

				ChangeChannelVolume(channel->channel_id, volume);
			}
		}
		
		void EX_ChangeChannelVolumeByName(const char * channel_name, float volume)
		{
			ChangeChannelVolume(channel_name, volume);
		}

		// --- CEREAL functions

		const char* EX_GetContractData(const char* contract_path)
		{
			return GetContractJSON(contract_path).c_str();
		}

		// --- Debug functions
		void EX_SetDebugStatus(bool debug_on) 
		{
			KrilloudDebug::ActivateDebug(debug_on);
		}

		bool EX_GetDebugStatus() 
		{
			return KrilloudDebug::GetDebugStatus();
		}

		// --- Real Time
		bool EX_StartServer()
		{
			return KrilloudRT::StartServer().first;
		}

		bool EX_StoptServer()
		{
			return KrilloudRT::StopServer().first;
		}

		bool EX_IsKrilloudServerActive() 
		{
			return KrilloudRT::IsServerActive();
		}
		
		bool EX_SetServerPort(int server_port) 
		{
			return KrilloudRT::SetServerPort(server_port);
		}

		int EX_GetServerPort()
		{
			return KrilloudRT::GetServerPort();
		}

		bool EX_KeepServerOnlyOnPlay(bool only_on_play)
		{
			return KrilloudRT::SetServerOnPlay(only_on_play);
		}

	}
}
