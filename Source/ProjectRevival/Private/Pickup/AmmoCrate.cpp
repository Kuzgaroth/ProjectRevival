// Project Revival. All Rights Reserved


#include "AmmoCrate.h"

#include "AmmoWidget.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/HealthComponent.h"
#include "ProjectRevival/ProjectRevival.h"
#include "HealthBarWidget.h"
#include "AbilitySystem/AbilityActors/ChangeWorldSphereActor.h"
#include "GameFramework/GameModeBase.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/ISaveLoader.h"
#include "ProjectRevival/Public/Miscellaneous/PRUtils.h"

AAmmoCrate::AAmmoCrate()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("WidgetComponent");
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComponent->SetDrawAtDesiredSize(true);
	
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(100.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Trigger"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	CollisionComponent->SetupAttachment(RootComponent);
	

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::OnOverlapBegin);
	MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AAmmoCrate::ChangeWorld);
}

void AAmmoCrate::ChangeWorld(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AChangeWorldSphereActor>())
	{
		CurrentWorld = (CurrentWorld == OrdinaryWorld) ? OtherWorld : OrdinaryWorld;
		SetCrateVisibility(CurrentWorld==AmmoWorld);
	}
}

void AAmmoCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateWidgetVisibility();
}

void AAmmoCrate::BeginPlay()
{
	Super::BeginPlay();

	CollisionContainer = MeshComponent->GetCollisionResponseToChannels();
	
	check(WidgetComponent);
	int32 Clips=MaxClipsAmount;
	const auto SaveLoader = Cast<IISaveLoader>(GetWorld()->GetAuthGameMode());
	if (SaveLoader)
	{
		const auto SaveObject = SaveLoader->GetSaveFromLoader();
		if (SaveObject)
		{
			CurrentWorld = SaveObject->WorldNum;
			if (SaveObject->AmmoCrates.Contains(GetFName()))
			{
				const auto Data = SaveObject->AmmoCrates[GetFName()];
				Clips = Data.CurrentClips;
			}
		}
		else
		{
			CurrentWorld = OrdinaryWorld;
			
		}
	}
	CurrentClipsAmount = Clips;
	SetCrateVisibility(CurrentWorld==AmmoWorld);
	UpdateWidgetPercent();
}

void AAmmoCrate::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsInvisible) return;
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		const auto Pawn = Cast<APawn>(OtherActor);
		if (GivePickUpTo(Pawn))
		{
			UE_LOG(LogPlayerCharacter, Warning, TEXT("Pickup was taken"));
		}
		else UE_LOG(LogPlayerCharacter, Warning, TEXT("Pickup failed"));
	}
	UE_LOG(LogPlayerCharacter, Warning, TEXT("Clips remain: %d"), CurrentClipsAmount);
}

bool AAmmoCrate::GivePickUpTo(APawn* PlayerPawn)
{
	const auto HealthComponent = PRUtils::GetCharacterComponent<UHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead()) return false;
	
	const auto WeaponComponent = PRUtils::GetCharacterComponent<UWeaponComponent>(PlayerPawn);
	if(WeaponComponent)
	{
		const bool bIsPickupNecessary = !((WeaponComponent->GetCurrentWeaponBullets() == WeaponComponent->GetMaxWeaponBullets())
					&& (WeaponComponent->GetCurrentWeaponClips() == WeaponComponent->GetMaxWeaponClips()));
	
		if (CurrentClipsAmount > 0 && bIsPickupNecessary)
		{
			//количество недостающих обойм
			int32 TempClipsNeeded = WeaponComponent->GetMaxWeaponClips() - WeaponComponent->GetCurrentWeaponClips() + 1;
			if(TempClipsNeeded > CurrentClipsAmount)
			{
				TempClipsNeeded = CurrentClipsAmount;
				CurrentClipsAmount = 0;
			}
			else
			{
				CurrentClipsAmount = CurrentClipsAmount - TempClipsNeeded;
			}
			UpdateWidgetPercent();
			return (WeaponComponent->TryToAddAmmo(WeaponType, TempClipsNeeded));
		}
		else
		{
			UE_LOG(LogPlayerCharacter, Warning, TEXT("PbIsPickupNecessary"));
			return false;
		}
	}
	else
	{
		return false;
	}
}

float AAmmoCrate::GetAmmoCratePercent() const
{
	return CurrentClipsAmount / MaxClipsAmount;
}

void AAmmoCrate::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// call BP handler
	Super::NotifyActorBeginOverlap(OtherActor);
}

void AAmmoCrate::UpdateWidgetVisibility()
{
	if (IsInvisible) return;
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();
	DistanceToAmmo = FVector::Distance(PlayerLocation, this->GetActorLocation());
	WidgetComponent->SetVisibility(DistanceToAmmo < VisibilityDistance, false);
}

void AAmmoCrate::UpdateWidgetPercent()
{
	const auto AmmoWidget = Cast<UAmmoWidget>(WidgetComponent->GetUserWidgetObject());
	AmmoWidget->SetAmmoAmount(CurrentClipsAmount);
}

void AAmmoCrate::SetCrateVisibility(bool NewVisibility)
{
	IsInvisible = !NewVisibility;
	WidgetComponent->SetVisibility(NewVisibility);
	UpdateWidgetVisibility();
	MeshComponent->SetVisibility(NewVisibility);
	MeshComponent->SetCollisionResponseToChannels(NewVisibility ? CollisionContainer : FCollisionResponseContainer(ECollisionResponse::ECR_Overlap));
}
