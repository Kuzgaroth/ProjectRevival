// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Shell.generated.h"

//class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTREVIVAL_API AShell : public AActor
{
	GENERATED_BODY()

public:
	AShell();
	
protected:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleDefaultsOnly,					Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float LifeSeconds = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float Speed = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float RotationXY = 0.3f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float RotationZ = 0.4f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float Dispersion = 10.f;

	UPROPERTY(EditDefaultsOnly, Category="Sound")
	TMap<UPhysicalMaterial*, USoundCue*> SoundMap;
	UPROPERTY(EditDefaultsOnly)
	USoundCue* DefaultHitSound;
	UFUNCTION()
	virtual void PlaySoundAtHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	virtual void BeginPlay() override;
};
