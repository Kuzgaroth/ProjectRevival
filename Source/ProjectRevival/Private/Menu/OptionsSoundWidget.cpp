// Project Revival. All Rights Reserved


#include "Menu/OptionsSoundWidget.h"

#include "PRGameInstance.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/MenuLevelTheme.h"
#include "Menu/SaveGameClass.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


bool UOptionsSoundWidget::Initialize()
{
	const auto InitStatus = Super::Initialize();
	
	Slider_0->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnMasterVolumeChange);
	Slider->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnEffectsVolumeChange);
	Slider_1->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnMusicVolumeChange);
	Slider_2->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnVoiceVolumeChange);

	MyGameInstance = Cast<UPRGameInstance>
			(UGameplayStatics::GetGameInstance(GetWorld()));

	LoadSoundSettings();
	
	return InitStatus;
}


void UOptionsSoundWidget::SaveSoundSettings()
{
	MyGameInstance->SaveSoundData(Slider_0->Value, Slider->Value, Slider_1->Value, Slider_2->Value);
}

void UOptionsSoundWidget::LoadSoundSettings()
{
	Slider_0->Value = MyGameInstance->LoadSoundData()->SaveSlider_0Value;
	Slider->Value = MyGameInstance->LoadSoundData()->SaveSliderValue;
	Slider_1->Value = MyGameInstance->LoadSoundData()->SaveSlider_1Value;
	Slider_2->Value = MyGameInstance->LoadSoundData()->SaveSlider_2Value;
}


void UOptionsSoundWidget::OnMasterVolumeChange(float newValue)
{
	Slider_0->Value = newValue;
	SetVolume(newValue, "Master");
}

void UOptionsSoundWidget::OnEffectsVolumeChange(float newValue)
{
	Slider->Value = newValue;
	SetVolume(newValue, "Effects");
}

void UOptionsSoundWidget::OnMusicVolumeChange(float newValue)
{
	Slider_1->Value = newValue;
	SetVolume(newValue, "Music");
}


void UOptionsSoundWidget::OnVoiceVolumeChange(float newValue)
{
	Slider_2->Value = newValue;
	SetVolume(newValue, "Voice");
}

void UOptionsSoundWidget::SetVolume(float newValue, FString WhatSound)
{
	SaveSoundSettings();
	
	TSubclassOf<AMenuLevelTheme> ClassToFind = AMenuLevelTheme::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, OutActors);
	for (int EveryActor = 0; EveryActor < OutActors.Num(); EveryActor++)
	{
		Cast<AMenuLevelTheme>(OutActors[EveryActor])->ChangeVolume(newValue, WhatSound);
	}
}