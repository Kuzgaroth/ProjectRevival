
// Project Revival. All Rights Reserved


#include "Menu/CreditsWidget.h"
#include "Components/Button.h"
#include "Menu/MenuWidget.h"

void UCreditsWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &UCreditsWidget::OnBack);
	}
}

void UCreditsWidget::OnBack()
{
	if (MenuWidgetClass)
	{
		RemoveFromParent();
		UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(), MenuWidgetClass);
		MenuWidget->AddToViewport();
	}
}


