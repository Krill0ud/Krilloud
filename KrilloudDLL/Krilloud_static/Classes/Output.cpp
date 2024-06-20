//#include "Output.h"
#include "KrilloudClasses.h"
KrillOutput::KrillOutput()
{
}

KrillOutput::~KrillOutput()
{
}

TagInfo* KrillOutput::GetInfo()
{
	if (myKLinks.size() > 0 && myKLinks[0].connections.size() > 0) 
	{
		//Outputs only have one input socket
		if (myKLinks[0].connections[0] != nullptr)
		{
			//Get info from the only input, if there's anything connected to it
			return myKLinks[0].connections[0]->GetInfo();
		}
	}

	return nullptr;
}