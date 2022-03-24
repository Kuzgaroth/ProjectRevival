// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Magazine.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTREVIVAL_API AMagazine : public AActor
{
	GENERATED_BODY()

public:
	AMagazine();
	
	UFUNCTION()
	void AttachMagazine(USceneComponent* ParentMesh, const FName& SocketName);
	UFUNCTION()
	void DetachMagazine(USceneComponent* ParentMesh, const FName& SocketName);
	
protected:
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	//UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMeshComponent;
	UPROPERTY(VisibleDefaultsOnly,					Category="Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleDefaultsOnly,					Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InternalState")
	float LifeSeconds = 10.f;

	virtual void BeginPlay() override;
};
