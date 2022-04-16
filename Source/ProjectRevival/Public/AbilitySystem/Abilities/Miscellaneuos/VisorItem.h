// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/Abilities/VisorPlacerAbility.h"
#include "TimerManager.h"
#include "VisorItem.generated.h"

class USphereComponent;

UCLASS()
class PROJECTREVIVAL_API AVisorItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVisorItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool IsVisoring = false;
	
	// Array of objects/enemies to visor
	UPROPERTY()
	TArray<AActor*> ToBeVisored;
	
	// Array of objects/enemies to ignore at visoring
	UPROPERTY()
	TArray<AActor*> ToBeIgnoredByVisor;
	
	// sphere trigger which is active when we are using visor
	UPROPERTY()
	class USphereComponent* SphereDetectingVisorables;

	//Types of collisions by which objects are going to be visored
	UPROPERTY()
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToVisor;

	// Particle system effect to show
	UPROPERTY()
	UParticleSystemComponent* VisualEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle TimerHandleDestroyer;

	// Makes visor object destroy itself
	UFUNCTION()
	void SelfDestruction();

	// This function is both starting and ending function
	UFUNCTION()
	void VisorAbility();

	//The range in which enemies and objects are visored 
	UPROPERTY()
	float VisorRadius = 2000.0f;

	//Lifetime of ability
	UPROPERTY()
	float VisorDuration = 5.0f;
	
	// A particle effect which will be displaying the visor point
	UPROPERTY()
	UParticleSystem* PlayedEffect;

	// declare overlap begin function used specially for detecting objects when using Visor function
	UFUNCTION()
	void OnOverlapBeginForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function used specially for detecting objects when using visor function
	UFUNCTION()
	void OnOverlapEndForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void SetObjectTypesToVisor(TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisor);
	
};
