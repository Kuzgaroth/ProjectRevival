// Project Revival. All Rights Reserved


#include "PRSoundFunctionLibrary.h"
#include "Sound/SoundClass.h"

void UPRSoundFunctionLibrary::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (!SoundClass) return;

	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
}

void UPRSoundFunctionLibrary::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (!SoundClass) return;

	const auto MaxVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;

	SetSoundClassVolume(SoundClass, MaxVolume);
}
