//#include "Macro.h"
//#include "Output.h"
#include "KrilloudClasses.h"

KrilloudMacro::KrilloudMacro()
{
}

KrilloudMacro::~KrilloudMacro()
{
}

void KrilloudMacro::SetLevel(KLevel* level) 
{
	myLevel = level;
}

KLevel* KrilloudMacro::GetLevel() 
{
	return myLevel;
}

TagInfo* KrilloudMacro::GetInfo()
{
	if (myLevel != nullptr) 
	{
		for (size_t x = 0; x < myLevel->nodes.size(); x++)
		{
			KrillOutput* output_node = dynamic_cast<KrillOutput*>(myLevel->nodes[x]);

			if (output_node != nullptr)
			{
				return output_node->GetInfo();
			}
		}
	}

	return nullptr;
}