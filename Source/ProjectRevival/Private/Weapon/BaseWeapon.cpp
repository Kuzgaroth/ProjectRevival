// Project Revival. All Rights Reserved

#include "Weapon/BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
//#include "EditorViewportClient.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	RootComponent = WeaponMesh;
}


void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponMesh);
	CurrentAmmo = DefaultAmmo;

	SetupDynMaterialsFromMesh(this, DynamicMaterials);
}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void ABaseWeapon::MakeShot(){}

void ABaseWeapon::StartFire(){}

void ABaseWeapon::StopFire(){}

bool ABaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.bInfiniteAmmo || IsAmmoFull() || ClipsAmount <=0) return false;
	if (IsAmmoEmpty())
	{
		CurrentAmmo.Clips = FMath::Clamp<int32>(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips+1);
		OnClipEmpty.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipsAmount = CurrentAmmo.Clips+ClipsAmount;
		if (DefaultAmmo.Clips - NextClipsAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipsAmount;
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	}
	return true;
}

TArray<UMaterialInstanceDynamic*> ABaseWeapon::GetDynMaterials()
{
	return DynamicMaterials;
}

APlayerController* ABaseWeapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	const auto Controller = Player->GetController<APlayerController>();
	return Controller;
}

bool ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto PRCharacter = Cast<ACharacter>(GetOwner());
	if (!PRCharacter) return false;

	if (PRCharacter->IsPlayerControlled())
	{
		const auto Controller = GetPlayerController();
		if (!Controller) return false;
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzelSocketName);
	}
	
	return true;
}

FVector ABaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzelSocketName);
}

bool ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;
	
	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart+ShootDirection*ShootingRange;
	return true;
}

void ABaseWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
	if (!GetWorld()) return;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
}

void ABaseWeapon::DecreaseAmmo()
{
	if (CurrentAmmo.Bullets==0 ) return;
	CurrentAmmo.Bullets--;
	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnClipEmpty.Broadcast(this);
	}
}

bool ABaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.bInfiniteAmmo && CurrentAmmo.Clips==0 && IsClipEmpty();
}

bool ABaseWeapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets==0;
}

bool ABaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

UNiagaraComponent* ABaseWeapon::SpawnMuzzelFX()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzelFX, WeaponMesh, MuzzelSocketName, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
}

void ABaseWeapon::ChangeClip()
{
	
	if (!CurrentAmmo.bInfiniteAmmo)
	{
		if (CurrentAmmo.Clips==0 ) return;
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets<DefaultAmmo.Bullets && CurrentAmmo.Clips >0;
}


