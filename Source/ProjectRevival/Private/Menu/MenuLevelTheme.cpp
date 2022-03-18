// Project Revival. All Rights Reserved


#include "Menu/MenuLevelTheme.h"

#include "Components/AudioComponent.h"
#include "Components/Slider.h"
#include "Menu/OptionsSoundWidget.h"

// Sets default values
AMenuLevelTheme::AMenuLevelTheme()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SongComponent = CreateDefaultSubobject<UAudioComponent>("SongComponent");
	SongComponent->bAutoActivate = false;
	SongComponent->bAlwaysPlay = true;
}

// Called when the game starts or when spawned
void AMenuLevelTheme::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *SongComponent->Sound->SoundClassObject->GetName());

	float startTime = 9.f;
	float volume = 1.0f;
	float fadeTime = 1.0f;
	SongComponent->FadeIn(fadeTime, volume, startTime);
	
	SongComponent->Play();
}

// Called every frame
void AMenuLevelTheme::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AMenuLevelTheme::ChangeVolume(float Value, FString WhatSound)
{
	if (SongComponent) {
		if (WhatSound == "Master") MasterVolume = Value;
		if (WhatSound == "Effects" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Effects" || WhatSound == "Music" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Music" || WhatSound == "Voice" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Voice")
			MyVolume = Value;
		if (Value > 0)
		{
			SongComponent->SetVolumeMultiplier(MasterVolume * MyVolume);
		}
		else
		{
			if (WhatSound == "Effects" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Effects" || WhatSound == "Music" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Music" || WhatSound == "Voice" && SongComponent->Sound->SoundClassObject->GetName() == "SC_Voice" || WhatSound == "Master")
			SongComponent->SetVolumeMultiplier(0.0001);
		}
	}
}

