#include "SoloudFilter.h"
#include "SoloudAPI.h"
#include "soloud_filter.h"
#include "soloud_bassboostfilter.h"
#include "soloud_biquadresonantfilter.h"
#include "soloud_dcremovalfilter.h"
#include "Krill_Delay.h"
#include "soloud_echofilter.h"
#include "soloud_flangerfilter.h"
#include "soloud_lofifilter.h"
#include "Krill_FFT_Pitch.h"
#include "soloud_robotizefilter.h"
#include "soloud_waveshaperfilter.h"
#include "soloud_freeverbfilter.h"

SoloudFilter::SoloudFilter(std::vector<float> initial_parameters, KrilloudFilterType filter_type, std::shared_ptr<SoLoud::Soloud> engine)
{
	soloud_instance = engine;
	SetParameters(initial_parameters);
	this_filter_type = filter_type;

	switch (this_filter_type)
	{
	case KrilloudFilterType::BandPass:
		this_filter = new SoLoud::BiquadResonantFilter();
		((SoLoud::BiquadResonantFilter*)this_filter)->setParams(2, parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::BassBoost:
		this_filter = new SoLoud::BassboostFilter();
		((SoLoud::BassboostFilter*)this_filter)->setParams(parameters[1]);
		break;
	case KrilloudFilterType::BQR:
		this_filter = new SoLoud::BiquadResonantFilter();
		((SoLoud::BiquadResonantFilter*)this_filter)->setParams((int)parameters[3], parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::DC_Remover:
		this_filter = new SoLoud::DCRemovalFilter();
		((SoLoud::DCRemovalFilter*)this_filter)->setParams(parameters[0]);
		break;
	case KrilloudFilterType::Delay:
		this_filter = new SoLoud::Krill_Delay();
		((SoLoud::Krill_Delay*)this_filter)->setParams(parameters[1], parameters[2], parameters[3]);
		break;
	case KrilloudFilterType::Echo:
		this_filter = new SoLoud::EchoFilter();
		((SoLoud::EchoFilter*)this_filter)->setParams(parameters[1], parameters[2], parameters[3]);
		break;
	case KrilloudFilterType::Flanger:
		this_filter = new SoLoud::FlangerFilter();
		((SoLoud::FlangerFilter*)this_filter)->setParams(parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::HighPass:
		this_filter = new SoLoud::BiquadResonantFilter();
		((SoLoud::BiquadResonantFilter*)this_filter)->setParams(1, parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::LoFi:
		this_filter = new SoLoud::LofiFilter();
		((SoLoud::LofiFilter*)this_filter)->setParams(parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::LowPass:
		this_filter = new SoLoud::BiquadResonantFilter();
		((SoLoud::BiquadResonantFilter*)this_filter)->setParams(0, parameters[1], parameters[2]);
		break;
	case KrilloudFilterType::Pitch:
		this_filter = new SoLoud::Krill_FFT_Pitch();
		((SoLoud::Krill_FFT_Pitch*)this_filter)->setParams(0);
		break;
	case KrilloudFilterType::Robotizer:
		this_filter = new SoLoud::RobotizeFilter();
		break;
	case KrilloudFilterType::WaveShaper:
		this_filter = new SoLoud::WaveShaperFilter();
		((SoLoud::WaveShaperFilter*)this_filter)->setParams(parameters[1]);
		break;
	case KrilloudFilterType::Freeverb:
		this_filter = new SoLoud::FreeverbFilter();
		((SoLoud::FreeverbFilter*)this_filter)->setParams(0, parameters[1], parameters[2], parameters[3]);
		break;
	default:
		init_error = true;
		break;
	}
}

SoloudFilter::~SoloudFilter()
{
}

SoLoud::Filter * SoloudFilter::GetFilter()
{
	return this_filter;
}

void SoloudFilter::UpdatePlayingValues(AudioEngine* engine, int gBusHandle)
{
	int variable_index = -1;

	for (size_t x = 0; x < myKLinks.size(); x++)
	{
		if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Variable)
		{
			variable_index++;
			if (myKLinks[x].connections.size() > 0)
			{
				KrilloudVar* variable_node = dynamic_cast<KrilloudVar*>(myKLinks[x].connections[0]);

				if (variable_node != nullptr)
				{
					//Update parameters from connected variables
					parameters[variable_index] = variable_node->GetVariableValue();

					//Flanger parameters need to be greater than 0
					if (this_filter_type == KrilloudFilterType::Flanger && variable_index > 0)
					{
						if (parameters[variable_index] <= 0)
						{
							parameters[variable_index] = 0.01f;
						}
					}
				}
			}
		}
	}

	// Temporary - Pitch update
	if (this_filter_type == KrilloudFilterType::Pitch)
	{
		int voice = soloud_instance->getVoiceFromHandle_internal(gBusHandle);
		
		if (voice > -1 && soloud_instance->mVoice[voice] != NULL)
		{
			if (soloud_instance->mVoice[voice]->mFlags & 8)
			{
				//3D
				soloud_instance->mVoice[voice]->mBaseSamplerate = 44100 * std::pow(2, (parameters[0] / 12));
			}
			else
			{
				//2D
				soloud_instance->mVoice[voice]->mSamplerate = 44100 * std::pow(2, (parameters[0] / 12));
			}
		}
	}
	else
	{
		//Filter parameter update
		for (size_t x = 0; x < parameters.size(); x++)
		{
			engine->SetFilterParameter(gBusHandle, my_instance_id, x, parameters[x]);
		}
	}
}

TagInfo* SoloudFilter::GetInfo()
{
	if (!init_error && myKLinks.size() > 0 && myKLinks[0].connections.size() > 0)
	{
		if (myKLinks[0].connections[0] != nullptr)
		{
			//Get info structure from elements preceding this one
			TagInfo* tagInfo = myKLinks[0].connections[0]->GetInfo();

			if (tagInfo != nullptr)
			{
				my_instance_id = tagInfo->source->filters.size();

				//Soloud doesn't admit more than 8 filters in a single wave
				if (my_instance_id < 8)
				{
					//Add filter
					//tagInfo->wave->setFilter(my_instance_id, this_filter);

					///
					tagInfo->source->SetFilter(my_instance_id, this);
					tagInfo->source->filters.push_back(this);
					///

					//Add variables
					for (size_t x = 0; x < myKLinks.size(); x++)
					{
						if (myKLinks[x].connection_type == Input && myKLinks[x].data_type == Variable)
						{
							if (myKLinks[x].connections.size() > 0)
							{
								KrilloudVar* variable_node = dynamic_cast<KrilloudVar*>(myKLinks[x].connections[0]);

								if (variable_node != nullptr)
								{
									tagInfo->kvars.push_back(variable_node);
								}
							}
						}
					}
				}

				return tagInfo;
			}
		}
	}

	return nullptr;
}