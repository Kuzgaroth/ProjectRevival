// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickup/BasePickup.h"
#include "AmmoPickup.generated.h"

class ABaseWeapon;

UCLASS()
class PROJECTREVIVAL_API AAmmoPickup : public ABasePickup
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pickup", meta=(ClampMin="1.0", ClampMax="10.0"))
	int32 ClipsAmount = 10.0f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pickup")
	TSubclassOf<ABaseWeapon> WeaponType; 
	
	float RespawnTime = 5.0f;
private:
	virtual bool GivePickUpTo(APawn* PlayerPawn) override;
};
