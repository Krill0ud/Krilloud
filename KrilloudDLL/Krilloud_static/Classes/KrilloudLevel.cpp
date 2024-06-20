//#include "KrilloudLevel.h"
//#include "Output.h"
#include "KrilloudClasses.h"

TagInfo* KrilloudLevel::GetInfo()
{
	TagInfo* tagResult = new TagInfo();

	for (size_t x = 0; x < nodes.size(); x++) 
	{
		KrillOutput* output_node = dynamic_cast<KrillOutput*>(nodes[x]);

		if (output_node != nullptr) 
		{
			tagResult = output_node->GetInfo();
		}
	}

	return tagResult;
}