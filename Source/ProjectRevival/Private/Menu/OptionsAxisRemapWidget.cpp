
// Project Revival. All Rights Reserved


#include "Menu/OptionsAxisRemapWidget.h"

#include "Components/Button.h"
#include "GameFramework/InputSettings.h"

void UOptionsAxisRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ChangeInputButton)
	{
		ChangeInputButton->OnClicked.AddDynamic(this, &UOptionsAxisRemapWidget::OnChangeInputPressed);
	}
}

void UOptionsAxisRemapWidget::SetContent(const FInputAxisKeyMapping KeyMapping)
{
	KeyMap = KeyMapping;
	ActionText->SetText(FText::FromString(KeyMapping.AxisName.ToString()));
	KeyText->SetText(FText::FromString(KeyMapping.Key.ToString()));
	TipText->SetText(FText::FromString(""));
}

FReply UOptionsAxisRemapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	bool bCanUse = true;

	FReply Reply = FReply::Unhandled();

	if (InKeyEvent.GetKey().GetDisplayName().EqualTo(FText::FromString("Escape")))
	{
		SetContent(KeyMap);
		bCanInput = false;
		Reply = FReply::Handled();
		
		return Reply;
	}

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
		if(AxisMapping.Key == InKeyEvent.GetKey() && AxisMapping.AxisName != KeyMap.AxisName)
		{
			bCanUse = false;
		}
	}

	if (bCanInput && bCanUse)
	{
		Settings->RemoveAxisMapping(KeyMap);
		KeyMap.Key = InKeyEvent.GetKey();
		SetContent(KeyMap);
		bCanInput = false;
		Settings->AddAxisMapping(KeyMap, true);
		Settings->SaveKeyMappings();

		Reply = FReply::Handled();
	}
	else if (bCanInput && !bCanUse)
	{
		TipText->SetText(FText::FromString("Please, try another key"));
	}

	return Reply;
}

FReply UOptionsAxisRemapWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
		if(AxisMapping.Key == InMouseEvent.GetEffectingButton() && AxisMapping.AxisName != KeyMap.AxisName)
		{
			bCanUse = false;
		}
	}

	if (bCanInput && bCanUse)
	{
		Settings->RemoveAxisMapping(KeyMap);
		KeyMap.Key = InMouseEvent.GetEffectingButton();
		SetContent(KeyMap);
		bCanInput = false;
		Settings->AddAxisMapping(KeyMap, true);
		Settings->SaveKeyMappings();

		Reply = FReply::Handled();
	}
	else if (bCanInput && !bCanUse)
	{
		TipText->SetText(FText::FromString("Please, try another key"));
	}

	return Reply;
}

void UOptionsAxisRemapWidget::OnChangeInputPressed()
{
	KeyText->SetText(FText::FromString("?"));
	TipText->SetText(FText::FromString("Press desired key"));
	bCanInput = true;
	SetKeyboardFocus();
}

void UOptionsAxisRemapWidget::OnChangeInputReleased()
{
	KeyText->SetText(FText::FromString(KeyMap.Key.ToString()));
}


