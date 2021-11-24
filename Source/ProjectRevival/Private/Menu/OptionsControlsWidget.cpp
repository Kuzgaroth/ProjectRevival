// Project Revival. All Rights Reserved


#include "Menu/OptionsControlsWidget.h"

#include "Components/Button.h"
#include "GameFramework/InputSettings.h"
#include "Menu/OptionsKeyRemapWidget.h"

void UOptionsControlsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (KeyVerticalBox)
	{
		UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>());

		TArray<FInputActionKeyMapping> ActionMappings = Settings->GetActionMappings();

		for (FInputActionKeyMapping ActionMapping: ActionMappings)
		{
			if (KeyMapClass)
			{
				UOptionsKeyRemapWidget* KeyMapWidget = CreateWidget<UOptionsKeyRemapWidget>(GetWorld(), KeyMapClass);
				KeyMapWidget->SetContent(ActionMapping);
				KeyVerticalBox->AddChild(KeyMapWidget);	
			}
		}
	}
}

