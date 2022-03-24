// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Shell.h"
#include "Magazine.h"
#include "Weapon/RifleWeapon.h"
#include "AKWeapon.generated.h"

class ABaseProjectile;

UCLASS()
class PROJECTREVIVAL_API AKWeapon : public ARifleWeapon
{
	GENERATED_BODY()
public:
	AKWeapon();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void MakeShot() override;
	
	UFUNCTION()
	void SpawnShell() const;
	UFUNCTION()
	void SpawnMagazine() const;
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	TSubclassOf<AMagazine> Magazine;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	USceneComponent* AttachPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AShell> AmmoShell;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShutterMovement;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName ArmSocketName = "middle_01_l";
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName ShutterSocketName = "gate";
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName MagazineSocketName = "extended_magazine";
private:
	
	//UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Animations")
	//UAnimNotify* AttachMagazine;
};
