// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerInput.h"


#include "OptionsAxisRemapWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsAxisRemapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	UButton* ChangeInputButton;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ActionText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* KeyText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* TipText;

	virtual void NativeOnInitialized() override;

	UFUNCTION()
    void SetContent(const FInputAxisKeyMapping KeyMapping);
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	bool bCanInput;

	FInputAxisKeyMapping KeyMap;

	UFUNCTION()
    void OnChangeInputPressed();
	
};
