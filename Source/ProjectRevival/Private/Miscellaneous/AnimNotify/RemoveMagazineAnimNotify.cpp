// Project Revival. All Rights Reserved


#include "Miscellaneous/AnimNotify/RemoveMagazineAnimNotify.h"
#include "AKWeapon.h"
#include "BaseCharacter.h"

void URemoveMagazineAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!MeshComp || !MeshComp->GetOwner()) return;
	
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if(!Character) return;
	
	AKWeapon* Weapon = Cast<AKWeapon>(Character->GetWeaponComponent()->GetCurrentWeapon());
	if(!Weapon) return;
	
	Super::Notify(MeshComp, Animation);
	//Weapon->Remove();
}
