// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/AnimNotify/BaseAnimNotify.h"
#include "DropMagazineAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API UDropMagazineAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FOnNotifiedSignature OnNotified;
};

