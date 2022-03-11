// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


#include "ConfirmationWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UConfirmationWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UButton* ConfirmButton;

	UPROPERTY(meta=(BindWidget))
	UButton* DeclineButton;

	UFUNCTION()
	void SetLabelText(FText NewLabelText) { LabelText->SetText(NewLabelText); }

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* LabelText;
	
	virtual void NativeOnInitialized() override;
};
