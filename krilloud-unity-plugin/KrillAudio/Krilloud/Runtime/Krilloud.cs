using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace KrillAudio.Krilloud
{
	public sealed class Krilloud
	{
#if UNITY_EDITOR_OSX || UNITY_STANDALONE_OSX
		public const string KRILLOUD_LIB = "Krilloud_Macos";
#elif UNITY_STANDALONE_WIN
#if UNITY_64
		public const string KRILLOUD_LIB = "Krilloud_x64";
#else
		public const string KRILLOUD_LIB = "Krilloud_x86";
#endif
#elif UNITY_ANDROID
		public const string KRILLOUD_LIB = "Krilloud";
#elif UNITY_IOS
		public const string KRILLOUD_LIB = "Krilloud_iOS"; //"Krilloud_ARM64_Debug";
#endif

		public const int AUTO = 0;
		public const int SDL1 = 1;
		public const int SDL2 = 2;
		public const int PORTAUDIO = 3;
		public const int WINMM = 4;
		public const int XAUDIO2 = 5;
		public const int WASAPI = 6;
		public const int ALSA = 7;
		public const int OSS = 8;
		public const int OPENAL = 9;
		public const int COREAUDIO = 10;
		public const int OPENSLES = 11;
		public const int VITA_HOMEBREW = 12;
		public const int NULLDRIVER = 13;
		public const int BACKEND_MAX = 14;

		public const int CLIP_ROUNDOFF = 1;
		public const int ENABLE_VISUALIZATION = 2;
		public const int LEFT_HANDED_3D = 4;

		#region ENGINE
#if UNITY_EDITOR || UNITY_STANDALONE_WIN
		[DllImport(KRILLOUD_LIB)]
		internal static extern IntPtr EX_Krilloud_Create([MarshalAs(UnmanagedType.LPStr)] string contract_folder_path, [MarshalAs(UnmanagedType.LPStr)] string app_guid);
		public Krilloud(string contract_folder_path)
		{
			EX_Krilloud_Create(contract_folder_path, Application.buildGUID);
		}
#elif UNITY_ANDROID
		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_Create(IntPtr asset_manager, [MarshalAs(UnmanagedType.LPStr)] string app_guid);
        public Krilloud(string contract_folder_path)
        {
			IntPtr cls_Activity = (IntPtr)AndroidJNI.FindClass("com/unity3d/player/UnityPlayer");
			IntPtr fid_Activity = AndroidJNI.GetStaticFieldID(cls_Activity, "currentActivity", "Landroid/app/Activity;");
			IntPtr obj_Activity = AndroidJNI.GetStaticObjectField(cls_Activity, fid_Activity);

			IntPtr obj_cls = AndroidJNI.GetObjectClass(obj_Activity);
			IntPtr asset_func = AndroidJNI.GetMethodID(obj_cls, "getAssets", "()Landroid/content/res/AssetManager;");
			jvalue[] asset_array = new jvalue[2];
			IntPtr assetManager = AndroidJNI.CallObjectMethod(obj_Activity, asset_func, asset_array);
			if (assetManager != null)
			{
				EX_Krilloud_Create(assetManager, Application.buildGUID);
			}	
        }
#elif UNITY_IOS
		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern IntPtr EX_Krilloud_Create([MarshalAs(UnmanagedType.LPStr)] string contract_folder_path, [MarshalAs(UnmanagedType.LPStr)] string app_guid);
		public Krilloud(string contract_folder_path)
		{
			EX_Krilloud_Create(Application.dataPath + "/Raw/KrilloudData", Application.buildGUID);
		}
#else
		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern IntPtr EX_Krilloud_Create([MarshalAs(UnmanagedType.LPStr)] string contract_folder_path, [MarshalAs(UnmanagedType.LPStr)] string app_guid);
		public Krilloud(string contract_folder_path)
		{
			EX_Krilloud_Create(contract_folder_path, Application.buildGUID);
		}
#endif

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern IntPtr EX_Krilloud_Destroy();

		~Krilloud()
		{
			EX_Krilloud_Destroy();
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_Init(uint aFlags, uint aBackend, uint aSamplerate, uint aBufferSize, uint aChannels);

		public int Init(uint aFlags = CLIP_ROUNDOFF, uint aBackend = AUTO, uint aSamplerate = AUTO, uint aBufferSize = AUTO, uint aChannels = 2)
		{
#if UNITY_EDITOR || UNITY_STANDALONE_WIN
			return EX_Krilloud_Init(aFlags, aBackend, aSamplerate, aBufferSize, aChannels);
#elif UNITY_ANDROID
			return EX_Krilloud_Init(aFlags, 12, aSamplerate, aBufferSize, aChannels);
#elif UNITY_IOS
			return EX_Krilloud_Init(1,0,0,0,2);
#else
			return EX_Krilloud_Init(aFlags, aBackend, aSamplerate, aBufferSize, aChannels);
#endif
		}

			[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_DeInit();

		public void Deinit()
		{
			EX_Krilloud_DeInit();
		}

#endregion ENGINE

#region TAGS

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_Load([In] string[] tag_list, int length, int object_id);

		public int LoadTag(string[] tags, int objectId)
		{
			return EX_Krilloud_Load(tags, tags.Length, objectId);
		}

		//[DllImport(KRILLOUD_X64_DLL, CallingConvention = CallingConvention.Cdecl)]
		//internal static extern int EX_Krilloud_Unload([In] string[] tag_list, int length, int object_id);
		//public int UnloadTag(string[] tag_list, int object_id)
		//{
		//	return EX_Krilloud_Unload(tag_list, tag_list.Length, object_id);
		//}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_Unload(int objectId);

		public int UnloadObject(int object_id)
		{
			return EX_Krilloud_Unload(object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_Play([MarshalAs(UnmanagedType.LPStr)] string tag_name, int object_id);

		public int Play(string tag_name, int object_id)
		{
			return EX_Krilloud_Play(tag_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_StopTagID(int tag_id);

		public int Stop(int tag_id)
		{
			return EX_Krilloud_StopTagID(tag_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_StopTag([MarshalAs(UnmanagedType.LPStr)] string tag_name, int object_id);

		public int Stop(string tag_name, int object_id)
		{
			return EX_Krilloud_StopTag(tag_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_StopAllInstances([MarshalAs(UnmanagedType.LPStr)] string tag_name, int object_id);

		public int StopAllIstancesOfTag(string tag_name, int object_id)
		{
			return EX_Krilloud_StopAllInstances(tag_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_StopAll();

		public int StopAll()
		{
			return EX_Krilloud_StopAll();
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool EX_Krilloud_IsTagPlaying([MarshalAs(UnmanagedType.LPStr)] string tag_name, int object_id);

		public bool IsPlaying(string tag_name, int object_id)
		{
			return EX_Krilloud_IsTagPlaying(tag_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_Pause();

		public void PauseKrilloud()
		{
			EX_Krilloud_Pause();
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool EX_Krilloud_IsPaused();

		public bool IsKrilloudPaused()
		{
			return EX_Krilloud_IsPaused();
		}

#endregion TAGS

#region KRILLOUD VARS

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_Krilloud_ReadKVar_INT([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id);

		public void GetVariableValue(string variable_name, int object_id, ref int value)
		{
			value = EX_Krilloud_ReadKVar_INT(variable_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern float EX_Krilloud_ReadKVar_FLOAT([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id);

		public void GetVariableValue(string variable_name, int object_id, ref float value)
		{
			value = EX_Krilloud_ReadKVar_FLOAT(variable_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool EX_Krilloud_ReadKVar_BOOL([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id);

		public void GetVariableValue(string variable_name, int object_id, ref bool value)
		{
			value = EX_Krilloud_ReadKVar_BOOL(variable_name, object_id);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_UpdateKVarValue_INT([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id, int value);

		public void SetVariableValue(string variable_name, int object_id, int value)
		{
			EX_Krilloud_UpdateKVarValue_INT(variable_name, object_id, value);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_UpdateKVarValue_FLOAT([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id, float value);

		public void SetVariableValue(string variable_name, int object_id, float value)
		{
			EX_Krilloud_UpdateKVarValue_FLOAT(variable_name, object_id, value);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_UpdateKVarValue_BOOL([MarshalAs(UnmanagedType.LPStr)] string variable_name, int object_id, bool value);

		public void SetVariableValue(string variable_name, int object_id, bool value)
		{
			EX_Krilloud_UpdateKVarValue_BOOL(variable_name, object_id, value);
		}

#endregion KRILLOUD VARS

#region UPDATE POSITIONS

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_UpdateListenerPosition(float aPosX, float aPosY, float aPosZ,
																	   float aAtX, float aAtY, float aAtZ,
																	   float aUpX, float aUpY, float aUpZ,
																	   float aVelocityX, float aVelocityY, float aVelocityZ);

		public void SetListenerPosition(float aPosX, float aPosY, float aPosZ,
										   float aAtX = 0.0f, float aAtY = 0.0f, float aAtZ = 0.0f,
										   float aUpX = 0.0f, float aUpY = 0.0f, float aUpZ = 0.0f,
										   float aVelocityX = 0.0f, float aVelocityY = 0.0f, float aVelocityZ = 0.0f)
		{
			EX_Krilloud_UpdateListenerPosition(aPosX, aPosY, aPosZ, aAtX, aAtY, aAtZ, aUpX, aUpY, aUpZ, aVelocityX, aVelocityY, aVelocityZ);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_Krilloud_UpdateObjectPosition(int object_id, float position_x, float position_y, float position_z,
																	 float aVelX, float aVelY, float aVelZ);

		public void SetGameObjectPosition(int object_id,
											 float position_x, float position_y, float position_z,
											 float aVelX = 0.0f, float aVelY = 0.0f, float aVelZ = 0.0f)
		{
			EX_Krilloud_UpdateObjectPosition(object_id, position_x, position_y, position_z, aVelX, aVelY, aVelZ);
		}

#endregion UPDATE POSITIONS

#region CHANNELS

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern int EX_GetChannelCount();

		public int ChannelsCount()
		{
			return EX_GetChannelCount();
		}

		/*[DllImport(KRILLOUD_X64_DLL, CallingConvention = CallingConvention.Cdecl)]
		internal static extern float EX_GetChannelVolume(int position);

		public float GetChannelVolume(int position)
		{
			return EX_GetChannelVolume(position);
		}

		[DllImport(KRILLOUD_X64_DLL, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_ChangeChannelVolume(int position, float volume);

		public void SetChannelVolume(int position, float volume)
		{
			EX_ChangeChannelVolume(position, volume);
		}*/

        [DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern float EX_GetChannelVolumeByName([MarshalAs(UnmanagedType.LPStr)] string channel_name);
        public float GetChannelVolume(string channel_name)
        {
            return EX_GetChannelVolumeByName(channel_name);
        }

        [DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
        internal static extern void EX_ChangeChannelVolumeByName([MarshalAs(UnmanagedType.LPStr)] string channel_name, float volume);
        public void SetChannelVolume(string channel_name, float volume)
        {
            EX_ChangeChannelVolumeByName(channel_name, volume);
        }

#endregion CHANNELS
       
#region DEBUG

        [DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_SetDebugStatus(bool debug);

		public void SetDebugStatus(bool debug)
		{
			EX_SetDebugStatus(debug);
		}

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern bool EX_GetDebugStatus();

		public bool GetDebugStatus()
		{
			return EX_GetDebugStatus();
		}

		#endregion DEBUG

		[DllImport(KRILLOUD_LIB, CallingConvention = CallingConvention.Cdecl)]
		internal static extern void EX_GetContractData(string contract_path);

		public void GetContractData(string contract_path)
		{
			EX_GetContractData(contract_path);
		}
	}
    
}