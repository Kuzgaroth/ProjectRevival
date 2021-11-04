// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Pickup/BasePickup.h"
#include "Interfaces/BotPickupInterface.h"
#include "HealthPickup.generated.h"



UCLASS()
class PROJECTREVIVAL_API AHealthPickup : public ABasePickup, public IBotPickupInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
	void OnInteraction(AActor* Actor);
	virtual void OnInteraction_Implementation(AActor* Actor);
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Pickup", meta=(ClampMin="1.0", ClampMax="100.0"))
	float HealthAmount = 50.0f;
	
private:
	virtual bool GivePickUpTo(APawn* PlayerPawn) override;
};
