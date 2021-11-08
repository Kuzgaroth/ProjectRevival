// Project Revival. All Rights Reserved


#include "Menu/OptionsGraphicsWidget.h"


#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "RHI.h"

void UOptionsGraphicsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<FText> ResolutionList;
	FScreenResolutionArray Resolutions;
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	if (RHIGetAvailableResolutions(Resolutions, false))
	{
		for (const FScreenResolutionRHI& EachResolution : Resolutions)
		{ 
			PossibleResolutions.Add(FIntPoint(EachResolution.Width, EachResolution.Height));

			if (ResolutionBoxString)
			{
				ResolutionBoxString->AddOption(FString::Printf(TEXT("%dx%d  %dHz"), EachResolution.Width,
																EachResolution.Height, EachResolution.RefreshRate));
				ResolutionBoxString->SetSelectedIndex(PossibleResolutions.Find(GameSettings->GetScreenResolution()));
			}
		}
	}
}

void UOptionsGraphicsWidget::ApplyChanges()
{
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();

	if (FullScreenCheckBox)
	{
		if(FullScreenCheckBox->IsChecked())
		{
			GameSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else
		{
			GameSettings->SetFullscreenMode(EWindowMode::Windowed);
		}
	}
	if (ResolutionBoxString)
	{
		GameSettings->SetScreenResolution(PossibleResolutions[ResolutionBoxString->GetSelectedIndex()]);	
	}
	GameSettings->ApplySettings(false);
}
