#pragma once
#include "FilterNode.h"
class Filter_Dc_Remover : public FilterNode
{
public:
	Filter_Dc_Remover(int id, KPair pos);
	~Filter_Dc_Remover();
};