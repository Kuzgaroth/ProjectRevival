// Project Revival. All Rights Reserved


#include "Weapon/SoldierRifleWeapon.h"
#include "Weapon/Components/WeaponFXComponent.h"
#include "NiagaraComponent.h"
#include "GameFramework/GameModeBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Interfaces/ISaveLoader.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

DEFINE_LOG_CATEGORY(LogPRAISoldierRifle)

ASoldierRifleWeapon::ASoldierRifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");
}

void ASoldierRifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	InitFX();
	check(WeaponFXComponent);

	const auto SaveLoader = Cast<IISaveLoader>(GetWorld()->GetAuthGameMode());
	if (SaveLoader)
	{
		const auto SaveGame = SaveLoader->GetSaveFromLoader();
		if (SaveGame)
		{
			const auto Difficulty = SaveGame->GameDifficulty;
			switch (Difficulty)
			{
			case Normal:
				ShotDamage = 25.f;
				break;
			case Hard:
				ShotDamage = 35.f;
				break;
			default:
					break;
			}
		}
	}
}

void ASoldierRifleWeapon::StartFire()
{
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: StartFire() was called"), *GetName())
	CurrentBurstRow = BurstClipsNumber;
	CurrentRowTime = BurstBulletsDelay * BurstBulletsNumber + BurstClipsDelay;
	UE_LOG(LogPRAISoldier, Log, TEXT("%s: StartFire OneRowTime is: %f"), *GetName(), CurrentRowTime)
	GetWorld()->GetTimerManager().SetTimer(ClipsTimerHandle, this, &ASoldierRifleWeapon::ShootRowInternal, CurrentRowTime, true, 0.f);
}

void ASoldierRifleWeapon::ShootRowInternal()
{
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: ShootRowInternal() was called"), *GetName())
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: CurrentBurstRow is: %d"), *GetName(), CurrentBurstRow)
	PlayFX(false);
	--CurrentBurstRow;
	if (CurrentBurstRow < 0)
	{
		StopFire();
		return;
	}
	CurrentBurstShot = BurstBulletsNumber;
	GetWorld()->GetTimerManager().SetTimer(BulletsTimerHandle, this, &ASoldierRifleWeapon::MakeShotInternal, BurstBulletsDelay, true, 0.f);
}

void ASoldierRifleWeapon::MakeShotInternal()
{
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: MakeShotInternal() was called"), *GetName())
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: Current Burst Shot: %d"), *GetName(), CurrentBurstShot);
	PlayFX(false);
	--CurrentBurstShot;
	if (CurrentBurstShot < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);
		return;
	}
	MakeShot();
}

void ASoldierRifleWeapon::StopFire()
{
	PlayFX(false);
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: StopFire() was called. It's Owner is: %s"), *GetName(), *GetOwner()->GetName());
	if (GetWorld()->GetTimerManager().IsTimerActive(BulletsTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
	StoppedFireInWeaponDelegate.Broadcast();
}

void ASoldierRifleWeapon::StopFireExternal()
{
	PlayFX(false);
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: StopFireExternal() was called. It's Owner is: %s"), *GetName(), *GetOwner()->GetName());
	if (GetWorld()->GetTimerManager().IsTimerActive(BulletsTimerHandle)) {GetWorld()->GetTimerManager().ClearTimer(BulletsTimerHandle);}
	GetWorld()->GetTimerManager().ClearTimer(ClipsTimerHandle);
}

void ASoldierRifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}
	PlaySound();
	PlayFX(true);
	
	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult,TraceStart, TraceEnd);

	if (HitResult.bBlockingHit)
	{
		TraceEnd = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceEnd);
	
	if (!CurrentAmmo.bInfiniteAmmo) DecreaseAmmo();
	if (OnWeaponShotDelegate.IsBound()) OnWeaponShotDelegate.Broadcast();
	UE_LOG(LogPRAISoldierRifle, Log, TEXT("%s: MakeShot() was finished"), *GetName())
}

void ASoldierRifleWeapon::MakeHit(FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
	if (!GetWorld()) return;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.bReturnPhysicalMaterial = true;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, CollisionQueryParams);
}

bool ASoldierRifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	TraceStart = GetMuzzleWorldLocation();
	const FVector ShootDirection = FMath::VRandCone(WeaponMesh->GetSocketRotation(MuzzelSocketName).Vector(), FMath::DegreesToRadians(BulletSpread));
	TraceEnd = TraceStart + ShootDirection * ShootingRange;
	return true;
}

void ASoldierRifleWeapon::MakeDamage(FHitResult& HitResult)
{
	AActor* DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;
	DamagedActor->TakeDamage(ShotDamage,FDamageEvent{},GetController(),this);
}

void ASoldierRifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (bUseNiagaraTraceEffect == true)
	{
		const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFXNiagara, TraceStart);
		if (TraceFXComponent)
		{
			TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
		}
	}
	else
	{
		const auto TraceFXComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceFXCascade, TraceStart);
		if (TraceFXComponent)
		{
			TraceFXComponent->SetBeamEndPoint(0, TraceEnd);
		}
	}
}

void ASoldierRifleWeapon::InitFX()
{
	if (bUseNiagaraMuzzleEffect)
	{
		MuzzleFXComponentNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFXNiagara, WeaponMesh, MuzzelSocketName, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		if (MuzzleFXComponentNiagara)
		{
			MuzzleFXComponentNiagara->SetPaused(false);
			MuzzleFXComponentNiagara->SetVisibility(false, true);
		}
	}
	else
	{
		MuzzleFXComponentCascade = UGameplayStatics::SpawnEmitterAttached(MuzzleFXCascade, WeaponMesh, MuzzelSocketName, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		if (MuzzleFXComponentCascade)
		{
			MuzzleFXComponentCascade->SetVisibility(false, true);
		}
	}
}

void ASoldierRifleWeapon::PlayFX(bool bActivate)
{
	if (bUseNiagaraMuzzleEffect)
	{
		if (MuzzleFXComponentNiagara)
		{
			MuzzleFXComponentNiagara->SetPaused(!bActivate);
			MuzzleFXComponentNiagara->SetVisibility(bActivate, true);
		}
		else
		{
			if (bActivate) MuzzleFXComponentNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFXNiagara, WeaponMesh, MuzzelSocketName, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		}
	}
	else
	{
		if (MuzzleFXComponentCascade)
		{
			MuzzleFXComponentCascade->SetVisibility(bActivate, true);
		}
		else
		{
			if (bActivate) MuzzleFXComponentCascade = UGameplayStatics::SpawnEmitterAttached(MuzzleFXCascade, WeaponMesh, MuzzelSocketName, FVector::ZeroVector,
		FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false);
		}
	}
}


void ASoldierRifleWeapon::PlaySound()
{
	UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzelSocketName);
}

AController* ASoldierRifleWeapon::GetController() const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}