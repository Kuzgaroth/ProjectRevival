// Project Revival. All Rights Reserved


#include "Miscellaneous/AnimNotify/CoverAnimNotify.h"

void UCoverAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CoverDelegate.ExecuteIfBound(CoverType.GetValue(), CoverSide.GetValue(), CoverPart.GetValue());
	Super::Notify(MeshComp, Animation);
}
