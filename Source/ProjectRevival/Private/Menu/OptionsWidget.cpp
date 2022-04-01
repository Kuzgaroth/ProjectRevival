// Project Revival. All Rights Reserved


#include "Menu/OptionsWidget.h"

#include "PauseWidget.h"
#include "Components/Button.h"
#include "Menu/OptionsControlsWidget.h"
#include "Menu/OptionsGameWidget.h"
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
	if (GameButton)
	{
		GameButton->SetBackgroundColor(FLinearColor(1, 1, 1, 0));
		GameButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnGame);
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
	if (OptionsGameWidgetClass)
	{
		UOptionsGameWidget* OptionsGameWidget = CreateWidget<UOptionsGameWidget>(GetWorld(), OptionsGameWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsGameWidget);
	}
}

void UOptionsWidget::OnBack()
{
	if (CheckUnsavedChanges())
	{
		if (ConfirmationWidgetClass)
		{
			ConfirmationWidget = CreateWidget<UConfirmationWidget>(GetWorld(), ConfirmationWidgetClass);
			ConfirmationWidget->ConfirmButton->OnClicked.AddDynamic(this, &UOptionsWidget::ApplyAllChanges);
			ConfirmationWidget->DeclineButton->OnClicked.AddDynamic(this, &UOptionsWidget::CloseConfirmationWidget);
			ConfirmationWidget->SetLabelText(FText::FromString("Do you want to save your changes?"));
			ConfirmationWidget->AddToViewport();
		}
	}
	else
	{
		ChooseLevel();
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

void UOptionsWidget::OnGame()
{
	if (OptionsWidgetSwitcher)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(3);
	}
}

void UOptionsWidget::ChooseLevel()
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

bool UOptionsWidget::CheckUnsavedChanges() const
{
	if (Cast<UOptionsGraphicsWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(0))->HasUnsavedChanges())
	{
		return true;
	}
	if (Cast<UOptionsSoundWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(2))->HasUnsavedChanges())
	{
		return true;
	}
	if (Cast<UOptionsGameWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(3))->HasUnsavedChanges())
	{
		return true;
	}

	return false;
}

void UOptionsWidget::ApplyAllChanges()
{
	if (OptionsWidgetSwitcher)
	{
		Cast<UOptionsGraphicsWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(0))->ApplyChanges();
		Cast<UOptionsSoundWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(2))->ApplyChanges();
		Cast<UOptionsGameWidget>(OptionsWidgetSwitcher->GetWidgetAtIndex(3))->ApplyChanges();	
	}
	
	CloseConfirmationWidget();
}

void UOptionsWidget::CloseConfirmationWidget()
{
	if (ConfirmationWidget)
	{
		ConfirmationWidget->RemoveFromParent();
	}
	ChooseLevel();
}
