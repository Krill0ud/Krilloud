#pragma once

#ifndef KRILLDELAY_H
#define KRILLDELAY_H


#include "soloud.h"
namespace SoLoud
{
	class Krill_Delay;

	class Krill_DelayInstance : public FilterInstance
	{
		float *mBuffer;
		int mBufferLength;
		Krill_Delay *mParent;
		int mOffset;

	public:
		virtual void filter(float *aBuffer, unsigned int aSamples, unsigned int aChannels, float aSamplerate, time aTime);
		virtual ~Krill_DelayInstance();
		Krill_DelayInstance(Krill_Delay *aParent);
	};

	class Krill_Delay : public Filter
	{
	public:
		float mDelay;
		float mDecay;
		float mFilter;
		virtual FilterInstance *createInstance();
		Krill_Delay();
		result setParams(float aDelay, float aDecay, float aFilter);

	};
}
#endif
