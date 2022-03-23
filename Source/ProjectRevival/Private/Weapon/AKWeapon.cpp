// Project Revival. All Rights Reserved

#include "AKWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "Weapon/Components/WeaponFXComponent.h"

AKWeapon::AKWeapon()
{
	if(WeaponMesh)
	{
		RootComponent = WeaponMesh;
		WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//FRotator Rotation = WeaponMesh->GetRelativeRotation(); Rotation.Roll+=90;
		//WeaponMesh->SetRelativeRotation(Rotation);
	}
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AKWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKWeapon::MakeShot()
{
	UE_LOG(LogCustom, Error, TEXT("MakeShot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	//if(MuzzleFXComponentCascade)
	//{
	//	MuzzleFXComponentCascade->Activate();
	//}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	if (OnWeaponShotDelegate.IsBound()) OnWeaponShotDelegate.Broadcast();
	PlayForceEffects();
	DecreaseAmmo();
	SpawnShell();
}

void AKWeapon::AttachMagazine(ABaseWeapon* Weapon, USceneComponent* CharacterMesh, const FName& SocketName)
{
	UE_LOG(LogCustom, Warning, TEXT("AttachMagazineToArm"));
	if (!Weapon || !CharacterMesh) return;
	
	const FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Magazine->AttachToComponent(CharacterMesh, AttachmentTransformRules, SocketName);
}

void AKWeapon::DetachMagazine()
{
	UE_LOG(LogCustom, Warning, TEXT("DetachMagazine"));
	Magazine->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//Magazine->Destroy();	
}

void AKWeapon::SpawnShell()
{
	UE_LOG(LogCustom, Warning, TEXT("SpawnShell"));
	const FRotator Rotation = this->GetActorRotation();
	const FVector Location = WeaponMesh->GetSocketLocation(ShutterSocketName);
	GetWorld()->SpawnActor<AShell>(AmmoShell, Location, Rotation);
	//if(Shell && AttachPoint)
	/*if(Shell)
	{
		UE_LOG(LogCustom, Warning, TEXT("Shell"));
		//Shell->SetActorLocation(AttachPoint->GetComponentLocation());
		//Shell->SetActorRotation(AttachPoint->GetComponentRotation());
		Shell->SetActorLocation(this->GetActorLocation());
		Shell->SetActorRotation(this->GetActorRotation());
	}*/
}
