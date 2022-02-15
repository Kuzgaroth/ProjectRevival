// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"

#include "AmmoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UAmmoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;

	void SetAmmoAmount(int NewAmmoAmount);

protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* AmmoPB;
	
};
