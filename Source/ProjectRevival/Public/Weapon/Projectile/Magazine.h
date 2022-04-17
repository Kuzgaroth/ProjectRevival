// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Magazine.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustom, Log, All);

class UProjectileMovementComponent;

UCLASS()
class PROJECTREVIVAL_API AMagazine : public AActor
{
	GENERATED_BODY()

public:
	AMagazine();
	
	UFUNCTION()
	void DetachMagazine();
	UFUNCTION()
	USkeletalMeshComponent* GetMeshComponent();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* MeshComponent;
	UPROPERTY(VisibleDefaultsOnly,					Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InternalState")
	float LifeSeconds = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="InternalState")
	float InitialFallingSpeed = 500.f;

	virtual void BeginPlay() override;
};
