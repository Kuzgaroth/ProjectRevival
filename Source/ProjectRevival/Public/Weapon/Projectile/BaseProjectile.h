// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UWeaponFXComponent;

UCLASS()
class PROJECTREVIVAL_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, Category="VFX")
	UWeaponFXComponent* WeaponFXComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	float DamageRadius = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	bool bDoFullDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="InternalState")
	float LifeSeconds;
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	virtual void BeginPlay() override;

	FVector ShotDirection;
	AController* GetController() const;

public:	
	ABaseProjectile();
	FORCEINLINE void SetShotDirection(const FVector& Direction){ShotDirection = Direction;};

};
