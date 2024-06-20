#pragma once
#include <FilterNode.h>
class Filter_LowPass : public FilterNode
{
public:
	Filter_LowPass(int id, KPair pos);
	~Filter_LowPass();
};

