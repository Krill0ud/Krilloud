#ifndef KRILLOUD_DEFINITIONS_H
#define KRILLOUD_DEFINITIONS_H

enum SupportedPlatform { Windows, Android };
inline const char* ToString(SupportedPlatform platform)
{
	switch (platform)
	{
	case Windows:   return "Windows";
	case Android:   return "Android";
	default: return "";
	}
}

enum AudioEngineBackend { DefaultBackend, WinMM, XAudio2, Wasapi, OpenSLES };
enum AudioEngineBuffer { DefaultBuffer, b32, b64, b128, b256, b512, b1024, b2048, b4096 };
enum AudioEngineChannel { DefaultChannels, Mono, Stereo, Sorround4, Sorround5, Sorround7 };
enum AudioEngineBitDepth { DefaultBitDepth, b8, b16, b24 };
enum AudioEngineSampleRate { DefaultSampleRate, s44100, s48000, s88200, s96000 };

enum NodeType { Node_Normal, Node_Variable, Node_Selector, Node_Output, Node_Container, Node_Filter, Node_Mixer };
enum NodeClass { Class_ERROR, Class_AudioContainer, Class_Macro, Class_F_BandPass, Class_F_Bassboost, Class_F_BQR, Class_F_DC_Remover, Class_F_Delay, Class_F_Echo, Class_F_Flanger, Class_F_HighPass, Class_F_LoFi, Class_F_LowPass, Class_F_Pitch, Class_F_Robotizer, Class_F_WaveShaper, Class_IngameVariable, Class_Input, Class_Output, Class_Randomizer, Class_Selector, Class_F_Freeverb, Class_Mixer };
enum VarSourceType { Random_Repetition, Random_No_Repetition, InGame };
enum VarContentType { Ranged, Raw };
enum VarType { Global, Local };
enum SocketType { Input, Output };
enum SocketDataType { Normal, Variable };

#endif //KRILLOUD_DEFINITIONS_H
