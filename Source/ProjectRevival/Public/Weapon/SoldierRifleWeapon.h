// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RifleWeapon.h"
#include "SoldierRifleWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedFireInWeapon);

class UWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;
class USoundCue;

UCLASS()
class PROJECTREVIVAL_API ASoldierRifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
	
public:
	ASoldierRifleWeapon();
	virtual void StartFire() override;
	//Triggers StoppedFireDelegate. Use StopFireExternal() to avoid it
	virtual void StopFire() override;
	//Stops fire without broadcasting StoppedFireDelegate.
	virtual void StopFireExternal();
	
	FStoppedFireInWeapon StoppedFireInWeaponDelegate;

	//By default is true. If false than it will shoot in a common rifle non-stop way (which is yet quite not implemented in soldier lol)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	bool bUseBurst = true;

	//Number of shoot rows in a series
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	int32 BurstClipsNumber = 1;

	//Number of shots in one row
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	int32 BurstBulletsNumber = 3;

	//Delay between shoot rows
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	float BurstClipsDelay = 1.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMode")
	float BurstBulletsDelay = 0.5f;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd) override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
	virtual void MakeDamage(FHitResult& HitResult);
	virtual void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
	// virtual void InitFX();
	virtual void InitFX();
	virtual void PlayFX(bool bActivate);
	virtual void PlaySound();
	UFUNCTION()
	virtual void MakeShotInternal();
	UFUNCTION()
	virtual void ShootRowInternal();
	AController* GetController() const;

	UPROPERTY()
	float CurrentRowTime;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="FireMod")
	float BulletSpread = 1.0f;

	//if set to "true" then Niagara is used, otherwise uses Cascade. By default is set to "true"
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	bool bUseNiagaraTraceEffect = true;
	
	//Niagara effect to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* TraceFXNiagara;
	
	//Cascade effect to play
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UParticleSystem* TraceFXCascade;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(VisibleAnywhere, Category="VFX")
	UWeaponFXComponent* WeaponFXComponent;
	
	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponentNiagara;
	UPROPERTY()
	UParticleSystemComponent* MuzzleFXComponentCascade;

private:
	int32 CurrentBurstShot;
	int32 CurrentBurstRow;
	
	FTimerHandle BulletsTimerHandle;
	FTimerHandle ClipsTimerHandle;
};
