// Project Revival. All Rights Reserved


#include "UI/SpectatorWidget.h"
#include "PRUtils.h"
#include "RespawnComponent.h"

bool USpectatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = PRUtils::GetCharacterComponent<URespawnComponent>(GetOwningPlayer());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountdown();
	return true;
}
