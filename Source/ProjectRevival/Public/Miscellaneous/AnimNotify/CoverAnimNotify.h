// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Interfaces/ICoverable.h"
#include "CoverAnimNotify.generated.h"

DECLARE_DELEGATE_ThreeParams(FCoverDelegateSignature, ECoverType, ECoverSide, ECoverPart)

UCLASS(Blueprintable)
class PROJECTREVIVAL_API UCoverAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECoverType> CoverType;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECoverSide> CoverSide;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECoverPart> CoverPart;

	FCoverDelegateSignature CoverDelegate;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
