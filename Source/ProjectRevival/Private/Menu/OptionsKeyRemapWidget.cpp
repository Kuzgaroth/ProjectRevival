// Project Revival. All Rights Reserved


#include "Menu/OptionsKeyRemapWidget.h"
#include "Components/Button.h"
#include "GameFramework/InputSettings.h"


void UOptionsKeyRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ChangeInputButton)
	{
		ChangeInputButton->OnClicked.AddDynamic(this, &UOptionsKeyRemapWidget::OnChangeInputPressed);
	}
}

void UOptionsKeyRemapWidget::SetContent(const FInputActionKeyMapping KeyMapping)
{
	KeyMap = KeyMapping;
	ActionText->SetText(FText::FromString(KeyMapping.ActionName.ToString()));
	KeyText->SetText(FText::FromString(KeyMapping.Key.ToString()));
	TipText->SetText(FText::FromString(""));
}

FReply UOptionsKeyRemapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	bool bCanUse = true;

	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey().GetDisplayName().EqualTo(FText::FromString("Escape")))
	{
		SetContent(KeyMap);
		bCanInput = false;
		ChangeInputButton->SetIsEnabled(true);
		Reply = FReply::Handled();
		
		return Reply;
	}
	
	UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());
	TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

	for (FInputActionKeyMapping ActionMapping: ActionMappings)
	{
		if(ActionMapping.Key == InKeyEvent.GetKey() && ActionMapping.ActionName != KeyMap.ActionName)
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
		Settings->AddActionMapping(KeyMap, true);
		Settings->SaveKeyMappings();
		ChangeInputButton->SetIsEnabled(true);
		
		Reply = FReply::Handled();
	}
	else if (bCanInput && !bCanUse)
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
		if(ActionMapping.Key == InMouseEvent.GetEffectingButton() && ActionMapping.ActionName != KeyMap.ActionName)
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
		Settings->AddActionMapping(KeyMap, true);
		Settings->SaveKeyMappings();
		ChangeInputButton->SetIsEnabled(true);

		Reply = FReply::Handled();
	}
	else if (bCanInput && !bCanUse)
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
	ChangeInputButton->SetIsEnabled(false);
}

void UOptionsKeyRemapWidget::OnChangeInputReleased()
{
	SetContent(KeyMap);
	bCanInput = false;
}
