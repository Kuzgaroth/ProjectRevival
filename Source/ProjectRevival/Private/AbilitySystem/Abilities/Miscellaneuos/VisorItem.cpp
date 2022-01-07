// Project Revival. All Rights Reserved


#include "AbilitySystem/Abilities/Miscellaneuos/VisorItem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AVisorItem::AVisorItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	SphereDetectingVisorables = CreateDefaultSubobject<USphereComponent>(TEXT("VisorComponent"));
	SphereDetectingVisorables->InitSphereRadius(VisorRadius);
	SphereDetectingVisorables->SetCollisionProfileName(TEXT("TriggerVisor"));
	RootComponent = SphereDetectingVisorables;
	
	SphereDetectingVisorables->OnComponentBeginOverlap.AddDynamic(this, &AVisorItem::OnOverlapBeginForVisor);
	SphereDetectingVisorables->OnComponentEndOverlap.AddDynamic(this, &AVisorItem::OnOverlapEndForVisor);
}

// Called when the game starts or when spawned
void AVisorItem::BeginPlay()
{
	Super::BeginPlay();
	if (SphereDetectingVisorables->GetScaledSphereRadius() != VisorRadius)
	{
		SphereDetectingVisorables->SetSphereRadius(VisorRadius);
	}
	ToBeIgnoredByVisor.Add(this);
	ToBeIgnoredByVisor.Add(this->GetInstigator());

	if (PlayedEffect)
	{
		VisualEffect = UGameplayStatics::SpawnEmitterAtLocation(this->GetInstigator(), PlayedEffect, this->GetActorLocation(),
			FRotator::ZeroRotator, false);
	}
	
	VisorAbility();
	GetWorldTimerManager().SetTimer(TimerHandleDestroyer, this, &AVisorItem::SelfDestruction, VisorDuration, true);
}

// Called every frame
void AVisorItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVisorItem::SelfDestruction()
{
	VisorAbility();
	if (PlayedEffect)
	{
		VisualEffect->Deactivate();
		VisualEffect->DestroyComponent();
	}
	GetWorldTimerManager().ClearTimer(TimerHandleDestroyer);
	this->Destroy();
}

void AVisorItem::VisorAbility()
{
	TArray<FHitResult> OutHits;
	const FVector ActorLocation = GetActorLocation();

	const bool IsHitKismetByObj = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), ActorLocation, ActorLocation, VisorRadius,
		ObjectTypesToVisor, true, ToBeIgnoredByVisor, EDrawDebugTrace::None, OutHits, true);
	
	if (IsVisoring == false)
	{
		if(IsHitKismetByObj)
		{
			for (FHitResult& Hit : OutHits)
			{
				Hit.GetComponent()->SetRenderCustomDepth(true);
			}
		}
		IsVisoring = true;
	} else if (IsVisoring == true)
	{
		if(IsHitKismetByObj)
		{
			for (FHitResult& Hit : OutHits)
			{
				Hit.GetComponent()->SetRenderCustomDepth(false);
			}
		}
		IsVisoring = false;
	}
}

void AVisorItem::OnOverlapBeginForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsVisoring == true)
	{
		if (OtherActor && (OtherActor != this) && OtherComp && (!Cast<APlayerCharacter>(OtherActor)))
		{
			if (Cast<ACharacter>(OtherActor))
			{
				ACharacter* CharacterTmp = Cast<ACharacter>(OtherActor);
				CharacterTmp->GetMesh()->SetRenderCustomDepth(true);
			} 
			// else
			// {
			// 	OtherComp->SetRenderCustomDepth(true);
			// }
		}
	}
}

void AVisorItem::OnOverlapEndForVisor(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsVisoring == true)
	{
		if (OtherActor && OtherComp && (OtherActor != this))
		{
			if (Cast<ACharacter>(OtherActor))
			{
				ACharacter* CharacterTmp = Cast<ACharacter>(OtherActor);
				CharacterTmp->GetMesh()->SetRenderCustomDepth(false);
			} 
			// else
			// {
			// 	OtherComp->SetRenderCustomDepth(false);
			// }
		}
	}
}

void AVisorItem::SetObjectTypesToVisor(TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesForVisor)
{
	ObjectTypesToVisor = ObjectTypesForVisor;
}

// void AVisorItem::ClassNameParse()
// { 
// 	FString NameToRead = this->GetName();
// 	int32 ind;
// 	NameToRead.FindChar('_', ind);
// 	NameToRead.RemoveAt(0, ind + 1);
// 	NameToRead.FindChar('_', ind);
// 	VisorDuration = FCString::Atoi(*NameToRead.Left(ind));
// 	NameToRead.RemoveAt(0, ind + 1);
// 	NameToRead.FindChar('_', ind);
// 	VisorRadius = FCString::Atoi(*NameToRead.Left(ind));
// 	NameToRead.RemoveAt(0, ind + 1);
// }
