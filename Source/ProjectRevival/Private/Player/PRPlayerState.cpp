// Project Revival. All Rights Reserved


#include "Player/PRPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogPRPlayerState, All, All)

void APRPlayerState::LogInfo()
{
	UE_LOG(LogPRPlayerState, Display, TEXT("TeamId: %i, Kills: %i, Deaths: %i"), TeamId, KillsNum, DeathsNum);
}
