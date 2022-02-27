// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	UAudioComponent* SongComponent;

	UPROPERTY()
	float MasterVolume = 0.0;

	UPROPERTY()
	float MyVolume = 0.0;

	UFUNCTION()
	void ChangeVolume(float Value, FString WhatSound);

};
