// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "MeleeWeapon.generated.h"

class ABaseProjectile;
class USoundCue;

UCLASS()
class PROJECTREVIVAL_API AMeleeWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	AMeleeWeapon();
	void AddNewBeam(FVector Point1, FVector Point2);
	
	virtual void StartFire() override;
	virtual void StopFire() override;
	
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BladeCollisionBox;
    	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ThrowPosition")
	UParticleSystemComponent* BeamComp;
	TArray<UParticleSystemComponent*>BeamArray;
    		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* HitSound;
    
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystem* BeamFX;
protected:
	
	
	virtual void MakeShot() override;
};
