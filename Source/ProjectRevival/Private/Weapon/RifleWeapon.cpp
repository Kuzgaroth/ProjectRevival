// Project Revival. All Rights Reserved


#include "Weapon/RifleWeapon.h"

#include "AICharacter.h"
#include "BasePlayerController.h"
#include "Weapon/Components/WeaponFXComponent.h"
#include "DrawDebugHelpers.h"
#include "GameHUD.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerHUDWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void ARifleWeapon::MakeShot()
{
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart;
	FVector TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFXEnd = TraceEnd;
	if (HitResult.bBlockingHit)
	{
		TraceFXEnd = HitResult.ImpactPoint;
		//DrawDebugLine(GetWorld(),GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 3.0f, 0, 3.0f);
		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFX(HitResult);
	}
	if (MuzzleFXComponentCascade)
	{
		MuzzleFXComponentCascade->Activate();
	}
	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFXEnd);
	if (OnWeaponShotDelegate.IsBound()) OnWeaponShotDelegate.Broadcast();
	PlayForceEffects();
	DecreaseAmmo();
}

bool ARifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread);
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
	//SocketTransform.GetRotation().GetForwardVector();
	TraceEnd = TraceStart + ShootDirection * ShootingRange;
	return true;
}

ARifleWeapon::ARifleWeapon()
{
	WeaponFXComponent = CreateDefaultSubobject<UWeaponFXComponent>("WeaponFXComponent");
}

void ARifleWeapon::StartFire()
{
	InitFX();
	GetWorld()->GetTimerManager().SetTimer(ShotTimerHandle, this, &ARifleWeapon::MakeShot, TimeBetweenShots, true);
	MakeShot();
}

void ARifleWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotTimerHandle);
	SetFXActive(false);
}

void ARifleWeapon::MakeDamage(FHitResult& HitResult)
{
	const auto DamagedActor = HitResult.GetActor();
	if (!DamagedActor) return;

	float Damage = ShotDamage;
	if (DamagedActor->IsA<AAICharacter>() && BodyMaterialMap.Contains(HitResult.PhysMaterial.Get()))
	{
		ProcessEnemyHit(HitResult);
		Damage = IsHitInHead(HitResult.PhysMaterial.Get()) ? ShotDamage*10.f : ShotDamage;
	}
	
	DamagedActor->TakeDamage(Damage, FDamageEvent{}, GetController(), this);
}

bool ARifleWeapon::IsHitInHead(const UPhysicalMaterial* PhysMaterial)
{
	const auto BodyPart = BodyMaterialMap[PhysMaterial].GetValue();
	return BodyPart==EBodyPart::Head;
}

void ARifleWeapon::ProcessEnemyHit(const FHitResult& HitResult)
{
	//UI logic stuff
	
	const auto BodyPart = BodyMaterialMap[HitResult.PhysMaterial.Get()].GetValue();
	if (BodyPart==NonePart) return;
	
	const bool IsInHead = IsHitInHead(HitResult.PhysMaterial.Get());
	if (GetWorld()->GetFirstPlayerController())
	{
		auto PlayerController = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
		if (PlayerController)
		{
			auto PlayerHUD = PlayerController->GetHUD<AGameHUD>();
			if (PlayerHUD)
			{
				auto HUDWidget = PlayerHUD->GetPlayerHUDWidget();
				if (HUDWidget)
				{
					auto PlayerHUDWidget = Cast<UPlayerHUDWidget>(HUDWidget);
					if (PlayerHUDWidget)
					{
						auto CrosshairWidget = PlayerHUDWidget->GetCrosshairWidget();

						if (CrosshairWidget)
						{
							CrosshairWidget->OnHitDetected(IsInHead);
						}
					}
				}
			}
		}
	}
}

void ARifleWeapon::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponFXComponent);
}

void ARifleWeapon::InitFX()
{
	if (bUseNiagaraMuzzleEffect == true)
	{
		if (!MuzzleFXComponentNiagara)
		{
			MuzzleFXComponentNiagara = SpawnMuzzleFXNiagara();
		}
	}
	else
	{
		if (!MuzzleFXComponentCascade)
		{
			MuzzleFXComponentCascade = SpawnMuzzleFXCascade();
		}
	}

	if (!FireAudioComponent)
	{
		FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzelSocketName);
	}
	SetFXActive(true);
}

void ARifleWeapon::SetFXActive(bool IsActive)
{
	if (bUseNiagaraMuzzleEffect == true)
	{
		if (MuzzleFXComponentNiagara)
		{
			MuzzleFXComponentNiagara->SetPaused(!IsActive);
			MuzzleFXComponentNiagara->SetVisibility(IsActive, true);
		}
	}
	else
	{
		if (MuzzleFXComponentCascade)
		{
			MuzzleFXComponentCascade->SetVisibility(IsActive, true);
			if (IsActive == true)
			{
				MuzzleFXComponentCascade->Activate();
			}
			else
			{
				MuzzleFXComponentCascade->Deactivate();
			}
		}
	}

	if (FireAudioComponent)
	{
		IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
	}
}

void ARifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	if (bUseNiagaraTraceEffect == true)
	{
		const auto TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), TraceFXNiagara, TraceStart);
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

AController* ARifleWeapon::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner());
	return Pawn ? Pawn->GetController() : nullptr;
}
