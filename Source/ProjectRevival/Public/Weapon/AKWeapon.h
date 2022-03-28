// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Shell.h"
#include "Magazine.h"
#include "Trace/Detail/EventNode.h"
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
	void SpawnShell(FName SocketName) const;
	UFUNCTION()
	void SpawnMagazine(FName SocketName);
	
	//функции, которые вызываются в соответствующих AnimNotify:
	//DropMagazineAnimNotify
	UFUNCTION()
	void Drop();
	//TakeMagazineAnimNotify
	UFUNCTION()
	void Take();
	//LockMagazineAnimNotify
	UFUNCTION()
	void Lock();
	
protected:
	UPROPERTY()
	TArray<AMagazine*> Magazines;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	TSubclassOf<AActor> MagazineClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* MagazineMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AShell> AmmoShell;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ShutterMovement;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName ArmSocketName = "left_arm_socket";
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName ShutterSocketName = "gate";
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category = "Magazine")
	FName MagazineSocketName = "magazine_endSocket";
private:
	FVector MagazineScale;
};

