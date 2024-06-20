#pragma once

#include "soloud.h"
#include "soloud_fftfilter.h"
namespace SoLoud {


	class Krill_FFT_Pitch;
	class Krill_FFT_PitchInstance : public FFTFilterInstance
	{
		enum FILTERATTRIBUTE
		{
			WET = 0,
			PITCH = 1
		};
		Krill_FFT_Pitch *mParent;
		public:
			virtual void fftFilterChannel(float *aFFTBuffer, unsigned int aSamples, float aSamplerate, time aTime, unsigned int aChannel, unsigned int aChannels);
			Krill_FFT_PitchInstance(Krill_FFT_Pitch *aParent);

	};
	
	
	class Krill_FFT_Pitch : public FFTFilter
	{
		
			//~Krill_FFT_Pitch();
	public:
		enum FILTERATTRIBUTE
		{
			WET = 0,
			PITCH = 1
		};
		float mPitch;
		
		result setParams(float aPitch);
		virtual FilterInstance *createInstance();
		Krill_FFT_Pitch();

	};
}
