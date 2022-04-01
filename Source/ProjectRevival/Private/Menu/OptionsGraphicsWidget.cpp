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
	if (ApplyDefaultButton)
	{
		ApplyDefaultButton->OnClicked.AddDynamic(this, &UOptionsGraphicsWidget::ApplyDefaultChanges);
	}

	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	TArray<FText> ResolutionList;
	FScreenResolutionArray Resolutions;
	if (RHIGetAvailableResolutions(Resolutions, false))
	{
		for (const FScreenResolutionRHI& EachResolution : Resolutions)
		{ 
			PossibleResolutions.Add(FIntPoint(EachResolution.Width, EachResolution.Height));

			if (ResolutionBoxString)
			{
				ResolutionBoxString->AddOption(FString::Printf(TEXT("%dx%d  %dHz"), EachResolution.Width,
                                                                EachResolution.Height, EachResolution.RefreshRate));
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
	}
	if (AntiAliasingBoxString)
	{
		AntiAliasingBoxString->AddOption("Low");
		AntiAliasingBoxString->AddOption("Medium");
		AntiAliasingBoxString->AddOption("High");
		AntiAliasingBoxString->AddOption("Epic");
		AntiAliasingBoxString->AddOption("Cinematic");
	}
	
	SetView();
}

void UOptionsGraphicsWidget::SetView()
{
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();

	if (ResolutionBoxString)
	{
		ResolutionBoxString->SetSelectedIndex(PossibleResolutions.Find(GameSettings->GetScreenResolution()));
	}
	// Setting fullscreen mode
	if (FullScreenCheckBox)
	{
		if (GameSettings->GetFullscreenMode() == EWindowMode::Fullscreen)
		{
			FullScreenCheckBox->SetIsChecked(true);	
		}
		else
		{
			FullScreenCheckBox->SetIsChecked(false);
		}
	}
	if (VSyncCheckBox)
	{
		if (GameSettings->IsVSyncEnabled())
		{
			VSyncCheckBox->SetIsChecked(true);	
		}
		else
		{
			VSyncCheckBox->SetIsChecked(false);
		}
	}
	// Setting shadow quality
	if (ShadowsBoxString)
	{
		ShadowsBoxString->SetSelectedIndex(GameSettings->GetShadingQuality());
	}
	// Setting graphics quality
	if (AntiAliasingBoxString)
	{
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

void UOptionsGraphicsWidget::ApplyDefaultChanges()
{
	UGameUserSettings* GameSettings = GEngine->GetGameUserSettings();
	if (GameSettings)
	{
		GameSettings->SetToDefaults();
		GameSettings->SetFullscreenMode(EWindowMode::Fullscreen);
		GameSettings->SetScreenResolution(PossibleResolutions.Last());
		GameSettings->ApplySettings(false);
		SetView();
	}
}
