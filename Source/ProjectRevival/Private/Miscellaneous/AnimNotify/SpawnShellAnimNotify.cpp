// Project Revival. All Rights Reserved


#include "SpawnShellAnimNotify.h"
#include "AKWeapon.h"
#include "BaseCharacter.h"

void USpawnShellAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(!MeshComp || !MeshComp->GetOwner()) return;
	
	ABaseCharacter* Character = Cast<ABaseCharacter>(MeshComp->GetOwner());
	if(!Character) return;
	
	AKWeapon* Weapon = Cast<AKWeapon>(Character->GetWeaponComponent()->GetCurrentWeapon());
	if(!Weapon) return;
	
	Super::Notify(MeshComp, Animation);
	Weapon->SpawnShell("gate");
	
	UE_LOG(LogActor,Error,TEXT("USpawnShellAnimNotify"));
}