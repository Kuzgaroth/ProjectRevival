// Project Revival. All Rights Reserved


#include "UI/GameDataWidget.h"
#include "PRGameModeBase.h"
#include "PRPlayerState.h"

APRGameModeBase* UGameDataWidget::GetGameMode() const
{
	return GetWorld() ? Cast<APRGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

APRPlayerState* UGameDataWidget::GetPlayerState() const 
{
	return GetOwningPlayer() ? Cast<APRPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
