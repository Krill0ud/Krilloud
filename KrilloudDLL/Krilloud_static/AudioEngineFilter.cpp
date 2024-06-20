#include "AudioEngineFilter.h"

int AudioEngineFilter::AddParameter(float value)
{
	parameters.push_back(value);
	return (parameters.size() - 1);
}

bool AudioEngineFilter::RemoveParameter(int position)
{
	if (position < (int)parameters.size())
	{
		parameters.erase(parameters.begin() + position);
		return true;
	}
	return false;
}

bool AudioEngineFilter::SetParameter(int position, float parameter_value)
{
	if (position < (int)parameters.size())
	{
		parameters[position] = parameter_value;
		return true;
	}
	return false;
}

bool AudioEngineFilter::SetParameters(std::vector<float> new_parameters)
{
	parameters.clear();

	for (size_t x = 0; x < new_parameters.size(); x++)
	{
		parameters.push_back(new_parameters[x]);
	}

	return true;
}

std::vector<float> AudioEngineFilter::GetParameters()
{
	return parameters;
}