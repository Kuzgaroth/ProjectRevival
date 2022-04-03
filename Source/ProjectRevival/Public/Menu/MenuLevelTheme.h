// Project Revival. All Rights Reserved

#pragma once

#include <string>
#include "Engine/Classes/Sound/SoundMix.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "MenuLevelTheme.generated.h"

class USoundCue;

UCLASS()
class PROJECTREVIVAL_API AMenuLevelTheme : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuLevelTheme();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	float MasterVolume = 0.0;

	UPROPERTY()
	float MyVolume = 0.0;

	UPROPERTY()
	FString WhoAmI = "";

	UFUNCTION()
	void ChangeVolume(float Value, FString WhatSound);

	UPROPERTY()
	class UPRGameInstance* MyGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* EffectsSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MusicSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* VoiceSoundMix;

	/*StrangeThings*/

};
