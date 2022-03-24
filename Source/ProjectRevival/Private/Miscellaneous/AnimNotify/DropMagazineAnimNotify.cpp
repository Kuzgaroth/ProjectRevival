// Project Revival. All Rights Reserved


#include "DropMagazineAnimNotify.h"

void UDropMagazineAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		Super::Notify(MeshComp, Animation);
		AActor* Actor = Cast<AActor>(MeshComp->GetOwner());
		if(Actor != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Red,__FUNCTION__);
		}
	}
}