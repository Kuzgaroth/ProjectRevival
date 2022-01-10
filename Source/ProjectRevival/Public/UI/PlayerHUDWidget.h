// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"

#include "CirclePBWidget.h"
#include "HealthPBWidget.h"
#include "Blueprint/UserWidget.h"
#include "ProjectRevival/Public/CoreTypes.h"
#include "PlayerHUDWidget.generated.h"

UCLASS()
class PROJECTREVIVAL_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable, Category="UI")
	float GetHealthPercentage() const ;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const;
	
	UFUNCTION(BlueprintCallable, Category="UI")
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerSpectating() const;

	UFUNCTION(BlueprintCallable, Category="UI")
	bool IsPlayerAlive() const;

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void OnTakeDamage(float Health);
	
protected:
	UPROPERTY(meta=(BindWidget))
	UHealthPBWidget* HealthPB;
	
	virtual void NativeOnInitialized() override;
private:
	void OnHealthChanged(float Health, float DeltaHealth);
	void OnNewPawn(APawn* NewPawn);
};
