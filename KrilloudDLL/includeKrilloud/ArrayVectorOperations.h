#pragma once
#ifndef ARRAYVECTOR_OPERATIONS_H
#define ARRAYVECTOR_OPERATIONS_H

struct KrilloudNodeSocket;
struct KrilloudNodeConnection;

namespace ArrayVectorOperations 
{
	inline void CopyKrilloudNodeConnectionArray(KrilloudNodeConnection* origin, KrilloudNodeConnection** result, int length)
	{
		*result = new KrilloudNodeConnection[length];

		for (int x = 0; x < length; x++)
		{
			(*result)[x] = KrilloudNodeConnection(origin[x]);
		}
	}

	inline std::vector<KrilloudNodeSocket> GetKrilloudNodeSocketVector(KrilloudNodeSocket* origin_array, int length)
	{
		std::vector<KrilloudNodeSocket> result;

		for (int x = 0; x < length; x++)
		{
			KrilloudNodeSocket new_socket;

			new_socket.connection_type = origin_array[x].connection_type;
			new_socket.data_type = origin_array[x].data_type;
			new_socket.socket_id = origin_array[x].socket_id;
			new_socket.links_array_length = origin_array[x].links_array_length;

			//new_socket.links = origin_array[x]->links;

			CopyKrilloudNodeConnectionArray(origin_array[x].links, &new_socket.links, new_socket.links_array_length);

			result.push_back(new_socket);
		}

		return result;
	}

	inline std::vector<KrilloudNodeConnection> GetKrilloudNodeConnectionVector(KrilloudNodeConnection* origin_array, int length)
	{
		std::vector<KrilloudNodeConnection> result;

		for (int x = 0; x < length; x++)
		{
			KrilloudNodeConnection new_connection(origin_array[x]);
			result.push_back(new_connection);
		}

		return result;
	}

	inline void GetKrilloudNodeConnectionArray(std::vector<KrilloudNodeConnection> origin_vector, KrilloudNodeConnection** result, int length)
	{
		*result = new KrilloudNodeConnection[origin_vector.size()];

		for (size_t x = 0; x < origin_vector.size(); x++)
		{
			(*result)[x] = KrilloudNodeConnection(origin_vector[x]);
		}
	}

	inline void GetKrilloudNodeSocketArray(std::vector<KrilloudNodeSocket> origin_vector, KrilloudNodeSocket** result, int length)
	{
		*result = new KrilloudNodeSocket[length];

		for (size_t x = 0; x < origin_vector.size(); x++)
		{
			(*result)[x] = KrilloudNodeSocket();

			(*result)[x].connection_type = origin_vector[x].connection_type;
			(*result)[x].data_type = origin_vector[x].data_type;
			(*result)[x].socket_id = origin_vector[x].socket_id;
			(*result)[x].links_array_length = origin_vector[x].links_array_length;
			
			CopyKrilloudNodeConnectionArray(origin_vector[x].links, &((*result)[x].links), (*result)[x].links_array_length);
		}
	}
}
#endif //ARRAYVECTOR_OPERATIONS_H