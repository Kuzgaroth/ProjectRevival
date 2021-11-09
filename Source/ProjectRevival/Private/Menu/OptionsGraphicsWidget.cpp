// Project Revival. All Rights Reserved


#include "Menu/OptionsGraphicsWidget.h"


#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "RHI.h"
#include "Components/Button.h"

void UOptionsGraphicsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (ApplyButton)
	{
		ApplyButton->OnClicked.AddDynamic(this, &UOptionsGraphicsWidget::ApplyChanges);	
	}

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
	if (ShadowsBoxString)
	{
		ShadowsBoxString->AddOption("Low");
		ShadowsBoxString->AddOption("Medium");
		ShadowsBoxString->AddOption("High");
		ShadowsBoxString->AddOption("Epic");
		ShadowsBoxString->AddOption("Cinematic");

		ShadowsBoxString->SetSelectedIndex(GameSettings->GetShadingQuality());
	}

	if (AntiAliasingBoxString)
	{
		AntiAliasingBoxString->AddOption("Low");
		AntiAliasingBoxString->AddOption("Medium");
		AntiAliasingBoxString->AddOption("High");
		AntiAliasingBoxString->AddOption("Epic");
		AntiAliasingBoxString->AddOption("Cinematic");

		AntiAliasingBoxString->SetSelectedIndex(GameSettings->GetAntiAliasingQuality());
	}
}

void UOptionsGraphicsWidget::ApplyChanges()
{
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();

	if (FullScreenCheckBox)
	{
		if (FullScreenCheckBox->IsChecked())
		{
			GameSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		}
		else
		{
			GameSettings->SetFullscreenMode(EWindowMode::Windowed);
		}
	}
	if (VSyncCheckBox)
	{
		if (VSyncCheckBox->IsChecked())
		{
			GameSettings->SetVSyncEnabled(true);
		}
		else
		{
			GameSettings->SetVSyncEnabled(false);
		}
	}
	if (ResolutionBoxString)
	{
		GameSettings->SetScreenResolution(PossibleResolutions[ResolutionBoxString->GetSelectedIndex()]);	
	}
	if (ShadowsBoxString)
	{
		GameSettings->SetShadowQuality(ShadowsBoxString->GetSelectedIndex());
	}
	if (AntiAliasingBoxString)
	{
		GameSettings->SetAntiAliasingQuality(AntiAliasingBoxString->GetSelectedIndex());
	}
	
	GameSettings->ApplySettings(false);
}
