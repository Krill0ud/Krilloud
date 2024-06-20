#pragma once
#include "FilterNode.h"
class Filter_Robotizer : public FilterNode
{
public:
	Filter_Robotizer(int id, KPair pos);
	~Filter_Robotizer();
};