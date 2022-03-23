// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/RifleWeapon.h"
#include "Shell.h"
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
	void AttachMagazine(ABaseWeapon* Weapon, USceneComponent* CharacterMesh, const FName& SocketName);

	UFUNCTION()
	void DetachMagazine();

	UFUNCTION()
	void SpawnShell();
protected:
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Magazine")
	AActor* Magazine;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Magazine")
	USceneComponent* AttachPoint;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AShell> AmmoShell;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShutterMovement;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Magazine")
	FName ArmSocketName = "middle_01_l";
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Magazine")
	FName ShutterSocketName = "gate";
	
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Magazine")
	FName MagazineSocketName = "extended_magazine";
private:
	//UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Animations")
	//UAnimNotify* AttachMagazine;
};
