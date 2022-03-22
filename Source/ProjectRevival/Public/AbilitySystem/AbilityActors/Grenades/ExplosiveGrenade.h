// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AbilityActors/Grenades/BaseGrenade.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "ExplosiveGrenade.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API AExplosiveGrenade : public ABaseGrenade
{
	GENERATED_BODY()

public:
	AExplosiveGrenade();
	
	virtual void GrenadeAction() override;
	virtual void SelfDestruction() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Action parameters")
	float ExplosionRadius = 200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Action parameters")
	float ExplosionDamage = 50.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Action parameters")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToDamage;
protected:
	virtual void BeginPlay() override;
	virtual void SpawnActionEffect() override;
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit) override;
	AController* GetController() const;

	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	URadialForceComponent* RadialForceComponent;

	UPROPERTY()
	TArray<AActor*> ToBeIgnoredByVisor;
};
