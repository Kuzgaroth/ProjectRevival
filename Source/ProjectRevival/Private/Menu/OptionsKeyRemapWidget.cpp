// Project Revival. All Rights Reserved


#include "Menu/OptionsKeyRemapWidget.h"
#include "Components/Button.h"
#include "GameFramework/InputSettings.h"


void UOptionsKeyRemapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ChangeInputButton)
	{
		ChangeInputButton->OnClicked.AddDynamic(this, &UOptionsKeyRemapWidget::OnChangeInput);
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
	Super::NativeOnKeyDown(InGeometry, InKeyEvent);

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

		Reply = FReply::Handled();
	}

	return Reply;

}

void UOptionsKeyRemapWidget::OnChangeInput()
{
	bCanInput = true;
	SetKeyboardFocus();
}
