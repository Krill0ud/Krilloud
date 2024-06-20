#include "Krill_FFT_Pitch.h"
#include <string.h>
#include "soloud.h"

namespace SoLoud{

Krill_FFT_PitchInstance::Krill_FFT_PitchInstance(Krill_FFT_Pitch *aParent)
{
	mParent = aParent;
	initParams(2);
	mParam[PITCH] = aParent->mPitch;
}

void Krill_FFT_PitchInstance::fftFilterChannel(float *aFFTBuffer, unsigned int aSamples, float aSamplerate, time aTime, unsigned int aChannel, unsigned int aChannels)
{
	updateParams(aTime);
	
}
result Krill_FFT_Pitch::setParams(float aPitch)
{
	if (aPitch < 0 || aPitch > 10 )
		return INVALID_PARAMETER;
	mPitch = aPitch;
	return SO_NO_ERROR;
}





Krill_FFT_Pitch::Krill_FFT_Pitch()
{
}

FilterInstance *Krill_FFT_Pitch::createInstance()
{
	return new Krill_FFT_PitchInstance(this);
}
}