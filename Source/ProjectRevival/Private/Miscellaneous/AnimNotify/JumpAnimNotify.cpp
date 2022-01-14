// Project Revival. All Rights Reserved


#include "Miscellaneous/AnimNotify/JumpAnimNotify.h"

void UJumpAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnActionPointReached.ExecuteIfBound();
	OnActionPointReached.Unbind();
	Super::Notify(MeshComp, Animation);
}
