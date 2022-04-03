// Project Revival. All Rights Reserved


#include "Menu/MenuLevelTheme.h"

#include "PRGameInstance.h"
#include "SampleBuffer.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Menu/OptionsSoundWidget.h"
#include "Menu/SaveGameClass.h"
#include "Sound/SoundCue.h"

// Sets default values
AMenuLevelTheme::AMenuLevelTheme()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMenuLevelTheme::BeginPlay()
{
	Super::BeginPlay();

	MasterVolume = MainSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume;
	//UE_LOG(LogTemp, Warning, TEXT("The integer value is: %d"), SoundMix->SoundClassEffects.Num());

	MyGameInstance = Cast<UPRGameInstance>
			(UGameplayStatics::GetGameInstance(GetWorld()));
	
	ChangeVolume(MyGameInstance->LoadSoundData()->SaveSlider_0Value, "Master");
	ChangeVolume(MyGameInstance->LoadSoundData()->SaveSliderValue, "Effects");
	ChangeVolume(MyGameInstance->LoadSoundData()->SaveSlider_1Value, "Music");
	ChangeVolume(MyGameInstance->LoadSoundData()->SaveSlider_2Value, "Voice");
}

// Called every frame
void AMenuLevelTheme::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMenuLevelTheme::ChangeVolume(float Value, FString WhatSound)
{
	if (Value > 0)
	{
		if (WhatSound == "Master") MainSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = Value, MasterVolume = Value;
		if (WhatSound == "Effects") EffectsSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = Value * MasterVolume;
		if (WhatSound == "Music") MusicSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = Value * MasterVolume;
		if (WhatSound == "Voice") VoiceSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = Value * MasterVolume;
	}
	else
	{
		if (WhatSound == "Master") MainSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = 0.0001;
		if (WhatSound == "Effects") EffectsSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = 0.0001;
		if (WhatSound == "Music") MusicSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = 0.0001;
		if (WhatSound == "Voice") VoiceSoundMix->SoundClassEffects[0].SoundClassObject->Properties.Volume = 0.0001;
	}
}

