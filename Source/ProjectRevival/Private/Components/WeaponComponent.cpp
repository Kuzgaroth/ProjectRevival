// Project Revival. All Rights Reserved


#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "Miscellaneous/AnimNotify/EquipFinishedAnimNotify.h"
#include "Miscellaneous/AnimNotify/ReloadFinishedAnimNotify.h"
#include "Miscellaneous/AnimUtils.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UWeaponComponent::StartFire()
{
	if (!CanFire()) return;
	CurrentWeapon->StartFire();
	ShootingInProgress = true;
}

void UWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
	ShootingInProgress = false;
}

bool UWeaponComponent::IsShooting()
{
	return ShootingInProgress;
}

TArray<UMaterialInstanceDynamic*> UWeaponComponent::GetCurrentWeaponMaterials()
{
	return CurrentWeapon->GetDynMaterials();
}


void UWeaponComponent::NextWeapon()
{
	return;
	if (!CanEquip()) return;
	CurrentWeaponIndex = (CurrentWeaponIndex+1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::Reload()
{
	ChangeClip();
}

bool UWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if (CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}

bool UWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if (CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}

bool UWeaponComponent::TryToAddAmmo(TSubclassOf<ABaseWeapon> WeaponType, int32 ClipsAmount)
{
	for (const auto Weapon : Weapons)
	{
		if (Weapon && Weapon->IsA(WeaponType))
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	
	EquipWeapon(CurrentWeaponIndex);
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
	for (auto Weapon: Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();	
	}
	Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::SpawnWeapons()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;
	
	for (auto WeaponData : WeaponDatas)
	{
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponData.WeaponClass);
		if (!Weapon) return;
		Weapon->OnClipEmpty.AddUObject(this, &UWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character);
		Weapons.Add(Weapon);
		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
	}
}

void UWeaponComponent::AttachWeaponToSocket(ABaseWeapon* Weapon, USceneComponent* CharacterMesh, const FName& SocketName)
{
	if (!Weapon || !CharacterMesh) return;
	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(CharacterMesh, AttachmentTransformRules, SocketName);
}

void UWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
	if (WeaponIndex<0 || WeaponIndex>Weapons.Num()) return;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(),WeaponArmorySocketName);
	}
	CurrentWeapon  = Weapons[WeaponIndex];
	//CurrentReloadAnimMontage=WeaponDatas[WeaponIndex].ReloadAnimMontage;
	const auto CurrentWeaponData = WeaponDatas.FindByPredicate([&](const FWeaponData& Data)
		{return CurrentWeapon->GetClass()==Data.WeaponClass;});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(),WeaponEquipSocketName);
	if (EquipAnimMontage)
	{
		EquipAnimInProgress = true;
		PlayAnimMontage(EquipAnimMontage);
	}
	
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* MontageToPlay) const
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;

	Character->PlayAnimMontage(MontageToPlay);
}

void UWeaponComponent::InitAnimations()
{
	
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimMontage);
	if (EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this,&UWeaponComponent::OnEquipFinished);
	}

	for (auto WeaponData : WeaponDatas)
	{
		auto ReloadFinishedNotify = AnimUtils::FindNotifyByClass<UReloadFinishedAnimNotify>(WeaponData.ReloadAnimMontage);
		if (!ReloadFinishedNotify) continue;
		
		ReloadFinishedNotify->OnNotified.AddUObject(this,&UWeaponComponent::OnReloadFinished);
	}
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh()!=MeshComponent) return;
	EquipAnimInProgress = false;
}

void UWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || Character->GetMesh()!=MeshComponent) return;
	ReloadAnimInProgress = false;
}

bool UWeaponComponent::CanFire()
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UWeaponComponent::CanEquip()
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool UWeaponComponent::CanReload()
{
	return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}

void UWeaponComponent::OnEmptyClip(ABaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;
	if (CurrentWeapon==AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else
	{
		for (const auto Weapon : Weapons)
		{
			if (Weapon==AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
				break;
			}
		}
	}
}

void UWeaponComponent::ChangeClip()
{
	if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimInProgress = true;
	PlayAnimMontage(CurrentReloadAnimMontage);
}


int32 UWeaponComponent::GetCurrentWeaponClips() const
{
	if (CurrentWeapon)
	{
		return  CurrentWeapon->GetAmmoData().Clips;
	}
	return -1;
}

int32 UWeaponComponent::GetCurrentWeaponBullets() const
{
	if (CurrentWeapon)
	{
		return  CurrentWeapon->GetAmmoData().Bullets;
	}
	return -1;
}

int32 UWeaponComponent::GetMaxWeaponClips() const
{
	if (CurrentWeapon)
	{
		return  CurrentWeapon->GetDefaultAmmoData().Clips;
	}
	return -1;
}

int32 UWeaponComponent::GetMaxWeaponBullets() const
{
	if (CurrentWeapon)
	{
		return  CurrentWeapon->GetDefaultAmmoData().Bullets;
	}
	return -1;
}
