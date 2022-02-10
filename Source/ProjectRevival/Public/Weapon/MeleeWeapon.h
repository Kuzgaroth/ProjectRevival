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
	void AddNewBeam(const FVector Point1, const FVector Point2);

	void ToggleCollisionOn();
	void ToggleCollisionOff() const;
	bool IsHitDone() const { return bIsHitDone; };
	void ResetHitStatus() { bIsHitDone = false; };
	
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
	virtual void StartFire() override { MakeShot(); };
	virtual void StopFire() override {};
	virtual void MakeShot() override {};
	bool bIsHitDone = false;
};
