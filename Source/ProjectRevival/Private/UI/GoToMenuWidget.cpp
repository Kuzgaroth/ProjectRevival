// Project Revival. All Rights Reserved


#include "UI/GoToMenuWidget.h"

#include "PRGameInstance.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGoToMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GoToMenuButton)
	{
		GoToMenuButton->OnClicked.AddDynamic(this, &UGoToMenuWidget::OnGoToMenu);
	}
}

void UGoToMenuWidget::OnGoToMenu()
{
	const auto GameInstance = GetWorld()->GetGameInstance<UPRGameInstance>();
	
	if (GameInstance->GetMenuLevelName().IsNone()) return;

	UGameplayStatics::OpenLevel(this,GameInstance->GetMenuLevelName());
}
