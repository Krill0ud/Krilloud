//#include "KrilloudClass.h"
#include "KrilloudClasses.h"

int KrilloudClass::CreateNewSocket(int ID, SocketType type, SocketDataType data) 
{
	KLink new_link;
	new_link.SocketID = ID;
	
	new_link.connection_type = type;
	new_link.data_type = data;

	myKLinks.push_back(new_link);

	return ID;
}

bool KrilloudClass::RemoveSocket(int id) 
{
	if (id < (int)myKLinks.size()) 
	{
		myKLinks.erase(myKLinks.begin() + id);
		return true;
	}
	return false;
}

bool KrilloudClass::AddLink(int socketID, KrilloudClass* class_to_link)
{
	if (socketID < (int)myKLinks.size())
	{
		for (size_t x = 0; x < myKLinks.size(); x++) 
		{
			if (myKLinks[x].SocketID == socketID) 
			{
				if (std::find(myKLinks[x].connections.begin(), myKLinks[x].connections.end(), class_to_link) == myKLinks[x].connections.end())
				{
					myKLinks[x].connections.push_back(class_to_link);
					return true;
				}
			}
		}
		
	}

	return false;
}

bool KrilloudClass::RemoveLink(int socketID, KrilloudClass* class_to_unlink)
{
	if (socketID < (int)myKLinks.size())
	{
		int index = 0;
		bool found = false;

		while (!found && index < (int)myKLinks[socketID].connections.size()) 
		{
			if (myKLinks[socketID].connections[index] == class_to_unlink)
			{
				found = true;
			}
			else
			{
				index++;
			}
			
		}

		if (found) 
		{
			myKLinks[socketID].connections.erase(myKLinks[socketID].connections.begin() + index);
			return true;
		}
	}

	return false;
}

// -- Virtual functions
//float KrilloudClass::GetVariableValue() 
//{
//	return 0.0f;
//}

TagInfo* KrilloudClass::GetInfo()
{
	return nullptr;
}