// Project Revival. All Rights Reserved

#include "AKWeapon.h"
#include "GameFramework/Character.h"
#include "Weapon/Components/WeaponFXComponent.h"

AKWeapon::AKWeapon()
{
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AKWeapon::BeginPlay()
{
	Super::BeginPlay();
	SpawnMagazine();
	check(WeaponMesh);
}

void AKWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKWeapon::MakeShot()
{
	Super::MakeShot();
	if(ShutterMovement)
	{
		this->WeaponMesh->PlayAnimation(ShutterMovement, false);
	}
	SpawnShell();
}

void AKWeapon::SpawnShell() const
{
	if(!this || !AmmoShell) return;
	const FRotator Rotation = this->GetActorRotation();
	const FVector Location = WeaponMesh->GetSocketLocation(ShutterSocketName);
	GetWorld()->SpawnActor<AShell>(AmmoShell, Location, Rotation);
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Blue, __FUNCTION__);
}

void AKWeapon::SpawnMagazine() const
{
	const ACharacter* Character = Cast<ACharacter>(this->GetOwner());
	if(!Character) return;
	const FRotator Rotation = this->GetActorRotation();
	const FVector Location = Character->GetMesh()->GetSocketLocation(ArmSocketName);
	GetWorld()->SpawnActor<AMagazine>(Magazine, Location, Rotation);
	GEngine->AddOnScreenDebugMessage(-1,4.f,FColor::Blue, __FUNCTION__);
}
