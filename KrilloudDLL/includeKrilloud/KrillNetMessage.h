//#ifndef CHAT_MESSAGE_HPP
//#define CHAT_MESSAGE_HPP

#define ENDCAPSULE "#@|KE|@#"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "NodeInfoStructures.h"

#include "KrilloudSerialized.h"
//#include "cereal/cereal.hpp"
//#include "cereal/types/string.hpp"
//#include "cereal/types/vector.hpp"
////#include <cereal/archives/json.hpp>
//#include "cereal/archives/json.hpp"
////#include <cereal/archives/binary.hpp>

class KrillNetMessage
{
public:
	enum { header_length = 4 };
	enum { max_body_length = 1024 };

	KrillNetMessage()
		: body_length(0)
	{
	}

	KrillNetMessage(std::string string)
	{
		Write(string);
	}

	/*const char* GetKrillData() const
	{
		return data_;
	}*/

	char* GetKrillData()
	{
		return data_;
	}

	std::size_t GetKrillDataLength() const
	{
		return header_length + body_length;
	}

	/*const char* GetKrillBody() const
	{
		return data_ + header_length;
	}*/

	char* GetKrillBody()
	{
		return data_ + header_length;
	}

	std::size_t GetKrillBodyLength() const
	{
		return body_length;
	}

	bool DecodeHeader()
	{
		char header[header_length + 1] = "";
		std::strncat(header, data_, header_length);
		body_length = std::atoi(header);
		if (body_length > max_body_length)
		{
			body_length = 0;
			return false;
		}
		return true;
	}

	void EncodeHeader()
	{
		char header[header_length + 1] = "";
		std::sprintf(header, "%4d", static_cast<int>(body_length));
		std::memcpy(data_, header, header_length);
	}

	void Write(const char* line) 
	{
		SetBodyLength(std::strlen(line));
		std::memcpy(GetKrillBody(), line, GetKrillBodyLength());
		EncodeHeader();
	}

	void Write(std::string string)
	{
		const char* line = string.c_str();
		SetBodyLength(std::strlen(line));
		std::memcpy(GetKrillBody(), line, GetKrillBodyLength());
		EncodeHeader();
	}

	std::string Read() 
	{
		std::stringstream ss;
		ss.write(GetKrillBody(), GetKrillBodyLength());
		return ss.str();
	}

private:
	char data_[header_length + max_body_length];
	std::size_t body_length;

	void SetBodyLength(std::size_t new_length)
	{
		body_length = new_length;
		if (body_length > max_body_length)
			body_length = max_body_length;
	}
};

//#endif 

class KCommand
{
public:
	enum Command { NullCommand, CreateNewNode, RemoveNode, FilterParameter, Channel_Name, Channel_Volume, Channel_Parent, Variable_Name, Variable_CurrentValue
		, Variable_Type, Variable_Source, Variable_Content, Variable_MinRange, Variable_MaxRange, Variable_SetOnPlay, Mixer_VolumeChanged };
	
	struct CommandData 
	{
		std::string target_name;
		int target_id;
		int element_index;

		std::string string_value;
		float float_value;
		int int_value;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(CEREAL_NVP(target_name), CEREAL_NVP(target_id), CEREAL_NVP(element_index), CEREAL_NVP(string_value), CEREAL_NVP(float_value), CEREAL_NVP(int_value));
		}
	};

	KCommand() 
		: command_type(NullCommand)
	{
	}

	KCommand(const KCommand &copy) 
		: command_type(copy.command_type),
		command_data(copy.command_data)
	{
	}
	
	KCommand(Command newCommand, CommandData newData) 
		: command_type(newCommand),
		command_data(newData)
	{
	}

	/*std::string ReadMessage()
	{
		std::stringstream ss;
		ss << "Command: " << commandTexts[command] << ", value: " << value << "\n";
		return ss.str();
	}*/

	std::string Serialized()
	{
		try
		{
			std::stringstream ss; // any stream can be used
			{
				cereal::JSONOutputArchive oarchive(ss); // Create an output archive

				oarchive(*this); // Write the data to the archive
			} // archive goes out of scope, ensuring all contents are flushed
			return ss.str();
		}
		catch (cereal::RapidJSONException &e)
		{
			SetError(true, "KCommand - Serialize. JSON EXCEPTION!");
			return "";
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
			return "";
		}

		return "";
	}

	void Deserialized(std::string serializedMessage)
	{
		try
		{
			std::stringstream ss;
			ss << serializedMessage;

			cereal::JSONInputArchive iarchive(ss); // Create an input archive

			iarchive(*this); // Read the data from the archive
		}
		catch (cereal::RapidJSONException &e)
		{
			SetError(true, "KCommand - Deserialize. JSON EXCEPTION!");
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
		}
	}

	bool Ok()
	{
		return !error;
	}

	std::string GetErrorMessage()
	{
		return errorString;
	}

	Command command_type;
	CommandData command_data;

	bool error = false;
	std::string errorString;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(command_type), CEREAL_NVP(command_data));
	}

private:
	void SetError(bool er, std::string message)
	{
		error = er;
message = errorString;
	}
};

class SystemObjectInfo
{
public:
	enum ObjectInfoType { Null, Listener, NewObject, RemoveObject, ChangeObject, NewPlayingTag, RemovePlayingTag, ChangePlayingTag };

	SystemObjectInfo()
		: info_type(Null),
		info_data("")
	{
	}

	SystemObjectInfo(const SystemObjectInfo &copy)
		: info_type(copy.info_type),
		info_data(copy.info_data)
	{
	}

	SystemObjectInfo(ObjectInfoType newType, std::string newData)
		: info_type(newType),
		info_data(newData)
	{
	}

	SystemObjectInfo(IngameObject object, ObjectInfoType type)
	{
		if ((int)type > 0 && (int)type < 5) 
		{
			info_type = type;
			try
			{
				std::stringstream ss; // any stream can be used
				{
					cereal::JSONOutputArchive oarchive(ss); // Create an output archive

					oarchive(object); // Write the data to the archive
				} // archive goes out of scope, ensuring all contents are flushed
				info_data = ss.str();
			}
			catch (cereal::RapidJSONException &e)
			{
				SetError(true, "SystemObjectInfo - Initialize object. JSON EXCEPTION!");
			}
			catch (std::exception& e)
			{
				SetError(true, e.what());
			}
		}
		else
		{
			SetError(true, "SystemObjectInfo - Initialize object. Wrong type assigned.");
		}
	}

	SystemObjectInfo(PlayingTag tag, ObjectInfoType type)
	{
		if ((int)type > 4 && (int)type < 8)
		{
			info_type = type;
			try
			{
				std::stringstream ss; // any stream can be used
				{
					cereal::JSONOutputArchive oarchive(ss); // Create an output archive

					oarchive(tag); // Write the data to the archive
				} // archive goes out of scope, ensuring all contents are flushed
				info_data = ss.str();
			}
			catch (cereal::RapidJSONException &e)
			{
				SetError(true, "SystemObjectInfo - Initialize tag. JSON EXCEPTION!");
			}
			catch (std::exception& e)
			{
				SetError(true, e.what());
			}
		}
		else
		{
			SetError(true, "SystemObjectInfo - Initialize tag. Wrong type assigned.");
		}
	}

	std::string Serialized()
	{
		try
		{
			std::stringstream ss; // any stream can be used
			{
				cereal::JSONOutputArchive oarchive(ss); // Create an output archive

				oarchive(*this); // Write the data to the archive
			} // archive goes out of scope, ensuring all contents are flushed
			return ss.str();
		}
		catch (cereal::RapidJSONException &e)
		{
			SetError(true, "SystemObjectInfo - Serialize. JSON EXCEPTION!");
			return "";
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
			return "";
		}

		return "";
	}

	void Deserialized(std::string serializedMessage)
	{
		try
		{
			std::stringstream ss;
			ss << serializedMessage;

			cereal::JSONInputArchive iarchive(ss); // Create an input archive

			iarchive(*this); // Read the data from the archive
		}
		catch (cereal::RapidJSONException &e)
		{
			SetError(true, "SystemObjectInfo - Deserialize. JSON EXCEPTION!");
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
		}
	}

	IngameObject GetResultObject()
	{
		IngameObject object;
		if ((int)info_type > 0 && (int)info_type < 5) 
		{
			try
			{
				std::stringstream ss;
				ss << info_data;

				cereal::JSONInputArchive iarchive(ss); // Create an input archive

				iarchive(object); // Read the data from the archive
				return object;
			}
			catch (cereal::RapidJSONException &e)
			{
				SetError(true, "SystemObjectInfo - GetResultObject. JSON EXCEPTION!");
				return object;
			}
			catch (std::exception& e)
			{
				SetError(true, e.what());
				return object;
			}
		}
		return object;
	}

	PlayingTag GetResultTag() 
	{
		PlayingTag result_tag;
		if ((int)info_type > 4 && (int)info_type < 8)
		{
			try
			{
				std::stringstream ss;
				ss << info_data;

				cereal::JSONInputArchive iarchive(ss); // Create an input archive

				iarchive(result_tag); // Read the data from the archive
				return result_tag;
			}
			catch (cereal::RapidJSONException &e)
			{
				SetError(true, "SystemObjectInfo - GetResultTag. JSON EXCEPTION!");
				return result_tag;
			}
			catch (std::exception& e)
			{
				SetError(true, e.what());
				return result_tag;
			}
		}
		return result_tag;
	}

	bool Ok()
	{
		return !error;
	}

	std::string GetErrorMessage()
	{
		return errorString;
	}

	ObjectInfoType info_type;
	std::string info_data;

	bool error = false;
	std::string errorString;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(info_type), CEREAL_NVP(info_data));
	}

private:
	

	void SetError(bool er, std::string message)
	{
		error = er;
		message = errorString;
	}
};

class SerializedMessage 
{
public:
	enum MessageType { Command, Contract, GameObjectInfo };
private:
	MessageType type;
	std::vector<std::string> messages;
	//std::string message;
	bool error = false;
	std::string errorString;

	void SetError(bool er, std::string message)
	{
		error = er;
		message = errorString;
	}
public:
	SerializedMessage(std::string receivedMessage) 
	{
		Deserialized(receivedMessage);
	}

	SerializedMessage(KCommand command) 
	{
		type = Command;
		messages.push_back(command.Serialized());
	}

	SerializedMessage(SystemObjectInfo info)
	{
		type = GameObjectInfo;
		messages.push_back(info.Serialized());
	}

	SerializedMessage(std::string newMessage, MessageType messageType) 
	{
		type = messageType;
		messages.push_back(newMessage);
	}

	bool Ok() 
	{
		return !error;
	}

	std::string GetErrorMessage() 
	{
		return errorString;
	}

	MessageType GetType() 
	{
		return type;
	}

	std::vector<std::string> GetContent() 
	{
		return messages;
	}

	bool AddMessage(KCommand command) 
	{
		if (type == Command) 
		{
			messages.push_back(command.Serialized());
			return command.Ok();
		}
		return false;
	}
	
	bool AddMessage(SystemObjectInfo info)
	{
		if (type == GameObjectInfo)
		{
			messages.push_back(info.Serialized());
			return info.Ok();
		}
		return false;
	}

	bool AddMessage(std::string newMessage, MessageType messageType) 
	{
		if (messageType == type) 
		{
			messages.push_back(newMessage);
			return true;
		}
		return false;
	}

	std::string Serialized()
	{
		try
		{
			std::stringstream ss; // any stream can be used
			{
				cereal::JSONOutputArchive oarchive(ss); // Create an output archive

				oarchive(*this); // Write the data to the archive
			} // archive goes out of scope, ensuring all contents are flushed

			ss << ENDCAPSULE;
			return ss.str();
		}
		catch (cereal::RapidJSONException &e)
		{
			SetError(true, "SerializedMessage - Serialize. JSON EXCEPTION!");
			return "";
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
			return "";
		}

		return "";
	}

	void Deserialized(std::string serializedMessage)
	{
		std::string endC = ENDCAPSULE;
		serializedMessage = serializedMessage.substr(0, serializedMessage.size() - endC.size());
		try
		{
			std::stringstream ss;
			ss << serializedMessage;

			cereal::JSONInputArchive iarchive(ss); // Create an input archive

			iarchive(*this); // Read the data from the archive
			error = false;
		}
		catch (cereal::RapidJSONException &e)
		{
			std::stringstream ss;
			ss << "SerializedMessage - Deserialize. JSON EXCEPTION!\n" << "Received:\n" << serializedMessage << "\n";
			SetError(true, ss.str());
		}
		catch (std::exception& e)
		{
			SetError(true, e.what());
		}
	}

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(CEREAL_NVP(type), CEREAL_NVP(messages));
	}
};