// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PROJECTREVIVAL_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category="Pickup", meta=(ClampMin="1.0", ClampMax="10.0"))
	float RespawnTime = 5.0f;;
	
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
public:	
	virtual void Tick(float DeltaTime) override;
private:
	float RotationYaw = 0.0f;
	virtual bool GivePickUpTo(APawn* PlayerPawn);
	void PickupWasTaken();
	void Respawn();
	void GenerateRotationYaw();
};
