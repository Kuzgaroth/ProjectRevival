// Project Revival. All Rights Reserved


#include "Menu/OptionsWidget.h"

#include "PauseWidget.h"
#include "Components/Button.h"
#include "PRGameInstance.h"
#include "Menu/OptionsControlsWidget.h"
#include "Menu/OptionsGraphicsWidget.h"
#include "Menu/OptionsSoundWidget.h"
#include "ProjectRevival/Public/Menu/MenuWidget.h"

void UOptionsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnBack);
	}
	if (ControlsButton)
	{
		ControlsButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		ControlsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnControls);
	}
	if (GraphicsButton)
	{
		GraphicsButton->SetBackgroundColor(FLinearColor(1, 1, 1, 1));
		GraphicsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnGraphics);
	}
	if (SoundButton)
	{
		SoundButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		SoundButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnSound);
	}
	if (OptionsGraphicsWidgetClass)
	{
		UOptionsGraphicsWidget* OptionsGraphicsWidget = CreateWidget<UOptionsGraphicsWidget>(GetWorld(),
                                                                                            OptionsGraphicsWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsGraphicsWidget);
	}
	if (OptionsControlsWidgetClass)
	{
		UOptionsControlsWidget* OptionsControlsWidget = CreateWidget<UOptionsControlsWidget>(GetWorld(),
																							OptionsControlsWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsControlsWidget);
	}
	if (OptionsSoundWidgetClass)
	{
		UOptionsSoundWidget* OptionsSoundWidget = CreateWidget<UOptionsSoundWidget>(GetWorld(), OptionsSoundWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsSoundWidget);
	}
}

void UOptionsWidget::OnBack()
{
	UWorld* MyWorld = GetWorld();
	FString CurrentMapName = MyWorld->GetMapName();
	if (CurrentMapName.Equals("MenuLevel"))
	{
		if (MenuWidgetClass)
		{
			LeaveEvent();
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UOptionsWidget::OpenMenu, 1.0f, false, 0.125f);
		}
	}
	else
	{
		if (PauseMenuWidgetClass)
		{
			RemoveFromParent();
		}
	}	
}

void UOptionsWidget::OnControls()
{
	if (OptionsWidgetSwitcher)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(1);
	}
}

void UOptionsWidget::OnGraphics()
{
	if (OptionsWidgetSwitcher)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UOptionsWidget::OnSound()
{
	if (OptionsWidgetSwitcher)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(2);
	}
}

void UOptionsWidget::OpenMenu()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	
	UWorld* MyWorld = GetWorld();
	FString CurrentMapName = MyWorld->GetMapName();
	if (CurrentMapName.Equals("MenuLevel"))
	{
		if (MenuWidgetClass)
		{
			RemoveFromParent();
			UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
			MenuWidget->AddToViewport();
		}
	}
	else
	{
		if (PauseMenuWidgetClass)
		{
			RemoveFromParent();
			// UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), PauseMenuWidgetClass);
			// MenuWidget->AddToViewport();
		}
	}
}


