// Project Revival. All Rights Reserved


#include "Menu/OptionsControlsWidget.h"

#include "Components/Button.h"
#include "GameFramework/InputSettings.h"
#include "Menu/OptionsAxisRemapWidget.h"
#include "Menu/OptionsKeyRemapWidget.h"

void UOptionsControlsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (KeyVerticalBox)
	{
		UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

		KeyVerticalBox->AddChild(ActionSizeBox);

		TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

		for (const FInputActionKeyMapping ActionMapping: ActionMappings)
		{
			if (KeyMapClass)
			{
				UOptionsKeyRemapWidget* KeyMapWidget = CreateWidget<UOptionsKeyRemapWidget>(GetWorld(), KeyMapClass);
				KeyMapWidget->SetContent(ActionMapping);
				KeyVerticalBox->AddChild(KeyMapWidget);	
			}
		}

		KeyVerticalBox->AddChild(AxisSizeBox);
		
		TArray<FInputAxisKeyMapping> AxisMappings = Settings->GetAxisMappings();

		for (const FInputAxisKeyMapping AxisMapping: AxisMappings)
		{
			if (AxisMapClass)
			{
				UOptionsAxisRemapWidget* AxisMapWidget = CreateWidget<UOptionsAxisRemapWidget>(GetWorld(), AxisMapClass);
				AxisMapWidget->SetContent(AxisMapping);
				KeyVerticalBox->AddChild(AxisMapWidget);	
			}
		}
	}
}

