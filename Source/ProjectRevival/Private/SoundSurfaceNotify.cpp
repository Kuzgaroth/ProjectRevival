// Project Revival. All Rights Reserved


#include "SoundSurfaceNotify.h"

#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

void USoundSurfaceNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	auto SoundData = DefaultSoundCue;
	const FVector StartLoc = MeshComp->GetOwner()->GetActorLocation();
	const FVector EndLoc = MeshComp->GetUpVector().DownVector * TraceDistance + StartLoc;
	FHitResult Hit;

	const TArray<AActor*> ActorsToIgnore;
	UKismetSystemLibrary::LineTraceSingle(MeshComp, StartLoc, EndLoc, UEngineTypes::ConvertToTraceType(ECC_Camera),
		false, ActorsToIgnore, EDrawDebugTrace::ForDuration,
		Hit, true, FLinearColor::Yellow, FLinearColor::White, 5.0f);
	
	if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMaterial = Hit.PhysMaterial.Get();
		if (SoundNotifyMap.Contains(PhysMaterial))
		{
			SoundData = SoundNotifyMap[PhysMaterial];
		}
		UGameplayStatics::SpawnSoundAttached(SoundData, MeshComp, FName("CameraSocket"), Hit.ImpactPoint, Hit.ImpactPoint.Rotation(),EAttachLocation::KeepWorldPosition);
	}
	
}
