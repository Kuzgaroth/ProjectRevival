// Project Revival. All Rights Reserved


#include "Menu/OptionsKeyRemapWidget.h"
#include "Components/Button.h"
#include "GameFramework/InputSettings.h"


void UOptionsKeyRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TipText->SetText(FText::FromString(""));

	if (ChangeInputButton)
	{
		// ChangeInputButton->OnClicked.AddDynamic(this, &UOptionsKeyRemapWidget::OnChangeInput);
		ChangeInputButton->OnClicked.AddDynamic(this, &UOptionsKeyRemapWidget::OnChangeInputPressed);
		// ChangeInputButton->OnReleased.AddDynamic(this, &UOptionsKeyRemapWidget::OnChangeInputReleased);
	}
}

void UOptionsKeyRemapWidget::SetContent(const FInputActionKeyMapping KeyMapping)
{
	KeyMap = KeyMapping;
	ActionText->SetText(FText::FromString(KeyMapping.ActionName.ToString()));
	KeyText->SetText(FText::FromString(KeyMapping.Key.ToString()));
}

FReply UOptionsKeyRemapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	// Super::NativeOnKeyDown(InGeometry, InKeyEvent);

	bool bCanUse = true;

	FReply Reply = FReply::Unhandled();

	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

	for (FInputActionKeyMapping ActionMapping: ActionMappings)
	{
		if(ActionMapping.Key == InKeyEvent.GetKey())
		{
			bCanUse = false;
		}
	}

	TArray<FInputAxisKeyMapping> AxisMappings = Settings->GetAxisMappings();

	for (FInputAxisKeyMapping AxisMapping: AxisMappings)
	{
		if(AxisMapping.Key == InKeyEvent.GetKey())
		{
			bCanUse = false;
		}
	}

	if (bCanInput && bCanUse)
	{
		Settings->RemoveActionMapping(KeyMap);
		KeyMap.Key = InKeyEvent.GetKey();
		SetContent(KeyMap);
		bCanInput = false;
		Settings->AddActionMapping(KeyMap);
		TipText->SetText(FText::FromString(""));
		
		Reply = FReply::Handled();
	}
	else
	{
		TipText->SetText(FText::FromString("Please, try another key"));
	}

	return Reply;

}

FReply UOptionsKeyRemapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bool bCanUse = true;

	FReply Reply = FReply::Unhandled();

	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

	for (FInputActionKeyMapping ActionMapping: ActionMappings)
	{
		if(ActionMapping.Key == InMouseEvent.GetEffectingButton())
		{
			bCanUse = false;
		}
	}

	TArray<FInputAxisKeyMapping> AxisMappings = Settings->GetAxisMappings();

	for (FInputAxisKeyMapping AxisMapping: AxisMappings)
	{
		if(AxisMapping.Key == InMouseEvent.GetEffectingButton())
		{
			bCanUse = false;
		}
	}

	if (bCanInput && bCanUse)
	{
		Settings->RemoveActionMapping(KeyMap);
		KeyMap.Key = InMouseEvent.GetEffectingButton();
		SetContent(KeyMap);
		bCanInput = false;
		Settings->AddActionMapping(KeyMap);
		TipText->SetText(FText::FromString(""));
		
		Reply = FReply::Handled();
	}
	else
	{
		TipText->SetText(FText::FromString("Please, try another key"));
	}

	return Reply;
}

void UOptionsKeyRemapWidget::OnChangeInput()
{
	bCanInput = true;
	SetKeyboardFocus();
}

void UOptionsKeyRemapWidget::OnChangeInputPressed()
{
	KeyText->SetText(FText::FromString("?"));
	TipText->SetText(FText::FromString("Press desired key"));
	bCanInput = true;
	SetKeyboardFocus();
}

void UOptionsKeyRemapWidget::OnChangeInputReleased()
{
	SetKeyboardFocus();
}
