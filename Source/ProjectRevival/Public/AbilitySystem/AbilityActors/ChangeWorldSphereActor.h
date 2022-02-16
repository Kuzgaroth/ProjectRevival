// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "ChangeWorldSphereActor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

DECLARE_MULTICAST_DELEGATE(FOnEndAbitity)
UCLASS()
class PROJECTREVIVAL_API AChangeWorldSphereActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AChangeWorldSphereActor();

	
private:
	UPROPERTY()
	USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	UNiagaraSystem* ChangeWorldFX;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	UNiagaraComponent* ChangeWorldFXComponent;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	float StartRadius=10.0f;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	float CurrentRadius;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	float EndRadius=1000.0f;
	UPROPERTY(EditAnywhere,Category="Sphere params")
	float ExpantionSpeed=100.0f;
	UFUNCTION()
	void OnSphereComponentCollision(UPrimitiveComponent* OverlappedComponent, 
					  AActor* OtherActor, 
					  UPrimitiveComponent* OtherComp, 
					  int32 OtherBodyIndex, 
					  bool bFromSweep, 
					  const FHitResult &SweepResult);
	UNiagaraComponent* SpawnChangeWorldEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

		
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FOnEndAbitity AbilityEnded;
	

};
