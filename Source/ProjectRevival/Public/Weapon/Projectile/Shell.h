// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	//Отклонение траектории гильз от правого вектора меша оружия
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float Rotation = 1.f;

	//Случайный разброс
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,	Category="InternalState")
	float Dispersion = 0.2f;
	
	virtual void BeginPlay() override;
};
