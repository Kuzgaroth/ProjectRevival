// Project Revival. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Sound/SoundCue.h"
#include "SoundSurfaceNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTREVIVAL_API USoundSurfaceNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, Category="SoundNotify")
	float TraceDistance = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoundNotify")
	USoundCue* DefaultSoundCue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SoundNotify")
	TMap<UPhysicalMaterial*, USoundCue*> SoundNotifyMap;

};
