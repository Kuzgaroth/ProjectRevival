// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Miscellaneuos/IDynMaterialsFromMesh.h"
#include "Components/WeaponComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraComponent;
class USoundCue;

UCLASS()
class PROJECTREVIVAL_API ABaseWeapon : public AActor, public IIDynMaterialsFromMesh
{
	GENERATED_BODY()
	
public:	
	ABaseWeapon();
	FOnClipEmptySignature OnClipEmpty;
	void ChangeClip();
	bool CanReload() const;
	FWeaponUIData GetUIData() const {return UIData;}
	FAmmoData GetAmmoData() const {return CurrentAmmo;}
	bool IsAmmoEmpty() const;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName MuzzelSocketName="MuzzleFlashSocket";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShootingRange=1000.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShotDamage=10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ammo")
	FAmmoData DefaultAmmo{15, 10, false};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	FWeaponUIData UIData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	UNiagaraSystem* MuzzelFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundCue* FireSound;
	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void MakeShot();

	APlayerController* GetPlayerController() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	void MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd);
	void DecreaseAmmo();
	
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;
	UNiagaraComponent* SpawnMuzzelFX();
public:	
	virtual void StartFire();
	virtual void StopFire();
	bool TryToAddAmmo(int32 ClipsAmount);
	virtual TArray<UMaterialInstanceDynamic*>& GetDynMaterials() override;
private:
	FAmmoData CurrentAmmo;
	UPROPERTY()
	TArray<UMaterialInstanceDynamic*> DynamicMaterials;
};
