// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Miscellaneous/AnimNotify/BaseAnimNotify.h"
#include "JumpAnimNotify.generated.h"

DECLARE_DELEGATE(FOnJumpStartedSignature);

UCLASS()
class PROJECTREVIVAL_API UJumpAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	FOnJumpStartedSignature OnJumpStarted;
};
