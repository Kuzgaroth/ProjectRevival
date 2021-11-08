// Project Revival. All Rights Reserved


#include "Menu/OptionsWidget.h"
#include "Components/Button.h"
#include "Menu/OptionsControlsWidget.h"
#include "Menu/OptionsGraphicsWidget.h"
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
		ControlsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnControls);
	}
	if (GraphicsButton)
	{
		GraphicsButton->OnClicked.AddDynamic(this, &UOptionsWidget::OnGraphics);
	}
	if (OptionsControlsWidgetClass)
	{
		UOptionsControlsWidget* OptionsControlsWidget = CreateWidget<UOptionsControlsWidget>(GetWorld(), OptionsControlsWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsControlsWidget);
	}
	if (OptionsGraphicsWidgetClass)
	{
		UOptionsGraphicsWidget* OptionsGraphicsWidget = CreateWidget<UOptionsGraphicsWidget>(GetWorld(), OptionsGraphicsWidgetClass);
		OptionsWidgetSwitcher->AddChild(OptionsGraphicsWidget);
	}
}

void UOptionsWidget::OnBack()
{
	if (MenuWidgetClass)
	{
		RemoveFromParent();
		UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		MenuWidget->AddToViewport();
	}
}

void UOptionsWidget::OnControls()
{
	if (OptionsControlsWidgetClass)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(0);
	}
}

void UOptionsWidget::OnGraphics()
{
	if (OptionsGraphicsWidgetClass)
	{
		OptionsWidgetSwitcher->SetActiveWidgetIndex(1);
	}
}


