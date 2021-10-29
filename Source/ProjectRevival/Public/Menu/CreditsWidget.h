// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UButton* BackButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnBack();
};
