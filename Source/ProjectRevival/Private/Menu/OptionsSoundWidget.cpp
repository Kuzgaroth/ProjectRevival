// Project Revival. All Rights Reserved


#include "Menu/OptionsSoundWidget.h"

#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/MenuLevelTheme.h"


bool UOptionsSoundWidget::Initialize()
{
	const auto InitStatus = Super::Initialize();

	
	Slider_0->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnMasterVolumeChange);
	Slider->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnEffectsVolumeChange);
	Slider_1->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnMusicVolumeChange);
	Slider_2->OnValueChanged.AddDynamic(this, &UOptionsSoundWidget::OnVoiceVolumeChange);

	return InitStatus;
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
	TSubclassOf<AMenuLevelTheme> ClassToFind = AMenuLevelTheme::StaticClass();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(this, ClassToFind, OutActors);
	for (int EveryActor = 0; EveryActor < OutActors.Num(); EveryActor++)
	{
		Cast<AMenuLevelTheme>(OutActors[EveryActor])->ChangeVolume(newValue, WhatSound);
	}
}

