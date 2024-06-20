#pragma once
//#include <vector>
//#include "NodeInfoStructures.h"
//#include "cereal/archives/json.hpp"
//#include "cereal/archives/binary.hpp"
//#include "cereal/types/string.hpp"
//#include "cereal/types/vector.hpp"
//
//static int current_version = 20200928;
//static int last_version = 20191030;
//
//struct IngameObject
//{
//	int object_id;
//	float position_x;
//	float position_y;
//	float position_z;
//	float aVelX;
//	float aVelY;
//	float aVelZ;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(
//			CEREAL_NVP(object_id),
//			CEREAL_NVP(position_x),
//			CEREAL_NVP(position_y),
//			CEREAL_NVP(position_z),
//			CEREAL_NVP(aVelX),
//			CEREAL_NVP(aVelY),
//			CEREAL_NVP(aVelZ));
//	}
//};
//
//struct PlayingTag
//{
//	size_t id;
//	IngameObject* parent_object;
//	std::string tagname;
//
//	int playHandle;
//	TagInfo* info;
//
//	int channel_id;
//
//	bool is3D;
//
//	bool stop;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(
//			CEREAL_NVP(id),
//			CEREAL_NVP(tagname),
//			CEREAL_NVP(playHandle),
//			CEREAL_NVP(channel_id),
//			CEREAL_NVP(is3D),
//			CEREAL_NVP(stop));
//	}
//};
//
//// --- Krilloud save structures
//
//struct VariableData
//{
//	std::string variable_name;
//	float current_value;
//	VarType type;
//	VarSourceType source;
//	VarContentType content;
//	float range_min;
//	float range_max;
//	bool set_only_on_play;
//
//	std::string previous_name;
//
//	VariableData()
//	{
//	}
//
//	VariableData(const VariableData & source)
//		: variable_name(source.variable_name),
//		current_value(source.current_value),
//		type(source.type),
//		source(source.source),
//		content(source.content),
//		range_min(source.range_min),
//		range_max(source.range_max),
//		set_only_on_play(source.set_only_on_play),
//		previous_name(source.previous_name)
//	{
//	}
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(variable_name), CEREAL_NVP(current_value), CEREAL_NVP(type), CEREAL_NVP(source), CEREAL_NVP(content), CEREAL_NVP(range_min), CEREAL_NVP(range_max), CEREAL_NVP(set_only_on_play));
//	}
//};
//
//struct KrillFileDescriptor
//{
//	int			id;
//	std::string bankFile;
//	size_t      offsetBytes;
//	size_t      size;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(id)
//			, CEREAL_NVP(bankFile)
//			, CEREAL_NVP(offsetBytes)
//			, CEREAL_NVP(size));
//	}
//};
//
//struct KrillInfo
//{
//	std::vector<KrillFileDescriptor> files;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(files));
//	}
//};
//
//struct ConnectionsSave
//{
//	int my_socket_id;
//	int connected_node_id;
//	int connected_node_socket_id;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(my_socket_id), CEREAL_NVP(connected_node_id), CEREAL_NVP(connected_node_socket_id));
//	}
//};
//
//struct NodeSocket
//{
//	int SocketID;
//	SocketType connection_type;
//	SocketDataType data_type;
//	std::vector<ConnectionsSave> links;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(SocketID), CEREAL_NVP(connection_type), CEREAL_NVP(data_type), CEREAL_NVP(links));
//	}
//};
//
//struct GraphSave {
//	int			type;
//	int			ID;
//	std::string	Name;
//	float		Pos_x;
//	float		Pos_y;
//	float		Size_x;
//	float		Size_y;
//	bool		can_be_deleted;
//	int			inputsCount;
//	int			outputsCount;
//
//	std::vector<NodeSocket> sockets;
//	std::vector<float> values;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(type), CEREAL_NVP(ID), CEREAL_NVP(Name), CEREAL_NVP(Pos_x), CEREAL_NVP(Pos_y), CEREAL_NVP(Size_x), CEREAL_NVP(Size_y), CEREAL_NVP(can_be_deleted), CEREAL_NVP(inputsCount), CEREAL_NVP(outputsCount), CEREAL_NVP(sockets), CEREAL_NVP(values));
//	}
//};
//
//struct LevelResult
//{
//	std::string tagname;
//
//	bool is3Dtag;
//	float Min3DDistance;
//	float Max3DDistance;
//	float roll_off_factor;
//	int attenuationType;
//	bool distance_delay;
//	bool doppler_active;
//	float doppler_factor;
//
//	bool isLoopable;
//
//	int channel_id;
//
//	std::string levelName;
//	int associated_ID;
//	int gids;
//	std::vector<GraphSave> graphNodes_info;
//	std::vector<LevelResult> graphLevels_info;
//
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		archive(CEREAL_NVP(tagname), CEREAL_NVP(is3Dtag), CEREAL_NVP(Min3DDistance), CEREAL_NVP(Max3DDistance), CEREAL_NVP(roll_off_factor), CEREAL_NVP(attenuationType), CEREAL_NVP(distance_delay), CEREAL_NVP(doppler_active), CEREAL_NVP(doppler_factor), CEREAL_NVP(isLoopable), CEREAL_NVP(channel_id), CEREAL_NVP(levelName), CEREAL_NVP(associated_ID), CEREAL_NVP(gids), CEREAL_NVP(graphNodes_info), CEREAL_NVP(graphLevels_info));
//	}
//};
//
//struct Channel
//{
//	int channel_id;
//	std::string channel_name;
//	int parent_id;
//	float volume;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		if (version == current_version)
//		{
//			archive(CEREAL_NVP(channel_id), CEREAL_NVP(channel_name), CEREAL_NVP(parent_id), CEREAL_NVP(volume));
//		}
//		else
//		{
//			archive(CEREAL_NVP(channel_name), CEREAL_NVP(parent_id), CEREAL_NVP(volume));
//			channel_id = -1;
//		}
//	}
//};
//
//struct KrilloudProjectInfo {
//
//	std::string project_path;
//	std::string game_path;
//	std::string project_name;
//
//	std::vector<LevelResult> tags;
//
//	std::vector<KrillFileDescriptor> sounbank_files;
//
//	std::vector<VariableData> variables;
//
//	std::vector<Channel> channels;
//
//	long int creation_time;
//
//	template<class Archive>
//	void serialize(Archive & archive, std::uint32_t const version)
//	{
//		if (version == current_version)
//		{
//			archive(CEREAL_NVP(project_path), CEREAL_NVP(game_path), CEREAL_NVP(project_name), CEREAL_NVP(tags), CEREAL_NVP(sounbank_files), CEREAL_NVP(variables), CEREAL_NVP(channels), CEREAL_NVP(creation_time));
//		}
//		else
//		{
//			archive(CEREAL_NVP(project_path), CEREAL_NVP(game_path), CEREAL_NVP(project_name), CEREAL_NVP(tags), CEREAL_NVP(sounbank_files), CEREAL_NVP(variables), CEREAL_NVP(channels));
//			creation_time = 0;
//		}
//	}
//};
//
//CEREAL_CLASS_VERSION(KrillFileDescriptor, last_version);
//CEREAL_CLASS_VERSION(KrillInfo, last_version);
//CEREAL_CLASS_VERSION(ConnectionsSave, last_version);
//CEREAL_CLASS_VERSION(NodeSocket, last_version);
//CEREAL_CLASS_VERSION(GraphSave, last_version);
//CEREAL_CLASS_VERSION(VariableData, last_version);
//CEREAL_CLASS_VERSION(LevelResult, last_version);
//CEREAL_CLASS_VERSION(Channel, current_version);
//CEREAL_CLASS_VERSION(KrilloudProjectInfo, current_version);