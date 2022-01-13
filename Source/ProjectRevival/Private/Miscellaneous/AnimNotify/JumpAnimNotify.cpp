// Project Revival. All Rights Reserved


#include "Miscellaneous/AnimNotify/JumpAnimNotify.h"

void UJumpAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnJumpStarted.ExecuteIfBound();
	OnJumpStarted.Unbind();
	Super::Notify(MeshComp, Animation);
}
