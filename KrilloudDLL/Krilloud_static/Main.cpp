#include "Main.h"
#include <experimental/filesystem>
#include <fstream>
#include <sstream>

#include "KrilloudDebug.h"
#include "KrilloudRT.h"

#include "SoloudAPI.h"

#ifdef __ANDROID__
jobject my_asset_manager;
JNIEnv* jni_env;
AAssetManager* mgr;

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	vm->AttachCurrentThread(&jni_env, 0);
	return JNI_VERSION_1_6;
}
#endif

namespace KrilloudEngine
{
	std::string local_variable_separator = "_$%KV%$_";

	// --- Working variables
	///General
	AudioEngine* engine_instance;
	bool working_on_game = false;
	bool closing_instance_mode = false;
	bool engine_paused = false;
	int last_given_id = 0;
	int last_given_channel_id = 0;
	bool is_Krilloud_ready = false;



	///Loading
	IngameObject* listener_object;
	std::vector<IngameObject*> session_loaded_objects;
	std::vector<LoadedSound*> session_loaded_audios;
	std::vector<VariableData*> session_variables;
	
	///Channels
	std::vector<Channel*> channels;

	///Contract
	std::string contract_full_path;
	KrilloudProjectInfo original_contract;
	KrilloudProjectInfo contract;

	///Tags
	std::vector<CompleteKrillTag> my_tags;
	std::vector<PlayingTag*> playing_tags;
	std::thread* playing_tags_checker;
	std::thread* volumes_checker;

	///Server side lists
	IngameObject* server_listener_object;
	std::vector<IngameObject*> server_session_loaded_objects;
	std::vector<PlayingTag> server_playing_tags;

	// --- Thread functions -----------------------------------------------------------------------------------------------------------------------------
	
	//INTERNAL
	void Thread_VolumeUpdater(std::vector<PlayingTag*>* playing_tags_list) 
	{
		using namespace std::literals::chrono_literals;

		std::vector<PlayingTag*>* tags_list = playing_tags_list;

		//Thread constantly working until Krilloud instance stops
		while (!closing_instance_mode && engine_instance != nullptr)
		{
			size_t index = 0;

			//Review the whole list of tags being played
			while (index < tags_list->size())
			{
				if (closing_instance_mode)
				{
					break;
				}

				PlayingTag* pTag = (*tags_list)[index];

				if (pTag != nullptr && pTag->info->source != nullptr)
				{
					if (!pTag->stop) 
					{
						TagInfo* tag = pTag->info;

						// Update audio volume
						for (size_t x = 0; x < tag->mixers.size(); x++)
						{
							tag->mixers[x]->UpdateVolumeValues();
						}
						float volume = tag->source->volume * GetChannelOverallVolume(pTag->channel_id);
						engine_instance->SetVolume(tag->source, volume);
					}
				}

				index++;
			}

			//Wait until the next iteration (otherwise the system crashes)
			std::this_thread::sleep_for(0.01s);
		}
	}

	//INTERNAL
	void Thread_PlayUpdater(std::vector<PlayingTag*>* playing_tags_list)
	{
		using namespace std::literals::chrono_literals;

		std::vector<PlayingTag*>* tags_list = playing_tags_list;
		
		//Thread constantly working until Krilloud instance stops
		while (!closing_instance_mode && engine_instance != nullptr)
		{
			size_t index = 0;

			//Review the whole list of tags being played
			while (index < tags_list->size())
			{
				if (closing_instance_mode)
				{
					break;
				}

				PlayingTag* pTag = (*tags_list)[index];

				if (pTag != nullptr)
				{
					if (pTag->info->source == nullptr) 
					{
						pTag->stop = true;
					}
					if (pTag->stop)
					{
						if (!working_on_game) 
						{
							UnloadSoundList();
						}
						engine_instance->Stop(pTag->playHandle);

						for (size_t x = 0; x < pTag->info->kvars.size(); x++) 
						{
							pTag->info->kvars[x]->Reset();
						}

						KrilloudDebug::DebugWrite({ "Stopped playing tag: " , pTag->tagname , "\n" });

						PlayingTag erasedTag = *(*tags_list)[index];

						(*tags_list)[index] = nullptr;
						tags_list->erase(tags_list->begin() + index);
						
						if (KrilloudRT::IsServerActive()) 
						{
							KrilloudRT::NotifyPlayingTagRemove(erasedTag);
						}

						continue;
					}
					else
					{
						if (!engine_instance->IsPaused(pTag->playHandle))
						{
							// Update 3D position
							if (pTag->is3D && pTag->parent_object != nullptr)
							{
								//soloud_instance->set3dSourcePosition(pTag->playHandle, pTag->parent_object->position_x, pTag->parent_object->position_y, pTag->parent_object->position_z);
								engine_instance->Set3dSourceParameters(pTag->playHandle
									, pTag->parent_object->position_x, pTag->parent_object->position_y, pTag->parent_object->position_z
									, pTag->parent_object->aVelX, pTag->parent_object->aVelY, pTag->parent_object->aVelZ);
							}

							TagInfo* tag = pTag->info;

							// Update audio volume
							/*for (size_t x = 0; x < tag->mixers.size(); x++)
							{
								tag->mixers[x]->UpdateVolumeValues();
							}
							float volume = tag->source->volume * GetChannelOverallVolume(pTag->channel_id);
							engine_instance->SetVolume(tag->source, volume);*/

							// Update effects
							/*for (size_t x = 0; x < tag->effects.size(); x++)
							{
								tag->effects[x]->UpdatePlayingValues(soloud_instance, pTag->playHandle);
							}*/
							tag->source->UpdateFilters(engine_instance);
							/*for (size_t x = 0; x < tag->source->filters.size(); x++)
							{
								tag->source->filters[x]->UpdatePlayingValues(engine_instance, pTag->playHandle);
							}*/

							if (engine_instance->IsFinished(tag->source) /*soloud_instance->countAudioSource(*tag->wave) == 0*/)
							{
								pTag->stop = true;
							}
						}
					}
				}
				
				index++;
			}

			//After all changes are done, update Soloud 3D audio data
			engine_instance->Update3dAudio();

			//Wait until the next iteration (otherwise the system crashes)
			std::this_thread::sleep_for(0.01s);
		}
	}
	   
	// --- Engine functions -----------------------------------------------------------------------------------------------------------------------------

	// Krilloud_create function
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	bool Krilloud_create(bool from_game, std::string contract_path, std::string app_guid)
	{
		if (engine_instance == nullptr)
		{
			working_on_game = from_game;
			contract_full_path = contract_path;

			// Debug file name
			std::stringstream ss;
			ss << contract_full_path << "\\KLDebug.txt";
			KrilloudDebug::SetDebugFilename(ss.str());
			
#ifdef _DEBUG
			KrilloudDebug::ActivateDebug(true);
#endif
			KrilloudDebug::DebugWrite(" --- Krilloud Creation Start ---");
			
			if (from_game)
			{
				// Get data from the contract
				if (!LoadContract(contract_full_path.c_str()))
					return false;

				if (original_contract.channels.size() <= 0)
					return false;

				for (size_t x = 0; x < contract.channels.size(); x++)
				{
					CreateNewChannel(contract.channels[x].channel_name, contract.channels[x].channel_id, contract.channels[x].parent_id);
					ChangeChannelVolume(x, contract.channels[x].volume);
				}
			}

			// Instance initialization
			//soloud_instance = new Soloud();
			engine_instance = new SoloudInstance();

			engine_instance->SetMaxActiveVoiceCount(128);

			closing_instance_mode = false;

			playing_tags_checker = new std::thread(Thread_PlayUpdater, &playing_tags);
			volumes_checker = new std::thread(Thread_VolumeUpdater, &playing_tags);
			
			if (from_game && KrilloudRT::IsServerSetOnPlay())
			{
				KrilloudRT::StartServer();
			}

			std::stringstream creation_ss;
			creation_ss << "Krilloud creation: " << (engine_instance != nullptr ? "Success." : "Failed.");
			KrilloudDebug::DebugWrite(creation_ss.str());
			return true;
		}
		return false;
	}
#elif __ANDROID__
	int Krilloud_create(bool from_game, std::string app_guid, jobject asset_manager)
	{
		if (engine_instance == nullptr)
		{
			working_on_game = from_game;
			contract_full_path = "KrilloudData";
			
			my_asset_manager = asset_manager;
			mgr = AAssetManager_fromJava(jni_env, my_asset_manager);

			// Debug file name
			std::stringstream ss;
			ss << contract_full_path << "\\KLDebug.txt";
			KrilloudDebug::SetDebugFilename(ss.str());

#ifdef _DEBUG
			KrilloudDebug::ActivateDebug(true);
#endif

			if (from_game)
			{
				// Get data from the contract
				if (!LoadContract(contract_full_path.c_str()))
					return -2;

				////Verify serial number
				if (!KrilloudSerial::ReadKrilloudValidation(contract_full_path.c_str(), app_guid.c_str(), mgr))
					return -4;

				if (original_contract.channels.size() <= 0)
					return -3;

				for (size_t x = 0; x < contract.channels.size(); x++)
				{
					CreateNewChannel(contract.channels[x].channel_name, contract.channels[x].channel_id, contract.channels[x].parent_id);
					ChangeChannelVolume(x, contract.channels[x].volume);
				}
			}

			// Instance initialization
			//soloud_instance = new Soloud();
			engine_instance = new SoloudInstance();

			engine_instance->SetMaxActiveVoiceCount(128);

			closing_instance_mode = false;

			playing_tags_checker = new std::thread(Thread_PlayUpdater, &playing_tags);
/*
			if (from_game)
			{
				bool server_set_on_play = KrilloudRT::IsServerSetOnPlay();

				if (server_set_on_play)
				{
					KrilloudDebug::DebugWrite("Asked if server is set to be automatically activated. Answer is TRUE.");
					KrilloudRT::StartServer();
				}
				else
				{
					KrilloudDebug::DebugWrite("Asked if server is set to be automatically activated. Answer is FALSE.");
				}
			}
*/
			return 0;
		}
		return -1;
	}
#endif	
	
	int Krilloud_destroy() 
	{
#ifndef __ANDROID__
		if (working_on_game && KrilloudRT::IsServerSetOnPlay())
		{
			KrilloudDebug::DebugWrite("Server Stop.");
			KrilloudRT::StopServer();
		}
#endif
		// Set Krilloud as uninitialized
		is_Krilloud_ready = false;
		engine_paused = false;

		// Start closing mode
		closing_instance_mode = true;

		// Wait until updater is finished
		if (playing_tags_checker != nullptr) 
		{
			playing_tags_checker->join();
		}

		if (playing_tags.size() > 0)
		{
			playing_tags.clear();
		}
		
		if (engine_instance != nullptr) 
		{
			engine_instance->StopAll();
		}

		// Destroy listener object
		listener_object = nullptr;

		// Unload objects
		for (int x = (session_loaded_objects.size() - 1); x >= 0; x--)
		{
			if (session_loaded_objects[x] != nullptr) 
			{
				UnloadObject(session_loaded_objects[x]->object_id);
			}
		}

		// Destroy audios
		if (session_loaded_audios.size() > 0) 
		{
			for (size_t x = 0; x < session_loaded_audios.size(); x++) 
			{
				delete session_loaded_audios[x];
			}
			session_loaded_audios.clear();
		}

		// Destroy objects
		if (session_loaded_objects.size() > 0)
		{
			for (size_t x = 0; x < session_loaded_objects.size(); x++)
			{
				delete session_loaded_objects[x];
			}
			session_loaded_objects.clear();
		}

		// Destroy variables
		if (session_variables.size() > 0)
		{
			for (size_t x = 0; x < session_variables.size(); x++)
			{
				delete session_variables[x];
			}
			session_variables.clear();
		}

		// Destroy tags
		if (my_tags.size() > 0) 
		{
			/*for (size_t x = 0; x < my_tags.size(); x++) 
			{
				delete my_tags[x].tag_content;
			}*/
			my_tags.clear();
		}
		
		// Clean channels
		channels.clear();

		// Clean server structures
		server_listener_object = nullptr;
		server_session_loaded_objects.clear();
		server_playing_tags.clear();

		// Destroy Soloud instance
		if (engine_instance != nullptr)
		{
			delete engine_instance;
			engine_instance = nullptr;
		}

		// End updater thread
		playing_tags_checker = nullptr;

		// Write remaining debug lines before closing
		KrilloudDebug::DumpDebug();

		return KrilloudIsClean();
	}

	int Krilloud_init(unsigned int flags, unsigned int backend, unsigned int sample_rate, unsigned int buffer_size, unsigned int channels)
	{
		if (engine_instance != nullptr)
		{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
			if (working_on_game)
			{
				backend = contract.engine_configurations[0].backend;
				sample_rate = contract.engine_configurations[0].sample_rate;
				buffer_size = contract.engine_configurations[0].buffer_size;
			}
			//Soloud20200207 WINMM = 4
			//Soloud20200207 XAUDIO2 = 5
			//Soloud20200207 WASAPI = 6
			switch (backend) 
			{
			case AudioEngineBackend::DefaultBackend: backend = 6; break;
			case AudioEngineBackend::WinMM: backend = 4; break;
			case AudioEngineBackend::XAudio2: backend = 5; break;
			case AudioEngineBackend::Wasapi: backend = 6; break;
			default: backend = 6; break;
			}
#elif __ANDROID__
			if (working_on_game)
			{
				backend = contract.engine_configurations[1].backend;
				sample_rate = contract.engine_configurations[1].sample_rate;
				buffer_size = contract.engine_configurations[1].buffer_size;
			}
			//Soloud20200207 OPENSLES = 12
			switch (backend)
			{
			case AudioEngineBackend::DefaultBackend: backend = 12; break;
			case AudioEngineBackend::OpenSLES: backend = 12; break;
			default: backend = 12; break;
			}
#elif __APPLE__
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
			return -3;
#endif
			switch (sample_rate)
			{
			case AudioEngineSampleRate::DefaultSampleRate: sample_rate = 44100; break;
			case AudioEngineSampleRate::s44100: sample_rate = 44100; break;
			case AudioEngineSampleRate::s48000: sample_rate = 48000; break;
			case AudioEngineSampleRate::s88200: sample_rate = 88200; break;
			case AudioEngineSampleRate::s96000: sample_rate = 96000; break;
			default: sample_rate = 44100; break;
			}

			switch (buffer_size) 
			{
			case AudioEngineBuffer::DefaultBuffer: buffer_size = 4096; break;
			case AudioEngineBuffer::b32: buffer_size = 32; break;
			case AudioEngineBuffer::b64: buffer_size = 64; break;
			case AudioEngineBuffer::b128: buffer_size = 128; break;
			case AudioEngineBuffer::b256: buffer_size = 256; break;
			case AudioEngineBuffer::b512: buffer_size = 512; break;
			case AudioEngineBuffer::b1024: buffer_size = 1024; break;
			case AudioEngineBuffer::b2048: buffer_size = 2048; break;
			case AudioEngineBuffer::b4096: buffer_size = 4096; break;
			default: buffer_size = 4096; break;
			}

			switch (channels) 
			{
			case AudioEngineChannel::DefaultChannels: channels = 2; break;
			case AudioEngineChannel::Mono: channels = 1; break;
			case AudioEngineChannel::Stereo: channels = 2; break;
			case AudioEngineChannel::Sorround4: channels = 4; break;
			case AudioEngineChannel::Sorround5: channels = 6; break;
			case AudioEngineChannel::Sorround7: channels = 8; break;
			default: channels = 2; break;
			}

			int left_handed_3D = 4;
			int result = engine_instance->Init(flags | left_handed_3D, backend, sample_rate, buffer_size, channels);

			KrilloudDebug::DebugWrite({"Krilloud init = "
				, std::to_string(result), ", ", std::to_string(backend), ", ", std::to_string(sample_rate)
				, ", ", std::to_string(buffer_size), ", ", std::to_string(channels) });

			if (result == 0) 
			{
				is_Krilloud_ready = true;
				return result;
			}
			return -2;
		}
		return -1;
	}

	void Krilloud_deinit()
	{
		//Moved to Krilloud_destroy

		//// Start closing mode
		//closing_instance_mode = true;

		//// Wait until updater is finished
		//playing_tags_checker->join();

		//if (playing_tags.size() > 0)
		//{
		//	playing_tags.clear();
		//}

		//if (soloud_instance != nullptr)
		//{
		//	soloud_instance->stopAll();
		//	soloud_instance->deinit();
		//}
	}

	// --- Contract functions ------------------------------------------------------------------------------------------------------------------------ 

	void SetContractData(KrilloudProjectInfo data)
	{
		contract = data;
	}

	// --- Tag functions --------------------------------------------------------------------------------------------------------------------------------

	int LoadTag(const char * tag_name, int object_id)
	{
		CreateIngameObject(object_id);

		if (working_on_game)
		{
			std::stringstream ss;
			ss << tag_name << object_id;

			KrilloudDebug::DebugWrite({ "Loading Tag ", ss.str(), " for object ", std::to_string(object_id) });

			//Instantiate tag nodes 
			int contract_tag_index = -1;

			for (size_t x = 0; x < contract.tags.size(); x++)
			{
				if (contract.tags[x].tagname == tag_name)
				{
					contract_tag_index = x;
				}
			}

			//Load audios
			if (contract_tag_index > -1)
			{
				InstantiateTag(&contract.tags[contract_tag_index], object_id);

				for (size_t x = 0; x < my_tags.size(); x++)
				{
					if (my_tags[x].tag.tag_name == ss.str())
					{
						int result = LoadLevelAudioSources(my_tags[x].tag_content, my_tags[x].tag.tag_name);
						KrilloudDebug::DebugWrite({"Load Tag ", ss.str(), " for object ", std::to_string(object_id) , " complete. Result = ", std::to_string(result)});
						return result;
					}
				}
			}

			KrilloudDebug::DebugWrite({ "Load Tag ", ss.str(), " for object ", std::to_string(object_id) , " failed. Result = -1" });
			return -1;
		}
		else
		{
			//Load audios
			for (size_t x = 0; x < my_tags.size(); x++)
			{
				if (my_tags[x].tag.tag_name == tag_name)
				{
					int result = LoadLevelAudioSources(my_tags[x].tag_content, my_tags[x].tag.tag_name);
					return result;
				}
			}
			return -2;
		}
	}

	//INTERNAL
	int UnloadTag(const char * tagname)
	{
		std::stringstream debug_ss;
		debug_ss << "Unload Tag: " << tagname << ". ";

		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tagname)
			{
				StopInstancedTag(tagname);

				//Unload audios
				int audio_index = 0;
				while (audio_index < session_loaded_audios.size()) 
				{
					//Find and erase owner of the sound
					RemoveLoadedAudioOwner(session_loaded_audios[audio_index]->id, my_tags[x].tag.tag_name);
					
					//Find and erase the audiosource IF it has 0 owners
					if (session_loaded_audios[audio_index]->owners.size() <= 0)
					{
						if (engine_instance->IsFinished(session_loaded_audios[audio_index]->source) /*soloud_instance->countAudioSource(session_loaded_audios[audio_index]->sound) > 0*/)
						{
							engine_instance->StopAudioSource(session_loaded_audios[audio_index]->source);
						}

						bool unloaded_audio = engine_instance->UnloadAudio(session_loaded_audios[audio_index]->id);
						if (!unloaded_audio) 
						{
							KrilloudDebug::DebugWrite({ "Unload audio ", std::to_string(session_loaded_audios[audio_index] ->id), " for tag: ", tagname, " failed."});
						}

						session_loaded_audios.erase(session_loaded_audios.begin() + audio_index);
					}
					else 
					{
						audio_index++;
					}
				}

				//Unload tag info
				my_tags.erase(my_tags.begin() + x);

				debug_ss << "Success.";
				KrilloudDebug::DebugWrite(debug_ss.str());

				return 1;
			}
		}

		debug_ss << "Failed.";
		KrilloudDebug::DebugWrite(debug_ss.str());
		return 0;
	}

	int PlayTag(const char * tag_name, int object_id)
	{
		std::stringstream playtag_ss;
		playtag_ss << "Play tag " << tag_name;
		KrilloudDebug::DebugWrite(playtag_ss.str());
		if (!is_Krilloud_ready)
			return -7;
		KrilloudDebug::DebugWrite("Krilloud ready");
		IngameObject* playingObject = FindIngameObject(object_id);
		
		try
		{
			if (playingObject != nullptr || (!working_on_game))
			{
				std::stringstream ss;
				if (working_on_game)
				{
					ss << tag_name << object_id;
				}
				else
				{
					ss << tag_name;
				}

				for (size_t x = 0; x < my_tags.size(); x++)
				{
					if (my_tags[x].tag.tag_name == ss.str())
					{
						if (my_tags[x].tag_content != nullptr)
						{
							//Get the info package
							TagInfo* tInfo = GetInfoFromKLevel(my_tags[x].tag_content);

							if (tInfo != nullptr)
							{
								//Create a new tag to be played with the retrieved info
								PlayingTag* new_playingTag = new PlayingTag();
								new_playingTag->id = ++last_given_id;
								
								new_playingTag->parent_object = playingObject;
								
								new_playingTag->is3D = my_tags[x].tag.is3D;
								new_playingTag->tagname = ss.str();
								new_playingTag->stop = false;
								
								//Playing info package
								new_playingTag->info = tInfo;
								
								//Set volume depending on channel
								new_playingTag->channel_id = my_tags[x].tag.channel_id;
								float volume = tInfo->source->volume * GetChannelOverallVolume(new_playingTag->channel_id);
								
								if (tInfo->source != nullptr) 
								{
									//Set loop
									if (my_tags[x].tag.isLoopable/* && working_on_game*/)
									{
										tInfo->source->SetLooping(true);
									}

									if (new_playingTag->is3D)
									{
										std::stringstream debug_ss;
										debug_ss << "--- PLAY 3D TAG--- TAG: " << tag_name << " - with object (ID " << object_id << ")";
										debug_ss << "Min_distance: " << my_tags[x].tag.min_3D_distance << ", Roll off: " << my_tags[x].tag.roll_off_factor << ", Doppler(factor):" << my_tags[x].tag.doppler_active << "(" << my_tags[x].tag.doppler_factor << "), Distance delay: " << my_tags[x].tag.distance_delay;
										KrilloudDebug::DebugWrite(debug_ss.str());

										//Min/Max distance the sound can be heard
										tInfo->source->Set3dMinMaxDistance(my_tags[x].tag.min_3D_distance, my_tags[x].tag.max_3D_distance);

										//Attenuation
										/*SoLoud::AudioSource::ATTENUATION_MODELS attenuation = SoLoud::AudioSource::ATTENUATION_MODELS::NO_ATTENUATION;

										if (my_tags[x].attenuation >= 0 && my_tags[x].attenuation <= 3)
										{
											attenuation = (SoLoud::AudioSource::ATTENUATION_MODELS)my_tags[x].attenuation;
										}*/

										unsigned int attenuation = (my_tags[x].tag.attenuation >= 0 && my_tags[x].tag.attenuation <= 3) ? my_tags[x].tag.attenuation : 0;
										tInfo->source->Set3dAttenuation(attenuation, my_tags[x].tag.roll_off_factor);

										//Doppler effect
										if (my_tags[x].tag.doppler_active)
										{
											tInfo->source->Set3dDopplerFactor(my_tags[x].tag.doppler_factor);
										}
										
										//Distance delay
										tInfo->source->Set3dDistanceDelay(my_tags[x].tag.distance_delay);

										//Play
										new_playingTag->playHandle = engine_instance->Play3d(tInfo->source, new_playingTag->parent_object->position_x, new_playingTag->parent_object->position_y, new_playingTag->parent_object->position_z, volume);
										
									}
									else
									{
										std::stringstream debug_ss;
										debug_ss << "--- PLAY TAG: " << tag_name << " - with object (ID " << object_id << ")";
										debug_ss << "     WAVE ID:" << tInfo->source->id;
										KrilloudDebug::DebugWrite(debug_ss.str());

										//Play
										new_playingTag->playHandle = engine_instance->Play(tInfo->source, volume);
									}

									//Update mixer values for the very first second of play
									for (size_t x = 0; x < new_playingTag->info->mixers.size(); x++)
									{
										new_playingTag->info->mixers[x]->UpdateVolumeValues();
									}

									//Update filter values for the very first second of play
									for (size_t x = 0; x < new_playingTag->info->source->filters.size(); x++)
									{
										new_playingTag->info->source->filters[x]->UpdatePlayingValues(engine_instance, new_playingTag->playHandle);
									}

									engine_instance->Update3dAudio();

									//Add the new playing tag to the list
									playing_tags.push_back(new_playingTag);

									if (KrilloudRT::IsServerActive()) 
									{
										KrilloudRT::NotifyNewPlayingTag(*new_playingTag);
									}

									return new_playingTag->id;
								}
								else
								{
									std::stringstream debug_ss;
									debug_ss << "ERROR PLAYING TAG. TAG: " << tag_name << " - with object (ID " << object_id << "). WAVE is null.";
									KrilloudDebug::DebugWrite(debug_ss.str());

									return -1;
								}
							}
							else
							{
								std::stringstream debug_ss;
								debug_ss << "ERROR PLAYING TAG. TAG: " << tag_name << " - with object (ID " << object_id << "). Tinfo is null.";
								KrilloudDebug::DebugWrite(debug_ss.str());
								return -2;
							}
						}
						else
						{
							std::stringstream debug_ss;
							debug_ss << "ERROR PLAYING TAG. TAG: " << tag_name << " - with object (ID " << object_id << "). Tag content is null.";
							KrilloudDebug::DebugWrite(debug_ss.str());
							return -3;
						}
					}
				}
				std::stringstream debug_ss;
				debug_ss << "ERROR PLAYING TAG. TAG: " << tag_name << " - with object (ID " << object_id << "). Tag not found.";
				KrilloudDebug::DebugWrite(debug_ss.str());
				return -4;
			}
		}
		catch (const std::exception &e)
		{
			std::stringstream debug_ss;
			debug_ss << "Exception trying to play object (ID " << object_id << "): " << e.what();
			KrilloudDebug::DebugWrite(debug_ss.str());

			return -5;
		}
		catch (...) 
		{
			std::stringstream debug_ss;
			debug_ss << "Error trying to play object (ID " << object_id << ").";
			KrilloudDebug::DebugWrite(debug_ss.str());

			return -6;
		}

		return -8;
	}

	int StopInstancedTag(int tag_id)
	{
		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x] != nullptr && playing_tags[x]->id == tag_id)
			{
				playing_tags[x]->stop = true;
				return 0;
			}
		}
		return -1;
	}

	//INTERNAL
	int StopInstancedTag(std::string tagname)
	{
		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x] != nullptr && playing_tags[x]->tagname == tagname)
			{
				std::stringstream debug_ss;
				debug_ss << "set tag: " << playing_tags[x]->tagname << " to stop.\n";
				KrilloudDebug::DebugWrite(debug_ss.str());
				playing_tags[x]->stop = true;
				return 0;
			}
		}
		return -1;
	}

	int StopInstancedTag(std::string tag_name, int object_id)
	{
		std::stringstream ss;
		if (working_on_game)
		{
			ss << tag_name << object_id;
		}
		else
		{
			ss << tag_name;
		}

		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x] != nullptr && playing_tags[x]->tagname == ss.str())
			{
				playing_tags[x]->stop = true;
				return 0;
			}
		}
		return -1;
	}

	int StopAllInstancesOfTag(std::string tag_name, int object_id)
	{
		std::stringstream ss;
		if (working_on_game)
		{
			ss << tag_name << object_id;
		}
		else
		{
			ss << tag_name;
		}
		int count = 0;

		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x] != nullptr && playing_tags[x]->tagname == ss.str() && !playing_tags[x]->stop)
			{
				playing_tags[x]->stop = true;
				count++;
			}
		}

		return count;
	}

	int StopAllInstancedTags()
	{
		int count = 0;

		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x] != nullptr && !playing_tags[x]->stop)
			{
				playing_tags[x]->stop = true;
				count++;
			}
		}

		return count;
	}

	bool isTagPlaying(std::string tag_name, int object_id)
	{
		std::stringstream ss;
		if (working_on_game)
		{
			ss << tag_name << object_id;
		}
		else
		{
			ss << tag_name;
		}

		for (size_t x = 0; x < playing_tags.size(); x++)
		{
			if (playing_tags[x]->tagname == ss.str() && !playing_tags[x]->stop)
			{
				return true;
			}
		}

		return false;
	}

	//INTERNAL
	CompleteKrillTag* GetTag(std::string tagname)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tagname)
			{
				return &my_tags[x];
			}
		}
		return nullptr;
	}

	int IsTagLoaded(std::string tag_name)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				return (int)x;
			}
		}

		return -1;
	}

	bool RenameTag(std::string old_name, std::string new_name)
	{
		if (IsTagLoaded(new_name) == -1)
		{
			for (size_t x = 0; x < my_tags.size(); x++)
			{
				if (my_tags[x].tag.tag_name == old_name)
				{
					my_tags[x].tag.tag_name = new_name;
					return true;
				}
			}
		}		
		return false;
	}

	bool CreateNewTag(std::string tag_name, KLevel* level_ptr)
	{
		if (IsTagLoaded(tag_name) == -1)
		{
			CompleteKrillTag new_tag;
			new_tag.tag.tag_name = tag_name;

			if (level_ptr == nullptr)
			{
				new_tag.tag_content = CreateKLevel();
			}
			else
			{
				new_tag.tag_content = level_ptr;
			}


			new_tag.tag.is3D = false;
			new_tag.tag.owner_id = 0;

			new_tag.tag.isLoopable = false;

			new_tag.tag.channel_id = 0;

			my_tags.push_back(new_tag);

			return true;
		}
		return false;
	}

	bool CreateNewTag(CompleteKrillTag tag_info)
	{
		if (IsTagLoaded(tag_info.tag.tag_name) == -1)
		{
			CompleteKrillTag new_tag(tag_info);
			my_tags.push_back(new_tag);

			return true;
		}
		return false;
	}

	int RemoveTag(std::string tag_name)
	{
		return UnloadTag(tag_name.c_str());
	}

	bool SetTagLoopable(std::string tag_name, bool new_value)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.isLoopable = new_value;
				return true;
			}
		}
		return false;
	}

	bool SetTagChannel(std::string tag_name, int channel_id)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.channel_id = channel_id;
				return true;
			}
		}
		return false;
	}
	
	bool SetTagAttenuation(std::string tag_name, int attenuation_type) 
	{
		if (attenuation_type < 0 && attenuation_type > 3)
			return false;

		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.attenuation = attenuation_type;

				switch (attenuation_type) 
				{
				case 1:
					if (my_tags[x].tag.min_3D_distance <= 0)
						my_tags[x].tag.min_3D_distance = 0.1f;
					if (my_tags[x].tag.max_3D_distance < my_tags[x].tag.min_3D_distance)
						my_tags[x].tag.max_3D_distance = my_tags[x].tag.min_3D_distance;
					if (my_tags[x].tag.roll_off_factor <= 0)
						my_tags[x].tag.roll_off_factor = 0.1f;
					break;
				case 2:
					if (my_tags[x].tag.min_3D_distance < 0)
						my_tags[x].tag.min_3D_distance = 0;
					if (my_tags[x].tag.max_3D_distance <= my_tags[x].tag.min_3D_distance)
						my_tags[x].tag.max_3D_distance = my_tags[x].tag.min_3D_distance + 1;
					if (my_tags[x].tag.roll_off_factor < 0)
						my_tags[x].tag.roll_off_factor = 0;
					if (my_tags[x].tag.roll_off_factor > 1)
						my_tags[x].tag.roll_off_factor = 1;
					break;
				case 3:
					if (my_tags[x].tag.min_3D_distance <= 0)
						my_tags[x].tag.min_3D_distance = 0.1f;
					if (my_tags[x].tag.max_3D_distance < my_tags[x].tag.min_3D_distance)
						my_tags[x].tag.max_3D_distance = my_tags[x].tag.min_3D_distance;
					if (my_tags[x].tag.roll_off_factor <= 0)
						my_tags[x].tag.roll_off_factor = 0.1f;
					if (my_tags[x].tag.roll_off_factor >= 100)
						my_tags[x].tag.roll_off_factor = 99.9f;
					break;
				}

				return true;
			}
		}
		return false;
	}

	bool SetTag3D(std::string tag_name, bool is_tag_3d)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.is3D = is_tag_3d;
				return true;
			}
		}
		return false;
	}

	bool SetTagDistanceDelay(std::string tag_name, bool has_distance_delay)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.distance_delay = has_distance_delay;
				return true;
			}
		}
		return false;
	}

	bool SetTagDopplerEffect(std::string tag_name, bool is_doppler_active)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.doppler_active = is_doppler_active;
				return true;
			}
		}
		return false;
	}

	bool SetTagDopplerFactor(std::string tag_name, float doppler_factor)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				my_tags[x].tag.doppler_factor = doppler_factor;
				return true;
			}
		}
		return false;
	}

	bool SetTagMinMaxDistance(std::string tag_name, float min_distance, float max_distance)
	{
		if (min_distance > max_distance)
			return false;

		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				switch (my_tags[x].tag.attenuation)
				{
				case 1:
					if (min_distance <= 0)
						return false;
					break;
				case 2:
					if (min_distance < 0 || min_distance == max_distance)
						return false;
					break;
				case 3:
					if (min_distance <= 0)
						return false;
					break;
				default:
					return false;
				}

				my_tags[x].tag.min_3D_distance = min_distance;
				my_tags[x].tag.max_3D_distance = max_distance;
				return true;
			}
		}
		return false;
	}

	bool SetTagRollOffFactor(std::string tag_name, float roll_off_factor)
	{
		for (size_t x = 0; x < my_tags.size(); x++)
		{
			if (my_tags[x].tag.tag_name == tag_name)
			{
				switch (my_tags[x].tag.attenuation)
				{
				case 1:
					if (roll_off_factor <= 0)
						return false;
					break;
				case 2:
					if (roll_off_factor < 0 || roll_off_factor > 1)
						return false;
					break;
				case 3:
					if (roll_off_factor <= 0 || roll_off_factor >= 100)
						return false;
					break;
				default:
					return false;
				}

				my_tags[x].tag.roll_off_factor = roll_off_factor;
				return true;
			}
		}
		return false;
	}

	std::vector<PlayingTag> GetPlayingTagsList()
	{
		if (KrilloudRT::IsConectionActive())
		{
			return server_playing_tags;
		}
		else
		{
			std::vector<PlayingTag> result;

			for (size_t x = 0; x < playing_tags.size(); x++)
			{
				if (playing_tags[x] != nullptr && !playing_tags[x]->stop)
				{
					result.push_back(*playing_tags[x]);
				}
			}

			return result;
		}
	}

	int GetLoadedTagsCount()
	{
		return (int)my_tags.size();
	}

	int GetPlayingTagsCount()
	{
		int result = 0;
		for (size_t x = 0; x < playing_tags.size(); x++) 
		{
			if (!playing_tags[x]->stop) 
			{
				result++;
			}
		}
		return result;
	}

	// --- KLevels and Nodes functions ------------------------------------------------------------------------------------------------------------------

	KLevel* CreateKLevel()
	{
		return new KLevel();
	}

	//INTERNAL
	TagInfo * GetInfoFromKLevel(KLevel* level)
	{
		for (size_t x = 0; x < level->nodes.size(); x++)
		{
			KrillOutput* output_node = dynamic_cast<KrillOutput*>(level->nodes[x]);

			if (output_node != nullptr)
			{
				return output_node->GetInfo();
			}
		}

		return nullptr;
	}

	bool AddNode(KrilloudClass* class_to_add, KLevel* target_level)
	{
		if (class_to_add != nullptr && target_level != nullptr)
		{
			target_level->nodes.push_back(class_to_add);
			return true;
		}
		return false;
	}

	bool RemoveNode(KrilloudClass* class_to_add, KLevel* target_level)
	{
		if (class_to_add != nullptr && target_level != nullptr)
		{
			for (size_t x = 0; x < target_level->nodes.size(); x++)
			{
				if (target_level->nodes[x] == class_to_add)
				{
					target_level->nodes.erase(target_level->nodes.begin() + x);
					return true;
				}
			}
		}

		return false;
	}

	bool AddKLevel(KLevel* new_level, KLevel* target_level)
	{
		if (new_level != nullptr && target_level != nullptr && new_level->parent == nullptr && LevelsNotRelated(new_level, target_level))
		{
			target_level->sublevels.push_back(new_level);
			new_level->parent = target_level;
			return true;
		}
		return false;
	}
	
	//INTERNAL
	bool LevelsNotRelated(KLevel* new_level, KLevel* level) 
	{
		if (new_level != nullptr && level != nullptr)
		{
			if (new_level == level)
				return false;

			if (level->parent != nullptr)
				return LevelsNotRelated(new_level, level->parent);
		}
		return true;
	}

	bool RemoveKLevel(KLevel* level_to_remove, KLevel* target_level)
	{
		if (level_to_remove != nullptr && target_level != nullptr)
		{
			for (size_t x = 0; x < target_level->sublevels.size(); x++)
			{
				if (target_level->sublevels[x] == level_to_remove)
				{
					target_level->sublevels.erase(target_level->sublevels.begin() + x);
					return true;
				}
			}
		}
		return false;
	}

	// -- Krilloud vars functions -----------------------------------------------------------------------------------------------------------------------

	//Variable reading
	float GetKrillVar(const char * variable_name, int owner_object_id)
	{
		std::stringstream ss;
		bool isGlobal = true;

		for (size_t x = 0; x < contract.variables.size(); x++) 
		{
			if (contract.variables[x].variable_name == variable_name) 
			{
				isGlobal = (contract.variables[x].type == Global);
				break;
			}
		}

		if (isGlobal) 
		{
			ss << variable_name;
		}
		else
		{
			ss << variable_name << local_variable_separator << owner_object_id;
		}

		for (size_t x = 0; x < session_variables.size(); x++)
		{
			if (session_variables[x] != nullptr && session_variables[x]->variable_name == ss.str())
			{
				return session_variables[x]->current_value;
			}
		}
		return 0;
	}

	//Variable updating
	bool SetKrillVar(const char * variable_name, int owner_object_id, float value)
	{
		std::stringstream debug_ss;
		debug_ss << "Set KrillOud variable:\n";
		

		std::stringstream ss;

		bool isGlobal = true;

		for (size_t x = 0; x < contract.variables.size(); x++)
		{
			if (contract.variables[x].variable_name == variable_name)
			{
				isGlobal = (contract.variables[x].type == Global);
				break;
			}
		}

		if (isGlobal)
		{
			ss << variable_name;
		}
		else
		{
			ss << variable_name << local_variable_separator << owner_object_id;
		}

		debug_ss << "		Name:" << ss.str() << "\n";
		
		bool variable_changed = false;
		
		for (size_t x = 0; x < session_variables.size(); x++) 
		{
			if (session_variables[x] != nullptr && session_variables[x]->variable_name == ss.str()) 
			{
				session_variables[x]->current_value = value;

				if (KrilloudRT::IsRealTimeOn())
				{
					KrilloudRT::NotifyVariableCurrentValue(x, value);
				}

				variable_changed = true;
				debug_ss << "		Variable changed to " << value;
				break;
			}
		}

		if (!variable_changed) 
		{
			debug_ss << "		Variable not found.";
		}

		KrilloudDebug::DebugWrite(debug_ss.str());

		return variable_changed;
	}

	VariableData* CreateNewKVariable(std::string var_name)
	{
		if (GetKVariableData(var_name) == nullptr) 
		{
			VariableData* var_data = new VariableData();
			var_data->variable_name = var_name;
			var_data->content = Raw;
			var_data->current_value = 0;
			var_data->range_max = 1;
			var_data->range_min = 0;
			var_data->source = InGame;
			var_data->type = Global;
			var_data->set_only_on_play = false;

			session_variables.push_back(var_data);

			return var_data;
		}
		return nullptr;
	}

	VariableData* CreateNewKVariable(VariableData var_data)
	{
		if (GetKVariableData(var_data.variable_name) == nullptr)
		{
			VariableData* new_var_data = new VariableData(var_data);

			/*VariableData* var_data = new VariableData();
			var_data->variable_name = data.variable_name;
			var_data->content = data.content;
			var_data->current_value = data.current_value;
			var_data->range_max = data.range_max;
			var_data->range_min = data.range_min;
			var_data->source = data.source;
			var_data->type = data.type;
			var_data->set_only_on_play = data.set_only_on_play;*/

			session_variables.push_back(new_var_data);

			return new_var_data;
		}
		return nullptr;
	}

	bool RemoveKVariable(std::string var_name) 
	{
		for (size_t x = 0; x < session_variables.size(); x++) 
		{
			if (session_variables[x]->variable_name == var_name)
			{
				session_variables.erase(session_variables.begin() + x);
				return true;
			}
		}
		return false;
	}

	bool RemoveKVariable(int var_index)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables.erase(session_variables.begin() + var_index);
			return true;
		}
		return false;
	}

	//INTERNAL
	VariableData* GetKVariableData(int position) 
	{
		if (position > 0 && position < session_variables.size()) 
		{
			return session_variables[position];
		}
		return nullptr;
	}

	//INTERNAL
	VariableData* GetKVariableData(std::string var_name)
	{
		for (size_t x = 0; x < session_variables.size(); x++) 
		{
			if (session_variables[x]->variable_name == var_name) 
			{
				return session_variables[x];
			}
		}
		return nullptr;
	}

	bool SetVariableName(int var_index, std::string new_name)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			for (size_t x = 0; x < session_variables.size(); x++) 
			{
				if (session_variables[x]->variable_name == new_name)
					return false;
			}

			session_variables[var_index]->variable_name = new_name;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableName(var_index, new_name);
			}
			return true;
		}
		return false;
	}

	bool SetVariableCurrentValue(int var_index, float new_value)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables[var_index]->current_value = new_value;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableCurrentValue(var_index, new_value);
			}
			return true;
		}
		return false;
	}

	bool SetVariableType(int var_index, VarType new_type)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables[var_index]->type = new_type;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableType(var_index, new_type);
			}
			return true;
		}
		return false;
	}

	bool SetVariableSource(int var_index, VarSourceType new_source_type)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables[var_index]->source = new_source_type;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableSource(var_index, new_source_type);
			}
			return true;
		}
		return false;
	}

	bool SetVariableContent(int var_index, VarContentType new_content_type)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables[var_index]->content = new_content_type;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableContent(var_index, new_content_type);
			}
			return true;
		}
		return false;
	}

	bool SetVariableRangeMin(int var_index, float new_range_min)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			if (new_range_min > session_variables[var_index]->range_max)
				return false;

			session_variables[var_index]->range_min = new_range_min;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableRangeMin(var_index, new_range_min);
			}
			return true;
		}
		return false;
	}

	bool SetVariableRangeMax(int var_index, float new_range_max)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			if (new_range_max < session_variables[var_index]->range_min)
				return false;

			session_variables[var_index]->range_max = new_range_max;
			if (KrilloudRT::IsRealTimeOn())
			{
				KrilloudRT::NotifyVariableRangeMax(var_index, new_range_max);
			}
			return true;
		}
		return false;
	}

	bool SetVariableOnPlay(int var_index, bool new_set_on_play)
	{
		if (var_index >= 0 && var_index < session_variables.size())
		{
			session_variables[var_index]->set_only_on_play = new_set_on_play;
			if (KrilloudRT::IsRealTimeOn()) 
			{
				KrilloudRT::NotifyVariableOnlyOnPlay(var_index, new_set_on_play);
			}
			return true;
		}
		return false;
	}

	// --- GameObjects functions ------------------------------------------------------------------------------------------------------------------------

	void SetListenerPosition(float position_x, float position_y, float position_z,
							 float at_x, float at_y, float at_z,
							 float up_x, float up_y, float up_z,
							 float velocity_x, float velocity_y, float velocity_z)
	{
		if (!is_Krilloud_ready)
			return;

		if (listener_object == nullptr) 
		{
			listener_object = new IngameObject();
			listener_object->object_id = 0;
		}

		listener_object->position_x = position_x;
		listener_object->position_y = position_y;
		listener_object->position_z = position_z;

		listener_object->aVelX = velocity_x;
		listener_object->aVelY = velocity_y;
		listener_object->aVelZ = velocity_z;

		engine_instance->Set3dListenerParameters(position_x, position_y, position_z, at_x, at_y, at_z, up_x, up_y, up_z, velocity_x, velocity_y, velocity_z);

		if (KrilloudRT::IsServerActive()) 
		{
			KrilloudRT::NotifyListener(*listener_object);
		}
	}

	void SetObjectPosition(int object_id, float position_x, float position_y, float position_z, float velocity_x, float velocity_y, float velocity_z)
	{
		IngameObject* gameObject = FindIngameObject(object_id);
		if (gameObject == nullptr)
		{
			CreateIngameObject(object_id, position_x, position_y, position_z, velocity_x, velocity_y, velocity_z);
		}
		else
		{
			gameObject->position_x = position_x;
			gameObject->position_y = position_y;
			gameObject->position_z = position_z;

			gameObject->aVelX = velocity_x;
			gameObject->aVelY = velocity_y;
			gameObject->aVelZ = velocity_z;

			if (KrilloudRT::IsServerActive())
			{
				KrilloudRT::NotifyObjectChange(*gameObject);
			}
		}
	}

	//INTERNAL
	IngameObject* FindIngameObject(int object_id)
	{
		for (size_t x = 0; x < session_loaded_objects.size(); x++)
		{
			if (session_loaded_objects[x]->object_id == object_id)
			{
				return session_loaded_objects[x];
			}
		}
		return nullptr;
	}

	void CreateIngameObject(int object_id, float position_x, float position_y, float position_z, float velocity_x, float velocity_y, float velocity_z)
	{
		if (FindIngameObject(object_id) == nullptr)
		{
			IngameObject* new_gameobject = new IngameObject();
			new_gameobject->object_id = object_id;

			new_gameobject->position_x = position_x;
			new_gameobject->position_y = position_y;
			new_gameobject->position_z = position_z;

			new_gameobject->aVelX = velocity_x;
			new_gameobject->aVelY = velocity_y;
			new_gameobject->aVelZ = velocity_z;

			session_loaded_objects.push_back(new_gameobject);

			if (KrilloudRT::IsServerActive()) 
			{
				KrilloudRT::NotifyNewObject(*new_gameobject);
			}
		}
	}

	int UnloadObject(int object_id)
	{
		IngameObject* gameobject = FindIngameObject(object_id);

		if (gameobject != nullptr)
		{
			std::stringstream debug_ss;
			debug_ss << "Unload object ID: " << object_id << "\n";
			KrilloudDebug::DebugWrite(debug_ss.str());

			//Remove object playing tags
			for (size_t x = 0; x < playing_tags.size(); x++)
			{
				if (playing_tags[x]->parent_object->object_id == object_id)
				{
					playing_tags[x]->stop = true;
				}
			}

			//Remove object tags
			int removed_tags = 0;
			std::vector<std::string> tags_to_remove;
			for (size_t x = 0; x < my_tags.size(); x++)
			{
				if (my_tags[x].tag.owner_id == object_id)
				{
					tags_to_remove.push_back(my_tags[x].tag.tag_name);
				}
			}
			for (size_t x = 0; x < tags_to_remove.size(); x++) 
			{
				removed_tags += UnloadTag(tags_to_remove[x].c_str());
			}

			//Remove object variables
			size_t index = 0;
			int destroyed_variables = 0;
			while (index < session_variables.size())
			{
				int found = -1;
				found = session_variables[index]->variable_name.find(local_variable_separator);

				if (found > -1)
				{
					std::string ss = session_variables[index]->variable_name.substr(found + local_variable_separator.size());

					std::string::size_type sz;

					int var_number = std::stoi(ss, &sz);
					if (var_number == object_id)
					{
						session_variables.erase(session_variables.begin() + index);
						destroyed_variables++;
					}
					else
					{
						index++;
					}
				}
				else
				{
					index++;
				}
			}

			//Remove object
			size_t current_objects_count = session_loaded_objects.size();
			for (size_t x = 0; x < session_loaded_objects.size(); x++)
			{
				if (session_loaded_objects[x]->object_id == object_id)
				{
					if (KrilloudRT::IsServerActive()) 
					{
						IngameObject destroyedObject;
						destroyedObject.object_id = session_loaded_objects[x]->object_id;
						destroyedObject.position_x = session_loaded_objects[x]->position_x;
						destroyedObject.position_y = session_loaded_objects[x]->position_y;
						destroyedObject.position_z = session_loaded_objects[x]->position_z;
						destroyedObject.aVelX = session_loaded_objects[x]->aVelX;
						destroyedObject.aVelY = session_loaded_objects[x]->aVelY;
						destroyedObject.aVelZ = session_loaded_objects[x]->aVelZ;

						session_loaded_objects.erase(session_loaded_objects.begin() + x);

						KrilloudRT::NotifyObjectRemove(destroyedObject);
					}
					else
					{
						session_loaded_objects.erase(session_loaded_objects.begin() + x);
					}
					break;
				}
			}

			bool removal_correct = (current_objects_count - session_loaded_objects.size() == 1);
			debug_ss.str("");
			debug_ss << "Unload object ID: " << object_id << " " << (removal_correct ? "Succeeded" : "Failed")
				<< " with " << removed_tags << " tags removed and " << destroyed_variables << " removed variables.";
			KrilloudDebug::DebugWrite(debug_ss.str());
			return removed_tags;
		}

		return -1;
	}
	
	std::vector<IngameObject> GetIngameObjectList() 
	{
		std::vector<IngameObject> result;
		
		IngameObject* listener;
		std::vector<IngameObject*> objects_list;

		if (KrilloudRT::IsConectionActive()) 
		{
			listener = server_listener_object;
			objects_list = server_session_loaded_objects;
		}
		else
		{
			listener = listener_object;
			objects_list = session_loaded_objects;
		}

		if (listener != nullptr)
		{
			result.push_back(*listener);
		}

		for (size_t x = 0; x < objects_list.size(); x++)
		{
			IngameObject new_object;

			if (objects_list[x] != nullptr)
			{
				new_object.object_id = objects_list[x]->object_id;

				new_object.position_x = objects_list[x]->position_x;
				new_object.position_y = objects_list[x]->position_y;
				new_object.position_z = objects_list[x]->position_z;

				new_object.aVelX = objects_list[x]->aVelX;
				new_object.aVelY = objects_list[x]->aVelY;
				new_object.aVelZ = objects_list[x]->aVelZ;
			}
			else
			{
				new_object.object_id = -1;
			}

			result.push_back(new_object);
		}

		return result;
	}

	// --- Channel functions ----------------------------------------------------------------------------------------------------------------------------
	
	bool CreateNewChannel(std::string channel_name, int channel_id, int parent_id)
	{
		if (parent_id == channel_id)
			return false;

		if (parent_id < -1 || channel_id < 0)
			return false;
		
		if (parent_id == -1) 
		{
			//The only channel with no parent is Master and it always has to have id 0
			if (channel_id != 0)
				return false;

			for (size_t x = 0; x < channels.size(); x++)
			{
				//channels can't share ids
				if (channels[x]->channel_id == channel_id)
					return false;

				//Only Master channel has id -1.
				if (channels[x]->parent_id == -1)
					return false;
			}
		}
		else
		{
			for (size_t x = 0; x < channels.size(); x++)
			{
				//channels can't share ids
				if (channels[x]->channel_id == channel_id)
					return false;

				if (channels[x]->parent_id == parent_id)
				{
					//childs of the same parent can't have the same name
					if (channels[x]->channel_name == channel_name)
						return false;
				}
			}
		}

		Channel* new_channel = new Channel();

		new_channel->channel_name = channel_name;
		new_channel->channel_id = channel_id;
		new_channel->parent_id = parent_id;
		new_channel->volume = 1;

		channels.push_back(new_channel);

		return true;
	}

	bool RemoveChannel(int channel_id)
	{
		//Master channel can't be deleted
		if (channel_id == 0)
			return false;

		for (size_t x = 0; x < channels.size(); x++)
		{
			if (channels[x]->channel_id == channel_id)
			{
				channels.erase(channels.begin() + x);

				for (int y = (channels.size() - 1); y >= 0; y--)
				{
					if (channels[y]->parent_id == channel_id)
					{
						RemoveChannel(channels[y]->channel_id);
					}
				}
				return true;
			}
		}
		return false;
	}

	Channel* GetChannel(int channel_index)
	{
		if (channel_index >= 0 && channel_index < channels.size())
		{
			return channels[channel_index];
		}
		return nullptr;
	}

	Channel* GetChannelByID(int channel_id)
	{
		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_id == channel_id) 
			{
				return channels[x];
			}
		}
		return nullptr;
	}

	int GetChannelCount() 
	{
		return (int)channels.size();
	}

	std::string GetChannelName(int channel_index)
	{
		if (channel_index >= 0 && channel_index < channels.size())
		{
			return channels[channel_index]->channel_name;
		}

		return "";
	}

	float GetChannelVolume(int channel_index)
	{
		if (channel_index >= 0 && channel_index < channels.size())
		{
			return channels[channel_index]->volume;
		}

		return 0;
	}

	float GetChannelVolume(std::string channel_name)
	{
		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_name == channel_name) 
			{
				return GetChannelVolume(x);
			}
		}
		return 0;
	}
	
	float GetChannelOverallVolume(int channel_index)
	{
		if (channel_index >= 0 && channel_index < channels.size())
		{
			if (channels[channel_index]->parent_id >= 0)
			{
				for (size_t x = 0; x < channels.size(); x++) 
				{
					if (channels[x]->channel_id == channels[channel_index]->parent_id) 
					{
						return ((channels[channel_index]->volume * GetChannelOverallVolume(x)));
					}
				}
			}
			else
			{
				return channels[channel_index]->volume;
			}
		}
		return 0;
	}
	
	//INTERNAL
	bool ChannelHasChildName(int parent_id, std::string new_name) 
	{
		for (size_t x = 0; x < channels.size(); x++)
		{
			if (channels[x]->parent_id == parent_id && channels[x]->channel_name == new_name)
				return true;
		}
		return false;
	}

	//INTERNAL
	bool ChannelIDExists(int channel_id) 
	{
		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_id == channel_id)
				return true;				
		}
		return false;
	}

	//INTERNAL
	bool ChannelIsDescendant(int channel_id, int parent_id) 
	{
		if (channel_id == parent_id)
			return true;

		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_id == channel_id) 
			{
				if (channels[x]->parent_id == parent_id)
					return true;

				return ChannelIsDescendant(channels[x]->parent_id, parent_id);
			}
		}

		return false;
	}

	bool ChangeChannelParent(int channel_id, int parent_id)
	{
		//A channel can't be parent of itself
		if (channel_id == parent_id)
			return false;

		//Master channel can't have a parent channel
		if (channel_id == 0)
			return false;

		//Only Master can have no parent
		if (parent_id < 0)
			return false;

		// parent_id has to exist
		if (!ChannelIDExists(parent_id))
			return false;

		// channel and parent can't be related already, to prevent loops
		if ((ChannelIsDescendant(channel_id, parent_id)) || (ChannelIsDescendant(parent_id, channel_id)))
			return false;

		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_id == channel_id) 
			{
				// Parent can't have a channel with the same name has the one to be changed
				if (ChannelHasChildName(parent_id, channels[x]->channel_name))
					return false;

				channels[x]->parent_id = parent_id;
				return true;
			}
		}
		return false;
	}

	bool ChangeChannelName(int channel_id, std::string new_name)
	{
		for (size_t x = 0; x < channels.size(); x++)
		{
			if (channels[x]->channel_id == channel_id)
			{
				if (ChannelHasChildName(channels[x]->parent_id, new_name))
					return false;

				channels[x]->channel_name = new_name;
				return true;
			}
		}
		return false;
	}

	void ChangeChannelVolume(int channel_id, float volume)
	{
		for (size_t x = 0; x < channels.size(); x++)
		{
			if (channels[x]->channel_id == channel_id)
			{
				if (volume >= 0)
				{
					if (volume <= 1)
					{
						channels[x]->volume = volume;
					}
					else
					{
						channels[x]->volume = 1;
					}
				}
				else
				{
					channels[x]->volume = 0;
				}
				break;
			}
		}
	}

	void ChangeChannelVolume(std::string channel_name, float volume)
	{
		for (size_t x = 0; x < channels.size(); x++) 
		{
			if (channels[x]->channel_name == channel_name) 
			{
				ChangeChannelVolume(channels[x]->channel_id, volume);
				break;
			}
		}
		
	}
	   
	// --- KClass functions -----------------------------------------------------------------------------------------------------------------------------
	
	//INTERNAL
	KrilloudClass* GetKClass(int id, std::vector<KrilloudClass*> class_container) 
	{
		for (size_t x = 0; x < class_container.size(); x++) 
		{
			if (class_container[x]->internal_id == id) 
			{
				return class_container[x];
			}
		}
		return nullptr;
	}

	KrilloudClass* CreateKClass(int class_type, std::string class_name, int class_id, std::vector<float> initial_parameters, int object_id)
	{
		NodeClass node_class = (NodeClass)class_type; //REVISION: Unify with KrilloudClass::KrilloudFilterType
		KrilloudClass* new_KClass = nullptr;

		/*if (node_class == Class_ERROR) 
		{
			return nullptr;
		}*/

		switch (node_class)
		{
		case Class_ERROR:
			return nullptr;
		case Class_AudioContainer: 
		{
			//std::vector<LoadedSound*>* loaded_audio_ptr = &session_loaded_audios;
			new_KClass = new AudioContainer(&session_loaded_audios, engine_instance);

			if (new_KClass != nullptr && initial_parameters.size() > 0) 
			{
				((AudioContainer*)new_KClass)->SetLoop(initial_parameters[0] == 1);
				((AudioContainer*)new_KClass)->ChangeSelection(initial_parameters[1] == 1 ? 1 : 0);

				for (size_t y = 2; y < initial_parameters.size(); y += 2)
				{
					((AudioContainer*)new_KClass)->AddSoundID((int)initial_parameters[y], initial_parameters[y + 1]);
				}
			}
		}	
			break;
		case Class_F_BandPass:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::BandPass);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::BandPass);
			break;
		case Class_F_Bassboost:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::BassBoost);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::BassBoost);
			break;
		case Class_F_BQR:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::BQR);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::BQR);
			break;
		case Class_F_DC_Remover:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::DC_Remover);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::DC_Remover);
			break;
		case Class_F_Delay:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Delay);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Delay);
			break;
		case Class_F_Echo:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Echo);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Echo);
			break;
		case Class_F_Flanger:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Flanger);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Flanger);
			break;
		case Class_F_HighPass:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::HighPass);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::HighPass);
			break;
		case Class_F_LoFi:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::LoFi);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::LoFi);
			break;
		case Class_F_LowPass:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::LowPass);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::LowPass);
			break;
		case Class_F_Pitch:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Pitch);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Pitch);
			break;
		case Class_F_Robotizer:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Robotizer);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Robotizer);
			break;
		case Class_F_WaveShaper:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::WaveShaper);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::WaveShaper);
			break;
		case Class_F_Freeverb:
			//new_KClass = new KrilloudFilter(initial_parameters, KrilloudClass::Freeverb);
			new_KClass = engine_instance->InstanceFilter(initial_parameters, KrilloudClass::Freeverb);
			break;
		case Class_IngameVariable:
		{
			new_KClass = new KrilloudVar();

			for (size_t x = 0; x < contract.variables.size(); x++)
			{
				if (contract.variables[x].variable_name == class_name)
				{
					bool found = false;
					std::stringstream ss;

					std::string var_name = class_name;
					if (contract.variables[x].type == Local) 
					{
						ss << class_name << local_variable_separator << object_id;
						var_name = ss.str();
					}
					
					for (size_t x = 0; x < session_variables.size(); x++)
					{
						if (session_variables[x] != nullptr && session_variables[x]->variable_name == var_name)
						{
							((KrilloudVar*)new_KClass)->SetVariableData(session_variables[x]);
							found = true;
							break;
						}
					}

					if (!found)
					{
						VariableData var_data;

						var_data.content = contract.variables[x].content;
						var_data.current_value = 0;
						var_data.range_max = contract.variables[x].range_max;
						var_data.range_min = contract.variables[x].range_min;
						var_data.source = contract.variables[x].source;
						var_data.type = contract.variables[x].type;
						var_data.set_only_on_play = contract.variables[x].set_only_on_play;

						var_data.variable_name = var_name;
						
						((KrilloudVar*)new_KClass)->SetVariableData(CreateNewKVariable(var_data));
						break;
					}
				}
			}
		}

		break;
		case Class_Randomizer:
			break;
		case Class_Input:

			break;
		case Class_Macro:
			new_KClass = new KrilloudMacro();
			break;
		case Class_Output:
			new_KClass = new KrillOutput();
			break;
		case Class_Selector:
			new_KClass = new KrilloudSelector();
			break;
		case Class_Mixer:
			new_KClass = new KrilloudMixer();
			break;
		}

		if (new_KClass != nullptr) 
		{
			new_KClass->internal_id = class_id;
			new_KClass->node_class = node_class;
		}

		return new_KClass;
	}

	// --- Loaded audios list functions -----------------------------------------------------------------------------------------------------------------


	//INTERNAL
	std::vector<LoadedSound*>* GetLoadedSoundsList() 
	{
		return &session_loaded_audios;
	}

	//INTERNAL
	bool AudioIsLoaded(int audio_id)
	{
		for (size_t x = 0; x < session_loaded_audios.size(); x++) 
		{
			if (session_loaded_audios[x]->id == audio_id)
			{
				return true;
			}
		}
		return false;
	}

	//INTERNAL
	void UnloadAudio(int id) 
	{
		for (size_t x = 0; x < session_loaded_audios.size(); x++)
		{
			if (session_loaded_audios[x]->id == id)
			{
				session_loaded_audios.erase(session_loaded_audios.begin() + x);
				break;
			}
		}
	}

	//INTERNAL
	void UnloadSoundList()
	{
		for (size_t y = 0; y < session_loaded_audios.size(); y++)
		{
			if (session_loaded_audios[y]->owners.size() <= 0)
			{
				delete session_loaded_audios[y];
				session_loaded_audios.erase(session_loaded_audios.begin() + y);
			}
		}
	}

	//INTERNAL
	bool AddLoadedAudioOwner(int audio_id, std::string tag_name)
	{
		for (size_t x = 0; x < session_loaded_audios.size(); x++)
		{
			if (session_loaded_audios[x]->id == audio_id)
			{
				for (size_t y = 0; y < session_loaded_audios[x]->owners.size(); y++) 
				{
					if (session_loaded_audios[x]->owners[y] == tag_name)
					{
						//already registered
						KrilloudDebug::DebugWrite({"Add Owner to loaded audio: Already registered. Id=", std::to_string(audio_id), ", tag: ", tag_name });
						return false;
					}
				}
				session_loaded_audios[x]->owners.push_back(tag_name);

				KrilloudDebug::DebugWrite({"Add Owner to loaded audio: id=", std::to_string(audio_id), ", tag: ", tag_name});

				return true;
			}
		}
		KrilloudDebug::DebugWrite({ "Add Owner to loaded audio: No audio found. id=", std::to_string(audio_id), ", tag: ", tag_name });
		return false;
	}

	//INTERNAL
	bool RemoveLoadedAudioOwner(int audio_id, std::string tag_name)
	{
		for (size_t x = 0; x < session_loaded_audios.size(); x++)
		{
			if (session_loaded_audios[x]->id == audio_id)
			{
				for (size_t y = 0; y < session_loaded_audios[x]->owners.size(); y++)
				{
					if (session_loaded_audios[x]->owners[y] == tag_name)
					{
						session_loaded_audios[x]->owners.erase(session_loaded_audios[x]->owners.begin() + y);
						return true;
					}
				}
				return false;
			}
		}
		return false;
	}

	//INTERNAL
	void LoadAudioFromSoundbank(int id)
	{
		std::stringstream load_ss;
		load_ss << "Load Audio ID " << id << ": ";

		if (engine_instance == nullptr) 
		{
			load_ss << "Failed. Engine instance is null.";
			KrilloudDebug::DebugWrite(load_ss.str());
			return;
		}

		if (!engine_instance->AudioIsLoaded(id))
		{
			KrilloudDebug::DebugWrite("LoadAudioFromSoundbank: Audio is NOT loaded. Proceed to load it.");
			int index = -1;
			for (size_t x = 0; x < contract.sounbank_files.size(); x++)
			{
				if (contract.sounbank_files[x].id == id)
				{
					index = x;
					break;
				}
			}

			if (index > -1)
			{
				LoadedSound* newSound = new LoadedSound();
				newSound->id = id;

#ifdef __ANDROID__
				try
				{
					//AAssetManager* mgr = AAssetManager_fromJava(jni_env, my_asset_manager);

					char path[1024];
					strcpy(path, contract_full_path.c_str());
					strcat(path, "/");
					strcat(path, contract.sounbank_files[index].bankFile.c_str());

					/*std::stringstream sb_path;
					sb_path << contract_full_path << "/" << contract.sounbank_files[index].bankFile;
					std::string str = sb_path.str();
					char *cstr = new char[str.length() + 1];
					strcpy(cstr, str.c_str());*/

					AndroidFile* androidFile = new AndroidFile(mgr, path);

					if (!androidFile->EverythingOK())
						return;

					/*std::string fileContent = androidFile->readCompletely();
					char *fileContentChar = new char[fileContent.length() + 1];
					strcpy(fileContentChar, fileContent.c_str());*/
					
					// -----------------------

					// get size of file
					//ifs.seekg(0, ifs.end);
					long size = contract.sounbank_files[index].size;
					//ifs.seekg(contract.sounbank_files[index].offsetBytes);
					androidFile->Seek(contract.sounbank_files[index].offsetBytes);

					// allocate memory for file content
					char* buffer = new char[size];

					// read content of infile
					androidFile->Read(buffer, size);
					//ifs.read(buffer, size);

					// -----------------------

					//newSound->source = engine_instance->LoadAudioFromBuffer(id, fileContentChar, fileContent.size());
					newSound->source = engine_instance->LoadAudioFromBuffer(id, buffer, size);

					
					//unsigned int result = source->loadFile(androidFile);
					session_loaded_audios.push_back(newSound);

				}
				catch (...)
				{

				}
#else
				std::stringstream sb_path;
				sb_path << contract_full_path << "\\" << contract.sounbank_files[index].bankFile;

				try
				{
					std::ifstream ifs(sb_path.str(), std::ios::binary);
					if (ifs)
					{
						// get size of file
						ifs.seekg(0, ifs.end);
						long size = contract.sounbank_files[index].size;
						ifs.seekg(contract.sounbank_files[index].offsetBytes);

						// allocate memory for file content
						char* buffer = new char[size];

						// read content of infile
						ifs.read(buffer, size);

						newSound->source = engine_instance->LoadAudioFromBuffer(id, buffer, size);
						//newSound->sound.loadMem(reinterpret_cast<unsigned char*>(buffer), size);

						ifs.close();

						session_loaded_audios.push_back(newSound);

						load_ss << "Success. Loaded file " << index << " from soundbank " << contract.sounbank_files[index].bankFile;
						KrilloudDebug::DebugWrite(load_ss.str());
					}
				}
				catch (...)
				{
					load_ss << "Failed. Exception found.";
					KrilloudDebug::DebugWrite(load_ss.str());
				}
#endif		
			}
			else 
			{
				load_ss << "Failed. File not found in soundbanks.";
				KrilloudDebug::DebugWrite(load_ss.str());
			}
		}
		else 
		{
			KrilloudDebug::DebugWrite("LoadAudioFromSoundbank: Audio is already loaded.");
		}
	}

	//INTERNAL
	int LoadAudioFromDisk(int id, std::string filename)
	{
		if (engine_instance == nullptr)
			return 0;

		if (!AudioIsLoaded(id))
		{
			LoadedSound* newSound = new LoadedSound();
			newSound->id = id;
			
			newSound->source = engine_instance->LoadAudioFromDisk(id, filename);
			//newSound->sound.load(filename.c_str());

			session_loaded_audios.push_back(newSound);

			std::stringstream debug_ss;
			debug_ss << "AUDIO LOADED FROM DISK.\nName: " << filename << "\nID: " << id;
			KrilloudDebug::DebugWrite(debug_ss.str());

			return 1;
		}
		return 0;
	}

	//INTERNAL
	int LoadLevelAudioSources(KLevel* level, std::string owner_tag)
	{
		int count = 0;

		for (size_t x = 0; x < level->nodes.size(); x++)
		{
			AudioContainer* container_node = dynamic_cast<AudioContainer*>(level->nodes[x]);

			if (container_node != nullptr)
			{
				if (container_node->content_list.size() <= 0) 
				{
					KrilloudDebug::DebugWrite("Container found but it's empty");
				}

				for (size_t y = 0; y < container_node->content_list.size(); y++)
				{
					//Try to register the tag to the loaded audio.
					if (!AddLoadedAudioOwner(container_node->content_list[y]->id, owner_tag))
					{
						//If the audio is not already loaded, then load it
						if (working_on_game)
						{
							KrilloudDebug::DebugWrite("Load Audio from soundbank");
							//Get audio from the soundbanks
							LoadAudioFromSoundbank(container_node->content_list[y]->id);
						}
						else
						{
							KrilloudDebug::DebugWrite("Load Audio from disk");
							//Get audio from the disk
							count += LoadAudioFromDisk(container_node->content_list[y]->id, container_node->content_list[y]->filename);
						}

						//And try again.
						AddLoadedAudioOwner(container_node->content_list[y]->id, owner_tag);
					}
				}
			}
		}

		for (size_t x = 0; x < level->sublevels.size(); x++) 
		{
			count += LoadLevelAudioSources(level->sublevels[x], owner_tag);
		}

		return count;
	}
	
	// --- Instantiation --------------------------------------------------------------------------------------------------------------------------------
	
	//INTERNAL
	KLevel* InstantiateTagLevel(LevelResult* graphLevel_info, int object_id)
	{
		KLevel* new_level = new KLevel();

		//load nodes
		for (size_t x = 0; x < graphLevel_info->graphNodes_info.size(); x++)
		{
			KrilloudClass* new_KClass = CreateKClass(graphLevel_info->graphNodes_info[x].type, graphLevel_info->graphNodes_info[x].Name, graphLevel_info->graphNodes_info[x].ID, graphLevel_info->graphNodes_info[x].values, object_id);
			if (new_KClass != nullptr)
			{
				for (int y = 0; y < graphLevel_info->graphNodes_info[x].sockets.size(); y++)
				{
					new_KClass->CreateNewSocket(graphLevel_info->graphNodes_info[x].sockets[y].SocketID, graphLevel_info->graphNodes_info[x].sockets[y].connection_type, graphLevel_info->graphNodes_info[x].sockets[y].data_type);
				}

				if (new_KClass->node_class == Class_Mixer) 
				{
					std::stringstream mixer_ss;
					for (size_t y = 0; y < graphLevel_info->graphNodes_info[x].values.size(); y++)
					{
						((KrilloudMixer*)new_KClass)->SetVolume(y, graphLevel_info->graphNodes_info[x].values[y]);
					}
				}

				new_level->nodes.push_back(new_KClass);
			}
		}

		//load links
		for (size_t x = 0; x < graphLevel_info->graphNodes_info.size(); x++)
		{
			KrilloudClass* this_node = GetKClass(graphLevel_info->graphNodes_info[x].ID, new_level->nodes);

			if (this_node != nullptr)
			{
				for (size_t y = 0; y < graphLevel_info->graphNodes_info[x].sockets.size(); y++) 
				{
					for (size_t z = 0; z < graphLevel_info->graphNodes_info[x].sockets[y].links.size(); z++) 
					{
						KrilloudClass* connected_node = GetKClass(graphLevel_info->graphNodes_info[x].sockets[y].links[z].connected_node_id, new_level->nodes);

						if (connected_node != nullptr)
						{
							this_node->AddLink(graphLevel_info->graphNodes_info[x].sockets[y].links[z].my_socket_id, connected_node);
						}
					}
				}
			}
		}

		//load sublevels
		for (size_t x = 0; x < graphLevel_info->graphLevels_info.size(); x++)
		{
			KLevel* new_sub_level = InstantiateTagLevel(&graphLevel_info->graphLevels_info[x], object_id);

			KrilloudClass* node = GetKClass(new_sub_level->associated_macro_id, new_level->nodes);

			if (node != nullptr)
			{
				KrilloudMacro* macro = dynamic_cast<KrilloudMacro*>(node);

				if (macro != nullptr) 
				{
					macro->SetLevel(new_sub_level);
				}
			}

			new_level->sublevels.push_back(new_sub_level);
		}

		return new_level;
	}

	//INTERNAL
	void InstantiateTag(LevelResult* graphLevel_info, int object_id)
	{
		if (graphLevel_info != nullptr) 
		{
			std::stringstream ss;
			ss << graphLevel_info->tagname << object_id;

			CompleteKrillTag new_tag;
			new_tag.tag.tag_name = ss.str();
			new_tag.tag_content = InstantiateTagLevel(graphLevel_info, object_id);
			
			new_tag.tag.is3D = graphLevel_info->is3Dtag;
			new_tag.tag.min_3D_distance = graphLevel_info->Min3DDistance;
			new_tag.tag.max_3D_distance = graphLevel_info->Max3DDistance;
			new_tag.tag.roll_off_factor = graphLevel_info->roll_off_factor;
			new_tag.tag.attenuation = graphLevel_info->attenuationType;

			new_tag.tag.doppler_active = graphLevel_info->doppler_active;
			new_tag.tag.doppler_factor = graphLevel_info->doppler_factor;

			new_tag.tag.distance_delay = graphLevel_info->distance_delay;

			new_tag.tag.isLoopable = graphLevel_info->isLoopable;

			new_tag.tag.channel_id = graphLevel_info->channel_id;

			new_tag.tag.owner_id = object_id;
			my_tags.push_back(new_tag);
		}
	}

	// --- Auxiliary functions --------------------------------------------------------------------------------------------------------------------------

	bool IsKrilloudOn() 
	{
		return is_Krilloud_ready;
	}

	void SwitchSystemPause()
	{
		if (!is_Krilloud_ready)
			return;
		engine_paused = !engine_paused;
		engine_instance->SetPauseAll(engine_paused);
	}

	bool IsKrilloudPaused()
	{
		return engine_paused;
	}

	int GetActiveVoicesCount()
	{
		if (!is_Krilloud_ready)
			return -1;

		return 0;
		//return engine_instance->getActiveVoiceCount();	
	}

	int GetVoiceCount()
	{
		if (!is_Krilloud_ready)
			return -1;
		
		return 0;
		//return soloud_instance->getVoiceCount();
	}

	int GetMaxVoices()
	{
		if (!is_Krilloud_ready)
			return -1;
		
		return 0;
		//return soloud_instance->getMaxActiveVoiceCount();
	}

	//INTERNAL
	bool IsPathCorrect(std::string path) 
	{
#ifndef  __ANDROID__
		auto it = std::experimental::filesystem::directory_iterator(path);
		auto status = it->status();
		bool exists = std::experimental::filesystem::exists(status);

		bool is_directory = std::experimental::filesystem::is_directory(status);

		return exists && is_directory;
#else
		return true;
#endif
	}

	//INTERNAL
	int KrilloudIsClean() 
	{
		if (is_Krilloud_ready != false)
			return -1;
		
		if (playing_tags_checker != nullptr)
			return -2;

		if (engine_instance != nullptr)
			return -3;

		if (listener_object != nullptr)
			return -4;

		if (playing_tags.size() > 0)
			return -5;

		if (session_loaded_audios.size() > 0)
			return -6;

		if (session_loaded_objects.size() > 0)
			return - 7;

		if (session_variables.size() > 0)
			return -8;

		if (my_tags.size() > 0)
			return -9;

		if (channels.size() > 0)
			return -10;

		return 0;
	}

	//INTERNAL
	KrilloudProjectInfo GetContractData() 
	{
		return contract;
	}

	//INTERNAL
	bool LoadContract(const char* contract_path)
	{
		KrilloudProjectInfo info;

		if (!IsPathCorrect(contract_path))
			return false;

		char path[1024];
#ifdef __ANDROID__
		strcpy(path, contract_path);
		strcat(path, "/Contract.json");

#else
		strcpy_s(path, contract_path);
		strcat_s(path, "\\Contract.json");
#endif
		
		

		try
		{
#ifdef __ANDROID__
			//AAssetManager* mgr = AAssetManager_fromJava(jni_env, my_asset_manager);
			AndroidFile* androidFile = new AndroidFile(mgr, path);

			if (androidFile == nullptr || !androidFile->EverythingOK())
				return false;

			std::string fileResult = androidFile->ReadCompletely();

			std::stringstream ss;
			ss << fileResult;

			cereal::JSONInputArchive iarchive(ss);
			iarchive(info);
#else
			std::ifstream inf(path, std::ios_base::in | std::ios::binary);
			cereal::JSONInputArchive iarchive(inf);
			iarchive(info);
			inf.close();
#endif
			
			if ((info.channels.size() > 0) && ((original_contract.channels.size() <= 0) || (original_contract.creation_time != info.creation_time)))
			{
				//Temp until everyone uses next version
				int temp_id = 0;
				for (size_t x = 0; x < info.channels.size(); x++) 
				{
					if (info.channels[x].channel_id < 0) 
					{
						info.channels[x].channel_id = temp_id;
						temp_id++;
					}
				}

				original_contract = info;
				contract = info;
			}

			return true;
		}
		catch (std::exception &e) 
		{
			return false;
		}
		catch (...)
		{
			return false;
		}
	}

	std::string GetContractJSON(std::string contract_path)
	{
		try
		{
			if (!LoadContract(contract_path.c_str()))
				return "Error. Contract load failed.";

			KrilloudProjectInfo info = original_contract;

			std::stringstream ss;
			{
				cereal::JSONOutputArchive oarchive(ss);

				oarchive(info);
			}
			return ss.str();
		}
		catch (cereal::RapidJSONException &e)
		{
			return "Error. JSON exception.";
		}
		catch (std::exception& e)
		{
			std::stringstream ss;
			ss << "Error. " << e.what();
			return ss.str();;
		}
		catch (...)
		{
			return "Error.";
		}
	}
}