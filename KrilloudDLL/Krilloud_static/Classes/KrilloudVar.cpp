//#include "KrilloudVar.h"
#include "KrilloudClasses.h"
#include <ctime>

KrilloudVar::KrilloudVar()
{
	srand(static_cast <unsigned> (time(0)));
}

KrilloudVar::~KrilloudVar()
{
}

void KrilloudVar::SetValue(float value) 
{
	if (myData != nullptr) 
	{
		myData->current_value = value;
	}
}
void KrilloudVar::SetName(std::string new_name)
{
	if (myData != nullptr)
	{
		myData->variable_name = new_name;
	}
}
void KrilloudVar::SetType(VarType new_type)
{
	if (myData != nullptr)
	{
		myData->type = new_type;
	}
}
void KrilloudVar::SetSource(VarSourceType new_source_type)
{
	if (myData != nullptr)
	{
		myData->source = new_source_type;
	}
}
void KrilloudVar::SetContent(VarContentType new_content_type)
{
	if (myData != nullptr)
	{
		myData->content = new_content_type;
	}
}
void KrilloudVar::SetRangeMin(float new_range_min)
{
	if (myData != nullptr)
	{
		myData->range_min = new_range_min;
	}
}
void KrilloudVar::SetRangeMax(float new_range_max)
{
	if (myData != nullptr)
	{
		myData->range_max = new_range_max;
	}
}
void KrilloudVar::SetOnPlay(bool new_set_on_play)
{
	if (myData != nullptr)
	{
		myData->set_only_on_play = new_set_on_play;
	}
}

void KrilloudVar::SetVariableData(VariableData* data)
{
	myData = data;
}

VariableData* KrilloudVar::GetVariableData()
{
	return myData;
}

float KrilloudVar::GetVariableValue()
{
	if (myData != nullptr) 
	{
		if (myData->set_only_on_play && alreadySet) 
		{
			return lastValue;
		}
		else
		{
			if (myData->source == InGame)
			{
				//Variable value is set ingame
				if ((myData->content == Ranged) && (myData->range_min < myData->range_max))
				{
					//Variable value is filtered whithin a range
					if (myData->current_value > myData->range_min)
					{
						if (myData->current_value < myData->range_max)
						{
							lastValue = myData->current_value;
						}
						else
						{
							lastValue = myData->range_max;
						}
					}
					else
					{
						lastValue = myData->range_min;
					}
				}
				else
				{
					//Variable value is raw and we get its current real value
					lastValue = myData->current_value;
				}
			}
			else
			{
				if (myData->source == Random_Repetition)
				{
					//Get a random value. Repetition allowed.
					float r = myData->range_min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (myData->range_max - myData->range_min + 1)));
					lastValue = r;
				}
				else
				{
					//Get a random value. Repetition not allowed.
					size_t soundsLeft = (abs(myData->range_max - myData->range_min)) - played_values.size();
					int r = rand() / (RAND_MAX / (soundsLeft + 1));

					for (size_t x = 0; x <= myData->range_max; x++)
					{
						bool r_already_played = false;

						for (size_t y = 0; y < played_values.size(); y++)
						{
							if (played_values[y] == x)
							{
								r_already_played = true;
								break;
							}
						}

						if (!r_already_played) 
						{
							if (r <= 0)
							{
								played_values.push_back(x);

								lastValue = (float)x;

								break;
							}
							else
							{
								r--;
							}
						}
					}

					if (soundsLeft <= 1) 
					{
						played_values.clear();
					}

					//if (played_values.size() < (abs(myData->range_max - myData->range_min)))
					//{
					//	//While there's more than 1 sound left to be played, do this.
					//	bool r_already_played;
					//	int r = 0;

					//	do
					//	{
					//		r_already_played = false;
					//		r = myData->range_min + rand() / (RAND_MAX / (myData->range_max - myData->range_min + 1));

					//		for (size_t x = 0; x < played_values.size(); x++)
					//		{
					//			if (played_values[x] == r)
					//			{
					//				r_already_played = true;
					//			}
					//		}
					//	} while (r_already_played); //REVISION: POSSIBLE DEADLOCK

					//	played_values.push_back(r);

					//	lastValue = (float)r;
					//}
					//else
					//{
					//	//There's 1 sound left to be played. 
					//	//Find it, select it and clear all previous sounds so they can be played again in a new loop.
					//	int last_value = 0;
					//	int range = (abs(myData->range_max - myData->range_min + 1));

					//	for (int x = 0; x < range; x++)
					//	{
					//		bool found = false;
					//		for (size_t y = 0; y < played_values.size(); y++)
					//		{
					//			if (played_values[y] == x)
					//			{
					//				found = true;
					//				break;
					//			}
					//		}

					//		if (!found)
					//		{
					//			last_value = x;
					//			break;
					//		}
					//	}

					//	played_values.clear();

					//	lastValue = last_value;
					//}
				}
			}
		}
	}
	
	alreadySet = true;
	return lastValue;
}

void KrilloudVar::Reset() 
{
	alreadySet = false;
}

TagInfo * KrilloudVar::GetInfo()
{
	return nullptr;
}
