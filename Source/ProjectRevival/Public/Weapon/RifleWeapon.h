// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/BaseWeapon.h"
#include "RifleWeapon.generated.h"

class UWeaponFXComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UParticleSystemComponent;
class UParticleSystem;
class UAudioComponent;
class USoundCue;

UENUM(BlueprintType)
enum EBodyPart
{
	Body,
	Head,
	NonePart
};

UCLASS()
class PROJECTREVIVAL_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()
public:
	ARifleWeapon();
	
	virtual void StartFire() override;
	virtual void StopFire() override;
protected:
	UPROPERTY(VisibleAnywhere, Category="VFX")
	UWeaponFXComponent* WeaponFXComponent;

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BulletSpread=1.5f;

	UPROPERTY(EditDefaultsOnly, Category="VFX")
	TMap<UPhysicalMaterial*, TEnumAsByte<EBodyPart>> BodyMaterialMap;
	
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
	void MakeDamage(FHitResult& HitResult);
	virtual void BeginPlay() override;

	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
	bool IsHitInHead(const UPhysicalMaterial* PhysMaterial);
	void ProcessEnemyHit(const FHitResult& HitResult);
	
	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponentNiagara;
	UPROPERTY()
	UParticleSystemComponent* MuzzleFXComponentCascade;
	
	UPROPERTY()
	UAudioComponent* FireAudioComponent;
	
private:
	FTimerHandle ShotTimerHandle;
	
	AController* GetController() const;
};
