#include "KrilloudRT.h"

namespace KrilloudEngine 
{
	extern IngameObject* listener_object;
	extern std::vector<IngameObject*> session_loaded_objects;
	extern std::vector<LoadedSound*> session_loaded_audios;
	extern std::vector<VariableData*> session_variables;

	extern std::vector<Channel*> channels;
	extern std::vector<CompleteKrillTag> my_tags;
	extern std::vector<PlayingTag*> playing_tags;

	extern IngameObject* server_listener_object;
	extern std::vector<IngameObject*> server_session_loaded_objects;
	extern std::vector<PlayingTag> server_playing_tags;

	extern KrilloudProjectInfo contract;
}



namespace KrilloudRT
{
	using boost::asio::ip::tcp;

	void Context_Runner_thread(boost::asio::io_context* io_context)
	{
		try 
		{
			io_context->run();
		}
		catch (...) 
		{

		}
	}

	namespace KrilloudCommon
	{
		std::string GetContractJSON() 
		{
			try
			{
				std::stringstream ss; // any stream can be used
				{
					cereal::JSONOutputArchive oarchive(ss); // Create an output archive

					oarchive(KrilloudEngine::GetContractData()); // Write the data to the archive
				} // archive goes out of scope, ensuring all contents are flushed

				return ss.str();
			}
			catch (cereal::RapidJSONException &e)
			{
				//SetError(true, "SerializedMessage - Serialize. JSON EXCEPTION!");
				return "ERROR";
			}
			catch (std::exception& e)
			{
				//SetError(true, e.what());
				return "ERROR";
			}
		}

		KrilloudProjectInfo GetContractFromJSON(std::stringstream *ss)
		{
			KrilloudProjectInfo newContract;

			try
			{
				cereal::JSONInputArchive iarchive(*ss);

				iarchive(newContract);
			}
			catch (...)
			{
				newContract.game_path = "ERROR";
			}

			return newContract;
		}

		bool CheckIP(std::string ip)
		{
			int start = 0;
			int slices_count = 0;

			for (size_t x = 0; x < ip.size(); x++)
			{
				if ((ip[x] == '.') || (slices_count >= 3 && (x + 1) >= ip.size()))
				{
					std::string slice;

					if (ip[x] == '.')
					{
						slice = ip.substr(start, x - start);
					}
					else
					{
						slice = ip.substr(start);
					}

					start = x + 1;

					if (slice.find_first_not_of("0123456789") != std::string::npos)
						return false;

					try {
						int slice_int = std::stoi(slice);

						if (slice_int < 0 || slice_int > 255)
							return false;

						slices_count++;
					}
					catch (std::invalid_argument& e) {
						return false;
					}
					catch (std::out_of_range& e) {
						return false;
					}
					catch (...) {
						return false;
					}
				}
			}

			if (slices_count != 4)
				return false;

			return true;
		}

		bool CheckPort(int port)
		{
			return ((port >= 0) && (port <= 65535));
		}

		bool CheckPort(std::string port)
		{
			if (port.find_first_not_of("0123456789") != std::string::npos)
				return false;

			try {
				int port_int = std::stoi(port);
				return CheckPort(port_int);
			}
			catch (std::invalid_argument& e) {
				return false;
			}
			catch (std::out_of_range& e) {
				return false;
			}
			catch (...) {
				return false;
			}
		}
	}

	namespace KrilloudServer 
	{	
		typedef std::deque<KrillNetMessage> KrillMessages_queue;
		
		class KrillClient
		{
		public:
			virtual ~KrillClient() {}
			virtual void Deliver(SerializedMessage sMsg) = 0;
		};

		typedef std::shared_ptr<KrillClient> KrillClient_ptr;
		//----------------------------------------------------------------------

		class WorkGroup
		{
		public:
			void Join(KrillClient_ptr krillClient)
			{
				krillClients.insert(krillClient);

				//Send contract
				SerializedMessage message(KrilloudCommon::GetContractJSON(), SerializedMessage::Contract);
				krillClient->Deliver(message);

				//Send updated info of server ingame objects
				std::vector<SystemObjectInfo> objects_info;

				if (KrilloudEngine::listener_object != nullptr) 
				{
					SystemObjectInfo listener_info(*KrilloudEngine::listener_object, SystemObjectInfo::Listener);
					objects_info.push_back(listener_info);
				}

				for (size_t x = 0; x < KrilloudEngine::session_loaded_objects.size(); x++) 
				{
					IngameObject* object = KrilloudEngine::session_loaded_objects[x];

					if (object != nullptr) 
					{
						SystemObjectInfo object_info(*object, SystemObjectInfo::NewObject);
						objects_info.push_back(object_info);
					}
				}

				if (objects_info.size() > 0) 
				{
					SerializedMessage objects_info_message(objects_info[0]);

					for (size_t x = 1; x < objects_info.size(); x++) 
					{
						objects_info_message.AddMessage(objects_info[x]);
					}

					krillClient->Deliver(objects_info_message);
				}
				
				//Send updated info of server playing tags
				if (KrilloudEngine::playing_tags.size() > 0) 
				{
					std::vector<SystemObjectInfo> tags_info;

					for (size_t x = 0; x < KrilloudEngine::playing_tags.size(); x++) 
					{
						SystemObjectInfo playing_tag_info(*KrilloudEngine::playing_tags[x], SystemObjectInfo::NewPlayingTag);
						tags_info.push_back(playing_tag_info);
					}
					
					if (tags_info.size() > 0) 
					{
						SerializedMessage tags_info_message(tags_info[0]);

						for (size_t x = 1; x < tags_info.size(); x++)
						{
							tags_info_message.AddMessage(tags_info[x]);
						}

						krillClient->Deliver(tags_info_message);
					}
				}
			}

			void Leave(KrillClient_ptr krillClient)
			{
				krillClients.erase(krillClient);
			}

			//void Deliver(KrillClient_ptr krillClient)
			//{
			//	//for (int x = 0; x < boxes.size(); x++)
			//	//{
			//	//	SerializedMessage message(*(boxes[x]));

			//	//	for (auto krillClient : krillClients)
			//	//		krillClient->Deliver(message);

			//	//	//std::cout << "Message delivered to participant.\n";
			//	//}
			//}

			void DeliverToEveryone(SerializedMessage message) 
			{
				for (auto krillClient : krillClients) 
				{
					krillClient->Deliver(message);
				}	
			}

			void DeliverTo(SerializedMessage message, KrillClient_ptr target) 
			{
				target->Deliver(message);
			}

			void DeliverEveryoneBut(SerializedMessage message, KrillClient_ptr target)
			{
				for (auto krillClient : krillClients)
				{
					if (krillClient != target) 
					{
						krillClient->Deliver(message);
					}
				}
			}

		private:
			std::set<KrillClient_ptr> krillClients;
		};

		//----------------------------------------------------------------------

		class ClientSession
			: public KrillClient,
			public std::enable_shared_from_this<ClientSession>
		{
		public:
			ClientSession(tcp::socket socket, WorkGroup& group)
				: socket(std::move(socket)),
				workGroup(group)
			{
			}

			void Start()
			{
				workGroup.Join(shared_from_this());
				ReceiveNextMessage();
			}

			void Deliver(SerializedMessage sMsg)
			{
				std::string message = sMsg.Serialized();
				size_t mSize = message.size();
				while (mSize > 0)
				{
					std::string piece;
					if (mSize > KrillNetMessage::max_body_length)
					{
						piece = message.substr(0, KrillNetMessage::max_body_length);
						message = message.substr(KrillNetMessage::max_body_length);
					}
					else
					{
						piece = message;
						message = "";
					}
					mSize = message.size();
					const KrillNetMessage msg(piece);

					bool write_in_progress = !messages_queue.empty();
					messages_queue.push_back(msg);
					if (!write_in_progress)
					{
						SendNextMessage();
					}
				}
			}

		private:
			void ReceiveNextMessage()
			{
				auto self(shared_from_this());
				boost::asio::async_read(socket,
					boost::asio::buffer(incomingMessage.GetKrillData(), KrillNetMessage::header_length),
					[this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec && incomingMessage.DecodeHeader())
					{
						ReadMessageBody();
					}
					else
					{
						workGroup.Leave(shared_from_this());
					}
				});
			}

			void ReadMessageBody()
			{
				auto self(shared_from_this());
				boost::asio::async_read(socket,
					boost::asio::buffer(incomingMessage.GetKrillBody(), incomingMessage.GetKrillBodyLength()),
					[this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec)
					{
						std::stringstream ss;
						ss.write(incomingMessage.GetKrillBody(), incomingMessage.GetKrillBodyLength());
						std::string piece = ss.str();

						std::stringstream ss2;
						ss2 << receivedMessage << piece;
						receivedMessage = ss2.str();

						std::string end;
						std::string endcapsule = ENDCAPSULE;
						if (receivedMessage.size() > endcapsule.size())
						{
							end = receivedMessage.substr(receivedMessage.size() - endcapsule.size());
						}

						if (end == endcapsule)
						{
							SerializedMessage sm(receivedMessage);

							//sm.Deserialized(receivedMessage);

							if (sm.Ok())
							{
								SerializedMessage::MessageType type = sm.GetType();

								if (type == SerializedMessage::Command)
								{
									std::vector<std::string> smContent = sm.GetContent();

									for (int x = 0; x < smContent.size(); x++) 
									{
										KCommand newCommand;
										newCommand.Deserialized(smContent[x]);

										PerformCommand(newCommand);

										//HAY QUE ACTUALIZAR EL CONTRATO EN EL SERVER

										if (newCommand.command_type == KCommand::FilterParameter) 
										{
											workGroup.DeliverEveryoneBut(sm, self);
										}
									}
									/*KCommand newCommand;
									newCommand.Deserialized(sm.GetContent());

									int position = newCommand.targetId;*/

									//if (position < boxes.size())
									//{
									//	newCommand.PerformCommand(boxes[position]);
									//	//std::cout << "Command performed: " << newCommand.ReadMessage() << "\n";
									//}
									//else
									//{
									//	//std::cout << "Command not performed: (position > boxes.size())\n";
									//}
								}
								else if (type == SerializedMessage::Contract)
								{
									//Server shouldn't receive Contract type messages??
								}

								//ONLY WHEN THE USER CHANGE SOMETHING
								//AND THE OTHER CLIENTS ARE LOOKING AT THE SAME TAG
								//AND DON'T SEND TO THE SAME USER THAT MADE THE CHANGES
								//workGroup.Deliver(shared_from_this() /*read_msg_*/);
							}
							else
							{
								//std::cout << "sm not ok\n";
							}

							receivedMessage = "";
						}

						ReceiveNextMessage();
					}
					else
					{
						workGroup.Leave(shared_from_this());
					}
				});
			}

			void SendNextMessage()
			{
				auto self(shared_from_this());
				boost::asio::async_write(socket,
					boost::asio::buffer(messages_queue.front().GetKrillData(),
						messages_queue.front().GetKrillDataLength()),
					[this, self](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec)
					{
						messages_queue.pop_front();
						if (!messages_queue.empty())
						{
							SendNextMessage();
						}
					}
					else
					{
						workGroup.Leave(shared_from_this());
					}
				});
			}

			tcp::socket socket;
			WorkGroup& workGroup;
			KrillNetMessage incomingMessage;
			std::string receivedMessage;
			KrillMessages_queue messages_queue;
		};

		//----------------------------------------------------------------------

		class KrillServerConnector
		{
		public:
			KrillServerConnector(boost::asio::io_context& io_context,
				const tcp::endpoint& endpoint)
				: acceptor(io_context, endpoint)
			{
				AcceptConnections();
			}

			void DeliverEveryone(SerializedMessage sMsg)
			{
				workGroup.DeliverToEveryone(sMsg);
			}
			
		private:
			void AcceptConnections()
			{
				acceptor.async_accept(
					[this](boost::system::error_code ec, tcp::socket socket)
				{
					if (!ec)
					{
						std::make_shared<ClientSession>(std::move(socket), workGroup)->Start();
					}

					AcceptConnections();
				});
			}

			tcp::acceptor acceptor;
			WorkGroup workGroup;
		};

		//----------------------------------------------------------------------

		class KrillServer
		{
		
		public:
			KrillServer(std::string port)
				: endpoint(tcp::v4(), std::atoi(port.c_str())),
				connector(io_context, endpoint)
			{
				context_thread = new std::thread(Context_Runner_thread, &io_context);
			}

			~KrillServer()
			{
				io_context.stop();
				//context_thread->join();
			}

			void DeliverToAll(SerializedMessage sMsg) 
			{
				connector.DeliverEveryone(sMsg);
			}

			KrillServerConnector* GetServerPtr()
			{
				return &connector;
			}
		private:
			boost::asio::io_context io_context;
			tcp::endpoint endpoint;
			KrillServerConnector connector;
			std::thread* context_thread;

		};
	}

	namespace KrilloudClient 
	{
		typedef std::deque<KrillNetMessage> KrillMessages_queue;

		class KrillClientConnector
		{
		public:
			KrillClientConnector(boost::asio::io_context& io_context,
				const tcp::resolver::results_type& endpoints)
				: io_context(io_context),
				socket(io_context),
				deadline(io_context)
			{
				connected = false;
				connecting = false;
				endClient = false;
				endSession = false;

				Connect(endpoints);
			}

			void Deliver(SerializedMessage sMsg)
			{
				std::string message = sMsg.Serialized();
				size_t mSize = message.size();
				while (mSize > 0)
				{
					std::string piece;
					if (mSize > KrillNetMessage::max_body_length)
					{
						piece = message.substr(0, KrillNetMessage::max_body_length);
						message = message.substr(KrillNetMessage::max_body_length);
					}
					else
					{
						piece = message;
						message = "";
					}
					mSize = message.size();
					const KrillNetMessage msg(piece);

					boost::asio::post(io_context,
						[this, msg]()
					{
						bool write_in_progress = !messages_queue.empty();
						messages_queue.push_back(msg);
						if (!write_in_progress)
						{
							SendNextMessage();
						}
					});
				}


			}

			void Close()
			{
				boost::asio::post(io_context, [this]() { socket.close(); });
			}

			bool IsConnected()
			{
				return connected;
			}

			bool IsConnecting()
			{
				return connecting;
			}

			KrilloudProjectInfo GetContractData()
			{
				return contractData;
			}

		private:
			void Connect(const tcp::resolver::results_type& endpoints)
			{
				// Set a deadline for the connect operation.
				deadline.expires_after(std::chrono::seconds(5));

				boost::asio::async_connect(socket, endpoints,
					[this](boost::system::error_code ec, tcp::endpoint)
				{
					if (!ec)
					{
						//AddResultString("Do connect");
						connecting = false;
						connected = true;
						stopped_ = true;
						ReceiveNextMessage();
					}
				});


				deadline.async_wait(std::bind(&KrillClientConnector::CheckDeadline, this));
			}

			void CheckDeadline()
			{
				if (stopped_)
					return;

				// Check whether the deadline has passed. We compare the deadline against
				// the current time since a new asynchronous operation may have moved the
				// deadline before this actor had a chance to run.
				if (deadline.expiry() <= boost::asio::steady_timer::clock_type::now())
				{
					// The deadline has passed. The socket is closed so that any outstanding
					// asynchronous operations are cancelled.
					socket.close();
					connected = false;
					endSession = true;
					endClient = true;
					io_context.stop();

					// There is no longer an active deadline. The expiry is set to the
					// maximum time point so that the actor takes no action until a new
					// deadline is set.
					deadline.expires_at(boost::asio::steady_timer::time_point::max());
				}

				// Put the actor back to sleep.
				deadline.async_wait(std::bind(&KrillClientConnector::CheckDeadline, this));
			}

			void ReceiveNextMessage()
			{
				boost::asio::async_read(socket,
					boost::asio::buffer(incomingMessage.GetKrillData(), KrillNetMessage::header_length),
					[this](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec && incomingMessage.DecodeHeader())
					{
						ReadMessageBody();
					}
					else
					{
						connected = false;
						socket.close();
					}
				});
			}

			void ReadMessageBody()
			{
				boost::asio::async_read(socket,
					boost::asio::buffer(incomingMessage.GetKrillBody(), incomingMessage.GetKrillBodyLength()),
					[this](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec)
					{
						std::stringstream ss;
						ss.write(incomingMessage.GetKrillBody(), incomingMessage.GetKrillBodyLength());
						std::string piece = ss.str();

						std::stringstream ss2;
						ss2 << receivedMessage << piece;
						receivedMessage = ss2.str();

						std::string end;
						std::string endcapsule = ENDCAPSULE;
						if (receivedMessage.size() > endcapsule.size())
						{
							end = receivedMessage.substr(receivedMessage.size() - endcapsule.size());
						}

						if (end == endcapsule)
						{
							SerializedMessage sm(receivedMessage);

							//sm.Deserialized(receivedMessage);

							if (sm.Ok())
							{
								SerializedMessage::MessageType type = sm.GetType();

								if (type == SerializedMessage::Command)
								{
									std::vector<std::string> smContent = sm.GetContent();

									for (int x = 0; x < smContent.size(); x++)
									{
										KCommand newCommand;
										newCommand.Deserialized(smContent[x]);

										PerformCommand(newCommand);
									}
								}
								else if (type == SerializedMessage::Contract)
								{
									std::stringstream receivedStream;

									std::vector<std::string> smContent = sm.GetContent();
									if (smContent.size() == 1) 
									{
										receivedStream << smContent[0];
									}

									contractData = KrilloudCommon::GetContractFromJSON(&receivedStream);
								}
								else if (type == SerializedMessage::GameObjectInfo) 
								{
									std::vector<std::string> smContent = sm.GetContent();

									for (int x = 0; x < smContent.size(); x++)
									{
										SystemObjectInfo objectInfo;
										objectInfo.Deserialized(smContent[x]);

										RefreshGameObjectInfo(objectInfo);
									}
								}
							}
							else
							{
								//std::cout << "sm not ok\n";
							}

							receivedMessage = "";
						}

						ReceiveNextMessage();
					}
					else
					{
						connected = false;
						socket.close();
					}
				});
			}

			void SendNextMessage()
			{
				boost::asio::async_write(socket,
					boost::asio::buffer(messages_queue.front().GetKrillData(),
						messages_queue.front().GetKrillDataLength()),
					[this](boost::system::error_code ec, std::size_t /*length*/)
				{
					if (!ec)
					{
						messages_queue.pop_front();
						if (!messages_queue.empty())
						{
							SendNextMessage();
						}
					}
					else
					{
						connected = false;
						socket.close();
					}
				});
			}
		private:
			std::string receivedMessage;
			boost::asio::io_context& io_context;
			tcp::socket socket;
			KrillNetMessage incomingMessage;
			KrillMessages_queue messages_queue;

			bool stopped_ = false;
			boost::asio::steady_timer deadline;

			KrilloudProjectInfo contractData;

			bool connected = false;
			bool connecting = false;
			bool endClient = false;
			bool endSession = false;
		};

		class KrillClient
		{
		public:
			KrillClient(std::string ip, std::string port)
				: ip(ip),
				port(port),
				socket(io_context),
				resolver(io_context),
				endpoints(resolver.resolve(ip, port)),
				connector(io_context, endpoints)
			{
				context_thread = new std::thread(Context_Runner_thread, &io_context);
			}

			~KrillClient()
			{
				connector.Close();
				io_context.stop();
				//context_thread->join();
			}
			
			KrillClientConnector* GetClientPtr()
			{
				return &connector;
			}

		private:
			std::string ip;
			std::string port;
			boost::asio::io_context io_context;
			tcp::resolver resolver;
			boost::asio::ip::basic_resolver_results<boost::asio::ip::tcp> endpoints;
			tcp::socket socket;
			KrillClientConnector connector;
			std::thread* context_thread;
		};
	}

	KrilloudClient::KrillClient* kClient;
	KrilloudClient::KrillClientConnector* kClientConnector;

	KrilloudServer::KrillServer* kServer;
	
	int server_port = 25;
	bool server_on_play = false;

	// --- General --------------------------------------------------------------------------------------------------------------------------------------

	bool IsRealTimeOn() 
	{
		return (IsConectionActive() || IsServerActive());
	}

	// --- Server functions -----------------------------------------------------------------------------------------------------------------------------

	bool SetServerPort(int port) 
	{
		if (!KrilloudCommon::CheckPort(port))
			return false;
		
		server_port = port;
		return true;
	}

	int GetServerPort() 
	{
		return server_port;
	}

	std::pair<bool, const char*> StartServer()
	{
#ifndef __ANDROID__
		KrilloudDebug::DebugWrite("Server Start.");

		if (kServer == nullptr && !IsConectionActive())
		{
			kServer = new KrilloudServer::KrillServer(std::to_string(server_port));
			//krilloud_is_initiated = true;
			KrilloudDebug::DebugWrite("Krilloud server start. Everything ok.");
			return std::pair<bool, const char*>(true, "Everything ok");
		}

		std::stringstream ss;
		ss << (kServer != nullptr ? "kServer not null." : "kServer null.") << (IsConectionActive() ? "Connection active." : "Connection not active");

		KrilloudDebug::DebugWrite({ "Krilloud server start. Error.", ss.str() });

		return std::pair<bool, const char*>(false, ss.str().c_str());
#else
		return std::pair<bool, const char*>(false, "Server can't be used in this system.");
#endif
	}

	std::pair<bool, const char*> StopServer()
	{
		KrilloudDebug::DebugWrite("Server Stop.");

		if (IsServerActive()) 
		{
			//krilloud_is_initiated = false;
			kServer->~KrillServer();
			kServer = nullptr;

			KrilloudDebug::DebugWrite("Server stopped successfully.");

			return std::pair<bool, const char*>(true, "Everything ok" );
		}

		std::stringstream ss;
		ss << (kServer != nullptr ? "kServer not null." : "kServer null.") << (IsServerActive() ? "Server already active." : "Server not active yet");

		KrilloudDebug::DebugWrite({ "Krilloud server stop. Error.", ss.str() });

		return std::pair<bool, const char*>(false, ss.str().c_str());
	}

	bool IsServerActive()
	{
		return kServer != nullptr;
	}

	bool SetServerOnPlay(bool value)
	{
		server_on_play = value;

		std::string sdf = (server_on_play ? "True" : "False");

		KrilloudDebug::DebugWrite({ "Server automatic start set to ", sdf });

		return server_on_play;
	}

	bool IsServerSetOnPlay()
	{
		std::string sdf = (server_on_play ? "True" : "False");

		KrilloudDebug::DebugWrite({ "Server automatic start: ", sdf });

		return server_on_play;
	}

	// --- Server internal functions --------------------------------------------------------------------------------------------------------------------

	//INTERNAL
	void NotifyListener(IngameObject listener)
	{
		SystemObjectInfo newInfo(listener, SystemObjectInfo::Listener);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyNewObject(IngameObject object) 
	{
		SystemObjectInfo newInfo(object, SystemObjectInfo::NewObject);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyObjectRemove(IngameObject object)
	{
		SystemObjectInfo newInfo(object, SystemObjectInfo::RemoveObject);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyObjectChange(IngameObject object)
	{
		SystemObjectInfo newInfo(object, SystemObjectInfo::ChangeObject);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyNewPlayingTag(PlayingTag tag) 
	{
		SystemObjectInfo newInfo(tag, SystemObjectInfo::NewPlayingTag);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyPlayingTagRemove(PlayingTag tag) 
	{
		SystemObjectInfo newInfo(tag, SystemObjectInfo::RemovePlayingTag);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//INTERNAL
	void NotifyNewPlayingTagChange(PlayingTag tag) 
	{
		SystemObjectInfo newInfo(tag, SystemObjectInfo::ChangePlayingTag);

		SerializedMessage sMsg(newInfo);

		DeliverMessage(sMsg);
	}

	//Client basics
	void Connect(std::string ip, std::string port)
	{
#ifndef __ANDROID__
		if (!KrilloudCommon::CheckIP(ip) || !KrilloudCommon::CheckPort(port))
			return;

		if (kClient == nullptr && !IsServerActive()) 
		{
			kClient = new KrilloudRT::KrilloudClient::KrillClient(ip, port);
			kClientConnector = kClient->GetClientPtr();
			server_port = std::stoi(port);
		}
#endif
	}

	void Disconnect()
	{
		if (kClient != nullptr) 
		{
			kClient->~KrillClient();
			kClientConnector = nullptr;
			kClient = nullptr;
		}
	}

	bool IsConectionActive()
	{
		if (kClientConnector != nullptr)
		{
			if (kClientConnector->IsConnected() || kClientConnector->IsConnecting())
			{
				return true;
			}
			else
			{
				Disconnect();
				return false;
			}
		}
		return false;
	}

	//Client functions
	KrilloudProjectInfo GetContractData()
	{
		KrilloudProjectInfo result;
		if (kClientConnector != nullptr)
		{
			result = kClientConnector->GetContractData();
		}
		return result;
	}
	
	bool NotifyFilterParameter(int target_tag_index, int target_node_id, int parameter_index, float new_value)
	{
		if (target_tag_index >= KrilloudEngine::my_tags.size())
			return false;

		KCommand::CommandData cData;
		std::string tag_name = KrilloudEngine::my_tags[target_tag_index].tag.tag_name;

		cData.target_name = tag_name;
		cData.target_id = target_node_id;
		cData.element_index = parameter_index;
		
		cData.float_value = new_value;

		KCommand new_command(KCommand::FilterParameter, cData);

		SerializedMessage sMsg(new_command);

		return DeliverMessage(sMsg);
	}

	bool NotifyChannelName(int channel_id, std::string new_channel_name)
	{
		KCommand::CommandData cData;

		cData.string_value = new_channel_name;
		cData.target_id = channel_id;

		KCommand new_command(KCommand::Channel_Name, cData);

		SerializedMessage sMsg(new_command);

		return DeliverMessage(sMsg);
	}
	bool NotifyChannelParentID(int channel_id, int new_parent_id)
	{
		KCommand::CommandData cData;

		cData.int_value = new_parent_id;
		cData.target_id = channel_id;

		KCommand new_command(KCommand::Channel_Parent, cData);

		SerializedMessage sMsg(new_command);

		return DeliverMessage(sMsg);
	}
	bool NotifyChannelVolume(int channel_id, float new_volume)
	{
		KCommand::CommandData cData;

		cData.float_value = new_volume;
		cData.target_id = channel_id;

		KCommand new_command(KCommand::Channel_Volume, cData);

		SerializedMessage sMsg(new_command);

		return DeliverMessage(sMsg);
	}

	bool NotifyVariableName(int variable_position, std::string new_variable_name)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.string_value = new_variable_name;

		KCommand new_command(KCommand::Variable_Name, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableCurrentValue(int variable_position, float new_current_value)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.float_value = new_current_value;

		KCommand new_command(KCommand::Variable_CurrentValue, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableType(int variable_position, VarType new_variable_type)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.int_value = (int)new_variable_type;

		KCommand new_command(KCommand::Variable_Type, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableSource(int variable_position, VarSourceType new_source_type)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.int_value = (int)new_source_type;

		KCommand new_command(KCommand::Variable_Source, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableContent(int variable_position, VarContentType new_content_type)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.int_value = (int)new_content_type;

		KCommand new_command(KCommand::Variable_Content, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableRangeMin(int variable_position, float new_range_min)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.float_value = new_range_min;

		KCommand new_command(KCommand::Variable_MinRange, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableRangeMax(int variable_position, float new_range_max)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.float_value = new_range_max;

		KCommand new_command(KCommand::Variable_MaxRange, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}
	bool NotifyVariableOnlyOnPlay(int variable_position, bool new_set_only_on_play)
	{
		if (variable_position >= KrilloudEngine::session_variables.size())
			return false;

		KCommand::CommandData cData;
		cData.target_id = variable_position;

		cData.int_value = (new_set_only_on_play ? 1 : 0);

		KCommand new_command(KCommand::Variable_SetOnPlay, cData);
		SerializedMessage sMsg(new_command);
		return DeliverMessage(sMsg);
	}

	bool NotifyMixerVolumeChange(int target_tag_index, int target_node_id, int parameter_index, float new_volume)
	{
		if (target_tag_index >= KrilloudEngine::my_tags.size())
			return false;

		KCommand::CommandData cData;
		std::string tag_name = KrilloudEngine::my_tags[target_tag_index].tag.tag_name;

		cData.target_name = tag_name;
		cData.target_id = target_node_id;
		cData.element_index = parameter_index;

		cData.float_value = new_volume;

		KCommand new_command(KCommand::Mixer_VolumeChanged, cData);

		SerializedMessage sMsg(new_command);

		return DeliverMessage(sMsg);
	}
	//Helper functions
	

	

	// -- Internal functions
	NodeType GetNodeType(NodeClass source_class)
	{
		NodeType result = NodeType::Node_Normal;

		switch (source_class)
		{
		case Class_AudioContainer:
			result = NodeType::Node_Container;
			break;
		case Class_F_BandPass:
		case Class_F_Bassboost:
		case Class_F_BQR:
		case Class_F_DC_Remover:
		case Class_F_Delay:
		case Class_F_Echo:
		case Class_F_Flanger:
		case Class_F_HighPass:
		case Class_F_LoFi:
		case Class_F_LowPass:
		case Class_F_Pitch:
		case Class_F_Robotizer:
		case Class_F_WaveShaper:
			result = NodeType::Node_Filter;
			break;
		case Class_IngameVariable:
			result = NodeType::Node_Variable;
			break;
		case Class_Output:
			result = NodeType::Node_Output;
			break;
		case Class_Selector:
			result = NodeType::Node_Selector;
			break;
		case Class_Mixer:
			result = NodeType::Node_Mixer;
			break;
		default:
			result = NodeType::Node_Normal;
		}

		return result;
	}

	void PerformCommand(KCommand command) 
	{
		switch (command.command_type) 
		{
		case KCommand::FilterParameter:
		{
			for (size_t x = 0; x < KrilloudEngine::my_tags.size(); x++) 
			{
				int found = KrilloudEngine::my_tags[x].tag.tag_name.find(std::to_string(KrilloudEngine::my_tags[x].tag.owner_id));

				if (found > -1) 
				{
					std::string tag_name = KrilloudEngine::my_tags[x].tag.tag_name.substr(0, found);

					if (tag_name == command.command_data.target_name)
					{
						for (size_t y = 0; y < KrilloudEngine::my_tags[x].tag_content->nodes.size(); y++)
						{
							if (KrilloudEngine::my_tags[x].tag_content->nodes[y]->internal_id == command.command_data.target_id)
							{
								if (GetNodeType(KrilloudEngine::my_tags[x].tag_content->nodes[y]->node_class) == NodeType::Node_Filter)
								{
									((AudioEngineFilter*)KrilloudEngine::my_tags[x].tag_content->nodes[y])->SetParameter(command.command_data.element_index, command.command_data.float_value);

									for (size_t xx = 0; xx < KrilloudEngine::contract.tags.size(); xx++)
									{
										if (KrilloudEngine::contract.tags[xx].tagname == command.command_data.target_name)
										{
											KrilloudEngine::contract.tags[xx].graphNodes_info[y].values[command.command_data.element_index] = command.command_data.float_value;
											break;
										}
									}

								}
							}
						}
					}
				}
			}
		}
			break;
		case KCommand::Channel_Name:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::channels.size())
			{
				for (size_t x = 0; x < KrilloudEngine::channels.size(); x++) 
				{
					if (KrilloudEngine::channels[x]->channel_id == command.command_data.target_id) 
					{
						KrilloudEngine::channels[x]->channel_name = command.command_data.string_value;

						for (size_t y = 0; y < KrilloudEngine::contract.channels.size(); y++) 
						{
							if (KrilloudEngine::contract.channels[x].channel_id == command.command_data.target_id) 
							{
								KrilloudEngine::contract.channels[x].channel_name = command.command_data.string_value;
								break;
							}
						}

						break;
					}
				}
				/*KrilloudEngine::channels[command.command_data.target_id]->channel_name = command.command_data.string_value;

				KrilloudEngine::contract.channels[command.command_data.target_id].channel_name = command.command_data.string_value;*/
			}
			break;
		case KCommand::Channel_Parent:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::channels.size())
			{
				for (size_t x = 0; x < KrilloudEngine::channels.size(); x++)
				{
					if (KrilloudEngine::channels[x]->channel_id == command.command_data.target_id)
					{
						KrilloudEngine::channels[x]->parent_id = command.command_data.int_value;

						for (size_t y = 0; y < KrilloudEngine::contract.channels.size(); y++)
						{
							if (KrilloudEngine::contract.channels[x].channel_id == command.command_data.target_id)
							{
								KrilloudEngine::contract.channels[x].parent_id = command.command_data.int_value;
								break;
							}
						}

						break;
					}
				}
				/*KrilloudEngine::channels[command.command_data.target_id]->parent_id = command.command_data.int_value;

				KrilloudEngine::contract.channels[command.command_data.target_id].parent_id = command.command_data.int_value;*/
			}
			break;
		case KCommand::Channel_Volume:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::channels.size())
			{
				for (size_t x = 0; x < KrilloudEngine::channels.size(); x++)
				{
					if (KrilloudEngine::channels[x]->channel_id == command.command_data.target_id)
					{
						KrilloudEngine::channels[x]->volume = command.command_data.float_value;

						for (size_t y = 0; y < KrilloudEngine::contract.channels.size(); y++)
						{
							if (KrilloudEngine::contract.channels[x].channel_id == command.command_data.target_id)
							{
								KrilloudEngine::contract.channels[x].volume = command.command_data.float_value;
								break;
							}
						}

						break;
					}
				}
				/*KrilloudEngine::channels[command.command_data.target_id]->volume = command.command_data.float_value;

				KrilloudEngine::contract.channels[command.command_data.target_id].volume = command.command_data.float_value;*/
			}
			break;
		case KCommand::Variable_Content:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size()) 
			{
				VarContentType new_content_type = (VarContentType)command.command_data.int_value;

				KrilloudEngine::session_variables[command.command_data.target_id]->content = new_content_type;

				KrilloudEngine::contract.variables[command.command_data.target_id].content = new_content_type;
			}
			break;
		case KCommand::Variable_CurrentValue:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				KrilloudEngine::session_variables[command.command_data.target_id]->current_value = command.command_data.float_value;

				KrilloudEngine::contract.variables[command.command_data.target_id].current_value = command.command_data.float_value;
			}
			break;
		case KCommand::Variable_MaxRange:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				KrilloudEngine::session_variables[command.command_data.target_id]->range_max = command.command_data.float_value;

				KrilloudEngine::contract.variables[command.command_data.target_id].range_max = command.command_data.float_value;
			}
			break;
		case KCommand::Variable_MinRange:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				KrilloudEngine::session_variables[command.command_data.target_id]->range_min = command.command_data.float_value;

				KrilloudEngine::contract.variables[command.command_data.target_id].range_min = command.command_data.float_value;
			}
			break;
		case KCommand::Variable_Name:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				KrilloudEngine::session_variables[command.command_data.target_id]->variable_name = command.command_data.string_value;

				KrilloudEngine::contract.variables[command.command_data.target_id].variable_name = command.command_data.string_value;
			}
			break;
		case KCommand::Variable_SetOnPlay:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				bool new_set_on_play = (command.command_data.int_value == 1 ? true : false);

				KrilloudEngine::session_variables[command.command_data.target_id]->set_only_on_play = new_set_on_play;

				KrilloudEngine::contract.variables[command.command_data.target_id].set_only_on_play = new_set_on_play;
			}
			break;
		case KCommand::Variable_Source:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				VarSourceType new_source_type = (VarSourceType)command.command_data.int_value;

				KrilloudEngine::session_variables[command.command_data.target_id]->source = new_source_type;

				KrilloudEngine::contract.variables[command.command_data.target_id].source = new_source_type;
			}
			break;
		case KCommand::Variable_Type:
			if (command.command_data.target_id >= 0 && command.command_data.target_id < KrilloudEngine::session_variables.size())
			{
				VarType new_type = (VarType)command.command_data.int_value;

				KrilloudEngine::session_variables[command.command_data.target_id]->type = new_type;

				KrilloudEngine::contract.variables[command.command_data.target_id].type = new_type;
			}
			break;
		case KCommand::Mixer_VolumeChanged:
		{
			for (size_t x = 0; x < KrilloudEngine::my_tags.size(); x++)
			{
				int found = KrilloudEngine::my_tags[x].tag.tag_name.find(std::to_string(KrilloudEngine::my_tags[x].tag.owner_id));

				if (found > -1)
				{
					std::string tag_name = KrilloudEngine::my_tags[x].tag.tag_name.substr(0, found);

					if (tag_name == command.command_data.target_name)
					{
						for (size_t y = 0; y < KrilloudEngine::my_tags[x].tag_content->nodes.size(); y++)
						{
							if (KrilloudEngine::my_tags[x].tag_content->nodes[y]->internal_id == command.command_data.target_id)
							{
								if (GetNodeType(KrilloudEngine::my_tags[x].tag_content->nodes[y]->node_class) == NodeType::Node_Mixer)
								{
									//((AudioEngineFilter*)KrilloudEngine::my_tags[x].tag_content->nodes[y])->SetParameter(command.command_data.element_index, command.command_data.float_value);
									((KrilloudMixer*)KrilloudEngine::my_tags[x].tag_content->nodes[y])->SetVolume(command.command_data.element_index, command.command_data.float_value);

									for (size_t xx = 0; xx < KrilloudEngine::contract.tags.size(); xx++)
									{
										if (KrilloudEngine::contract.tags[xx].tagname == command.command_data.target_name)
										{
											KrilloudEngine::contract.tags[xx].graphNodes_info[y].values[command.command_data.element_index] = command.command_data.float_value;
											break;
										}
									}

								}
							}
						}
					}
				}
			}
		}
			break;
		}
	}

	void RefreshGameObjectInfo(SystemObjectInfo new_info) 
	{
		if (new_info.Ok()) 
		{
			switch (new_info.info_type)
			{
			case SystemObjectInfo::Listener: 
			{
				IngameObject object_info = new_info.GetResultObject();
				KrilloudEngine::server_listener_object = new IngameObject(object_info);
			}
				break;
			case SystemObjectInfo::NewObject:
			{
				IngameObject object_info = new_info.GetResultObject();
				KrilloudEngine::server_session_loaded_objects.push_back(new IngameObject(object_info));
			}
				break;
			case SystemObjectInfo::RemoveObject:
			{
				IngameObject object_info = new_info.GetResultObject();

				for (size_t x = 0; x < KrilloudEngine::server_session_loaded_objects.size(); x++)
				{
					if (KrilloudEngine::server_session_loaded_objects[x]->object_id == object_info.object_id)
					{
						KrilloudEngine::server_session_loaded_objects.erase(KrilloudEngine::server_session_loaded_objects.begin() + x);
						break;
					}
				}
			}
				break;
			case SystemObjectInfo::ChangeObject:
			{
				IngameObject object_info = new_info.GetResultObject();
				for (size_t x = 0; x < KrilloudEngine::server_session_loaded_objects.size(); x++)
				{
					if (KrilloudEngine::server_session_loaded_objects[x]->object_id == object_info.object_id)
					{
						KrilloudEngine::server_session_loaded_objects[x] = new IngameObject(object_info);
						break;
					}
				}
			}
				break;
			case SystemObjectInfo::NewPlayingTag:
			{
				PlayingTag playingtag_info = new_info.GetResultTag();
				KrilloudEngine::server_playing_tags.push_back(playingtag_info);
			}
				break;
			case SystemObjectInfo::RemovePlayingTag:
			{
				PlayingTag playingtag_info = new_info.GetResultTag();
				for (size_t x = 0; x < KrilloudEngine::server_playing_tags.size(); x++) 
				{
					if (KrilloudEngine::server_playing_tags[x].id == playingtag_info.id) 
					{
						KrilloudEngine::server_playing_tags.erase(KrilloudEngine::server_playing_tags.begin() + x);
						break;
					}
				}
			}
				break;
			case SystemObjectInfo::ChangePlayingTag:
			{
				PlayingTag playingtag_info = new_info.GetResultTag();
				for (size_t x = 0; x < KrilloudEngine::server_playing_tags.size(); x++)
				{
					if (KrilloudEngine::server_playing_tags[x].id == playingtag_info.id)
					{
						KrilloudEngine::server_playing_tags[x] = playingtag_info;
						break;
					}
				}
			}
				break;
			}
		}
	}

	bool DeliverMessage(SerializedMessage sMsg) 
	{
		if (kClientConnector != nullptr)
		{
			kClientConnector->Deliver(sMsg);
			return true;
		}
		else if (kServer != nullptr)
		{
			kServer->DeliverToAll(sMsg);
			return true;
		}
		return false;
	}
}