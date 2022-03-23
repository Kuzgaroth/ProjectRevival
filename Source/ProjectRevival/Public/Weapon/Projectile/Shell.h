// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shell.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
DECLARE_LOG_CATEGORY_EXTERN(LogCustom, Log, All);

UCLASS()
class PROJECTREVIVAL_API AShell : public AActor
{
	GENERATED_BODY()

public:
	AShell();

	UFUNCTION()
	UStaticMeshComponent* GetMesh() const;
protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USphereComponent* CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="InternalState")
	float LifeSeconds = 10.f;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* MovementComponent;

	virtual void BeginPlay() override;
	FVector MovementDirection;
};
