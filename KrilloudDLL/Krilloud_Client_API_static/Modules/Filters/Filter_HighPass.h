#pragma once
#include <FilterNode.h>
class Filter_HighPass : public FilterNode
{
public:
	Filter_HighPass(int id, KPair pos);
	~Filter_HighPass();
};

