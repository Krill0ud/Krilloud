#include "soloud.h"
#include "Krill_Delay.h"

namespace SoLoud
{
	Krill_DelayInstance::Krill_DelayInstance(Krill_Delay *aParent)
	{
		mParent = aParent;
		mBuffer = 0;
		mBufferLength = 0;
		mOffset = 0;
		initParams(1);

	}

	void Krill_DelayInstance::filter(float *aBuffer, unsigned int aSamples, unsigned int aChannels, float aSamplerate, double aTime)
	{
		updateParams(aTime);

		if (mBuffer == 0)
		{
			mBufferLength = (int)ceil(mParent->mDelay*aSamplerate);
			mBuffer = new float[mBufferLength* aChannels];
			unsigned int i = 0;
			for (i = 0; i < mBufferLength* aChannels; i++) {
				mBuffer[i] = 0;
			}
		}
		float decay = mParent->mDecay;
		unsigned int i, j;
		int prevofs = (mOffset + mBufferLength - 1) % mBufferLength;

		for (i = 0; i < aSamples; i++)
		{
			for (j = 0; j < aChannels; j++)
			{
				int chofs = j * mBufferLength;
				int bchofs = j * aSamples;

				mBuffer[mOffset + chofs] = mParent->mFilter * mBuffer[prevofs + chofs] + (1 - mParent->mFilter) * mBuffer[mOffset + chofs];
				float n = aBuffer[i + bchofs] + mBuffer[mOffset + chofs] * decay;
				mBuffer[mOffset + chofs] = n;

				aBuffer[i + bchofs] += (n - aBuffer[i + bchofs]) * mParam[0];
			}
			prevofs = mOffset;
			mOffset = (mOffset + 1) % mBufferLength;
		}
	}

	Krill_DelayInstance::~Krill_DelayInstance()
	{
		delete[] mBuffer;
	}



	Krill_Delay::Krill_Delay()
	{

		mDelay = 0.01f;
		mDecay = 0.01f;
		mFilter = 0.01f;

	}
	result Krill_Delay::setParams(float aDelay, float aDecay, float aFilter)
	{
		if (aDelay <= 0 || aDecay <= 0 || aFilter <= 0 || aFilter >= 1) {
			return INVALID_PARAMETER;
		}
		mDelay = aDelay;
		mDecay = aDecay;
		mFilter = aFilter;

		return 0;
	}
	FilterInstance *Krill_Delay::createInstance()
	{
		return new Krill_DelayInstance(this);
	}

}