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
	FComponentHitSignature GetHitDelegate() const;
protected:
	virtual void BeginPlay() override;
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual void LifeSpanExpired() override;
	

};
