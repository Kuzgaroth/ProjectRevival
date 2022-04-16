// Project Revival. All Rights Reserved


#include "PRGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Menu/SaveGameClass.h"


void UPRGameInstance::Init()
{
	// check if there is game data to load it
	// or create a new one if the game is just starting
	SoundData = Cast<USaveGameClass>(UGameplayStatics::LoadGameFromSlot(SaveSlot, 0));

	if(!SoundData)
	{
		// create a new game data
		SoundData = Cast<USaveGameClass>(UGameplayStatics::
			CreateSaveGameObject(USaveGameClass::StaticClass()));

		SoundData->SaveSlider_0Value = 1.0;
		SoundData->SaveSliderValue = 1.0;
		SoundData->SaveSlider_1Value = 1.0;
		SoundData->SaveSlider_2Value = 1.0;

		UGameplayStatics::SaveGameToSlot(SoundData, SaveSlot, 0);
	}

}

void UPRGameInstance::SaveSoundData(float SaveSlider_0Value, float SaveSliderValue, float SaveSlider_1Value, float SaveSlider_2Value)
{
	if (SoundData)
	{
		SoundData->SaveSlider_0Value = SaveSlider_0Value;
		SoundData->SaveSliderValue = SaveSliderValue;
		SoundData->SaveSlider_1Value = SaveSlider_1Value;
		SoundData->SaveSlider_2Value = SaveSlider_2Value;

		UGameplayStatics::SaveGameToSlot(SoundData, SaveSlot, 0);
	}
}

USaveGameClass* UPRGameInstance::LoadSoundData()
{
	if (SoundData)
		return SoundData;
	return nullptr;
}
