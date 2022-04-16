// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/Miscellaneuos/VisorItemAttached.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AVisorItemAttached::AVisorItemAttached()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SphereDetectingVisorables = CreateDefaultSubobject<USphereComponent>(TEXT("VisorComponent"));
	SphereDetectingVisorables->InitSphereRadius(VisorRadius);
	SphereDetectingVisorables->SetCollisionProfileName(TEXT("TriggerVisor"));
	RootComponent = SphereDetectingVisorables;
	
	SphereDetectingVisorables->OnComponentBeginOverlap.AddDynamic(this, &AVisorItemAttached::OnOverlapBeginForVisor);
	SphereDetectingVisorables->OnComponentEndOverlap.AddDynamic(this, &AVisorItemAttached::OnOverlapEndForVisor);
}

// Called when the game starts or when spawned
void AVisorItemAttached::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("ItemAttached: BeginPlay() was called"))
	if (SphereDetectingVisorables->GetScaledSphereRadius() != VisorRadius)
	{
		SphereDetectingVisorables->SetSphereRadius(VisorRadius);
	}
	ToBeIgnoredByVisor.Add(this);
	ToBeIgnoredByVisor.Add(this->GetInstigator());

	if (PlayedEffect)
	{
		VisualEffect = UGameplayStatics::SpawnEmitterAttached(PlayedEffect, SphereDetectingVisorables, "EffectPoint", FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
	}
	VisorAbility();
}

// Called every frame
void AVisorItemAttached::Tick(float DeltaTime)
{
	if (VisualEffect)
	{
		if (VisualEffect->bWasCompleted)
		{
			VisualEffect->Activate();
			UE_LOG(LogTemp, Log, TEXT("ItemAttached: VisualEffect->Activate() was called"))
		}
	}
	Super::Tick(DeltaTime);
}

void AVisorItemAttached::StartDestructionCountdown()
{
	UE_LOG(LogTemp, Log, TEXT("ItemAttached: StartDestructionCountdown() was called"))
	GetWorldTimerManager().SetTimer(TimerHandleDestroyer, this, &AVisorItemAttached::SelfDestruction, TurnOffDelay, false);
}

void AVisorItemAttached::SelfDestruction()
{
	UE_LOG(LogTemp, Log, TEXT("ItemAttached: SelfDestruction() was called"))
	VisorAbility();
	if (VisualEffect)
	{
		VisualEffect->Deactivate();
		VisualEffect->DestroyComponent();
	}
	GetWorldTimerManager().ClearTimer(TimerHandleDestroyer);
	this->Destroy();
}

void AVisorItemAttached::VisorAbility()
{
	UE_LOG(LogTemp, Log, TEXT("ItemAttached: VisorAbility() was called"))
	TArray<FHitResult> OutHits;
	FVector StartTrace = GetActorLocation();
	FVector EndTrace = StartTrace + FVector(0.f, 0.f, 0.1f);
	
	bool IsHitKismetByObj = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), StartTrace, EndTrace, VisorRadius,
		ObjectTypesToVisor, true, ToBeIgnoredByVisor, EDrawDebugTrace::ForDuration, OutHits, true, FLinearColor::Yellow, FLinearColor::Blue, 1.0f);
	
	if (IsVisoring == false)
	{
		if(IsHitKismetByObj)
		{
			for (FHitResult& Hit : OutHits)
			{
				if (Cast<AAICharacter>(Hit.GetActor()))
				{
					Cast<AAICharacter>(Hit.GetActor())->GetMesh()->SetRenderCustomDepth(true);
					Cast<AAICharacter>(Hit.GetActor())->GetMesh()->SetCustomDepthStencilValue(1);
					// AAICharacter* AIChar = Cast<AAICharacter>(Hit.GetActor());
					// TArray<UMaterialInterface*> TempMaterialArr;
					// int32 TotalNumMaterials = AIChar->GetMesh()->GetNumMaterials();
					// if(TotalNumMaterials > 0)
					// {
					// 	TempMaterialArr.AddZeroed(TotalNumMaterials);
					// 	for(int32 MaterialIndex=0; MaterialIndex < TotalNumMaterials; ++MaterialIndex)
					// 	{
					// 		TempMaterialArr[MaterialIndex] = AIChar->GetMesh()->GetMaterial(MaterialIndex);
					// 		AIChar->GetMesh()->SetMaterial(MaterialIndex, ReplaceMaterial);
					// 	}
					// }
					// OriginalMaterials.Add(AIChar->GetName(), TempMaterialArr);
				}
				//else if (Cast<AStaticObjectToNothing>(Hit.GetActor()) && Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent()))
				//{
				//	Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent())->SetRenderCustomDepth(true);
				//	Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent())->SetCustomDepthStencilValue(2);
				//}
				else if(Hit.GetComponent())
				{
					Hit.GetComponent()->SetRenderCustomDepth(true);
					Hit.GetComponent()->SetCustomDepthStencilValue(0);
				}
			}
		}
		IsVisoring = true;
	} 
	//else if (IsVisoring == true)
	//{
	//	if(IsHitKismetByObj)
	//	{
	//		for (FHitResult& Hit : OutHits)
	//		{
	//			if (Cast<AAICharacter>(Hit.GetActor()))
	//			{
	//				Cast<AAICharacter>(Hit.GetActor())->GetMesh()->SetRenderCustomDepth(false);
					// if (OriginalMaterials.Contains(Hit.GetActor()->GetName()))
					// {
					// 	AAICharacter* AIChar = Cast<AAICharacter>(Hit.GetActor());
					// 	int32 TotalNumMaterials = AIChar->GetMesh()->GetNumMaterials();
					// 	if(TotalNumMaterials > 0)
					// 	{
					// 		TArray<UMaterialInterface*> TempMaterialArr;
					// 		TempMaterialArr.Append(*OriginalMaterials.Find(Hit.GetActor()->GetName()));
					// 		for(int32 MaterialIndex=0; MaterialIndex < TotalNumMaterials; ++MaterialIndex)
					// 		{
					// 			AIChar->GetMesh()->SetMaterial(MaterialIndex, TempMaterialArr[MaterialIndex]);
					// 		}
					// 	}
					// }
					// OriginalMaterials.Remove(Hit.GetActor()->GetName());
	//			}
	//			else if (Cast<AStaticObjectToNothing>(Hit.GetActor()) && Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent()))
	//			{
	//				Cast<UStaticMeshComponent>(Hit.GetActor()->GetRootComponent())->SetRenderCustomDepth(false);
	//			}
	//			else if(Hit.GetComponent())
	//			{
	//				Hit.GetComponent()->SetRenderCustomDepth(false);
	//			}
	//		}
	//	}
	//	IsVisoring = false;
	//}
}

void AVisorItemAttached::OnOverlapBeginForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsVisoring == true)
	{
		if (OtherActor && (OtherActor != this) && OtherComp && (!Cast<APlayerCharacter>(OtherActor)))
		{
			if (Cast<AAICharacter>(OtherActor))
			{
				Cast<AAICharacter>(OtherActor)->GetMesh()->SetRenderCustomDepth(true);
				Cast<AAICharacter>(OtherActor)->GetMesh()->SetCustomDepthStencilValue(1);
				// AAICharacter* AIChar = Cast<AAICharacter>(OtherActor);
				// TArray<UMaterialInterface*> TempMaterialArr;
				// int32 TotalNumMaterials = AIChar->GetMesh()->GetNumMaterials();
				// if(TotalNumMaterials > 0)
				// {
				// 	TempMaterialArr.AddZeroed(TotalNumMaterials);
				// 	for(int32 MaterialIndex=0; MaterialIndex < TotalNumMaterials; ++MaterialIndex)
				// 	{
				// 		TempMaterialArr[MaterialIndex] = AIChar->GetMesh()->GetMaterial(MaterialIndex);
				// 		AIChar->GetMesh()->SetMaterial(MaterialIndex, ReplaceMaterial);
				// 	}
				// }
				// OriginalMaterials.Add(AIChar->GetName(), TempMaterialArr);
			}
			// else if (Cast<AStaticObjectToNothing>(OtherActor) && Cast<UStaticMeshComponent>(OtherActor->GetRootComponent()))
			// {
			// 	Cast<UStaticMeshComponent>(OtherActor->GetRootComponent())->SetRenderCustomDepth(true);
			// 	Cast<UStaticMeshComponent>(OtherActor->GetRootComponent())->SetCustomDepthStencilValue(2);
			// }
			else if(OtherComp)
			{
				OtherComp->SetRenderCustomDepth(true);
				OtherComp->SetCustomDepthStencilValue(0);
			}
		}
	}
}

void AVisorItemAttached::OnOverlapEndForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsVisoring == true)
	{
		if (OtherActor && OtherComp && (OtherActor != this))
		{
			if (Cast<AAICharacter>(OtherActor))
			{
				Cast<AAICharacter>(OtherActor)->GetMesh()->SetRenderCustomDepth(false);
				// if (OriginalMaterials.Contains(OtherActor->GetName()))
				// {
				// 	AAICharacter* AIChar = Cast<AAICharacter>(OtherActor);
				// 	int32 TotalNumMaterials = AIChar->GetMesh()->GetNumMaterials();
				// 	if(TotalNumMaterials > 0)
				// 	{
				// 		TArray<UMaterialInterface*> TempMaterialArr;
				// 		TempMaterialArr.Append(*OriginalMaterials.Find(OtherActor->GetName()));
				// 		for(int32 MaterialIndex=0; MaterialIndex < TotalNumMaterials; ++MaterialIndex)
				// 		{
				// 			AIChar->GetMesh()->SetMaterial(MaterialIndex, TempMaterialArr[MaterialIndex]);
				// 		}
				// 	}
				// }
				// OriginalMaterials.Remove(OtherActor->GetName());
			}
			//else if (Cast<AStaticObjectToNothing>(OtherActor) && Cast<UStaticMeshComponent>(OtherActor->GetRootComponent()))
			//{
			//	Cast<UStaticMeshComponent>(OtherActor->GetRootComponent())->SetRenderCustomDepth(false);
			//}
			//else if(OtherComp)
			//{
			//	OtherComp->SetRenderCustomDepth(false);
			//}
		}
	}
}

void AVisorItemAttached::SetObjectTypesToVisor(TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisor)
{
	ObjectTypesToVisor = ObjectTypesForVisor;
}

void AVisorItemAttached::SetReplaceMaterial(class UMaterial* MaterialToSet)
{
	// ReplaceMaterial = MaterialToSet;
}