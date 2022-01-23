// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerInput.h"

#include "OptionsKeyRemapWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UOptionsKeyRemapWidget : public UUserWidget
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
	void SetContent(const FInputActionKeyMapping KeyMapping);
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
private:
	bool bCanInput;

	FInputActionKeyMapping KeyMap;
	
	UFUNCTION()
    void OnChangeInput();

	UFUNCTION()
	void OnChangeInputPressed();

	UFUNCTION()
	void OnChangeInputReleased();
	
};
