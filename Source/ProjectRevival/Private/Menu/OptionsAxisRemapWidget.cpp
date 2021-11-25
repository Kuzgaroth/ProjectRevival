
// Project Revival. All Rights Reserved


#include "Menu/OptionsAxisRemapWidget.h"

#include "Components/Button.h"
#include "GameFramework/InputSettings.h"

void UOptionsAxisRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bCanInput = true;

	if (ChangeInputButton)
	{
		ChangeInputButton->OnPressed.AddDynamic(this, &UOptionsAxisRemapWidget::OnChangeInputPressed);
		ChangeInputButton->OnReleased.AddDynamic(this, &UOptionsAxisRemapWidget::OnChangeInputReleased);
	}
}

void UOptionsAxisRemapWidget::SetContent(const FInputAxisKeyMapping KeyMapping)
{
	KeyMap = KeyMapping;
	ActionText->SetText(FText::FromString(KeyMapping.AxisName.ToString()));
	KeyText->SetText(FText::FromString(KeyMapping.Key.ToString()));
}

FReply UOptionsAxisRemapWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
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
		Settings->RemoveAxisMapping(KeyMap);
		KeyMap.Key = InKeyEvent.GetKey();
		SetContent(KeyMap);
		bCanInput = false;
		Settings->AddAxisMapping(KeyMap);

		Reply = FReply::Handled();
	}

	return Reply;
}

void UOptionsAxisRemapWidget::OnChangeInputPressed()
{
	KeyText->SetText(FText::FromString("?"));
	bCanInput = true;
}

void UOptionsAxisRemapWidget::OnChangeInputReleased()
{
	KeyText->SetText(FText::FromString(KeyMap.Key.ToString()));
}


