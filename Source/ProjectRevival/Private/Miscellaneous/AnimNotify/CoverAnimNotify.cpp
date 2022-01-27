// Project Revival. All Rights Reserved


#include "Miscellaneous/AnimNotify/CoverAnimNotify.h"
#include "PlayerCharacter.h"

void UCoverAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	const auto Player = MeshComp->GetOwner<APlayerCharacter>();
	if (Player)
	{
		Player->GetCoverData().OnCoverStatusUpdated(CoverType, CoverSide, CoverPart);
		
	}
	Super::Notify(MeshComp, Animation);
}
