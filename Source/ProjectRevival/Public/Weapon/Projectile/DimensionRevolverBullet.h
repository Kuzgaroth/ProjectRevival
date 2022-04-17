// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile/BaseProjectile.h"
#include "DimensionRevolverBullet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API ADimensionRevolverBullet : public ABaseProjectile
{
	GENERATED_BODY()
public:
	void SetHealPercentage(const float Percentage){HealHealthPercent=Percentage;}
protected:
	virtual void BeginPlay() override;
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	float HealHealthPercent=30.0f;

};
